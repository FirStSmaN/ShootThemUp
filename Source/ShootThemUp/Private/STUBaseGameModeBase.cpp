// Shoot Them Up game


#include "STUBaseGameModeBase.h"

#include <chrono>

#include "Player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"
#include "AIController.h"

#include "STUGameHUD.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "EngineUtils.h"
#include "STURespawnComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Sound/SoundCue.h"
#include "STUGameInstance.h"
#include "STUWeaponComponent.h"
#include "GameFramework/GameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModBase, All, All)

ASTUBaseGameModeBase::ASTUBaseGameModeBase() 
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
	PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUBaseGameModeBase::StartPlay()
{
	Super::StartPlay();

	
	SpawnBots();
	CreateTeamsInfo();
	

	CurrentRound = 1;
	StartRound();

	SetMatchState(ESTUMatchState::InProgress);
}

void ASTUBaseGameModeBase::SpawnBots()
{
	if(!GetWorld()) return;
	for(int32 i = 0; i < GameData.PlayersNum - 1; i++)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(STUAIController);

	}
}


UClass* ASTUBaseGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if(InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}


void ASTUBaseGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundsTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUBaseGameModeBase::GameTimerUpdate, 1.0f, true);


	
	/*const TArray<FString> SoundClassNames = {"SC_Weapon"}; // мой костыль по удалению акторов звука оружия
	if(GetWorld()) 
	{
		TArray<AActor*> SoundCues;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),USoundCue::StaticClass(), SoundCues );

		for(auto SoundCue : SoundCues)
		{
			auto Sound = Cast<USoundCue>(SoundCue);
			if(SoundClassNames.Contains(Sound->SoundClassObject->GetFullName()))
			{
				UE_LOG(LogTemp, Error, TEXT("YESSSSS"));
				Sound->BeginDestroy();
			}
		}
	}*/

	
	


}

void ASTUBaseGameModeBase::GameTimerUpdate()
{
	//UE_LOG(LogSTUGameModBase, Display, TEXT("Time: %i, Round: %i/%i"),RoundCountDown, CurrentRound, GameData.RoundsNum );
	

	if(-- RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
		if(CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();

			
			
		}else
		{
			GameOver();
			
			
		}
	}

}

void ASTUBaseGameModeBase::ResetPlayers()
{
if(!GetWorld()) return;
	
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}


void ASTUBaseGameModeBase::ResetOnePlayer(AController* Controller)
{
	
	if(Controller && Controller->GetPawn())
	{
		
		Controller->GetPawn()->Reset();
	}
	
	RestartPlayer(Controller);
	SetPlayerColor(Controller);
	
	
}

void ASTUBaseGameModeBase::CreateTeamsInfo()
{
	if(!GetWorld()) return;

	int32 TeamID = 1;
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;

		const auto PlayerState =Cast<ASTUPlayerState>( Controller->PlayerState);
		if(!PlayerState) continue;

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		SetPlayerColor(Controller);
		TeamID = TeamID == 1 ? 2 : 1;
		
		
	}
}
FLinearColor ASTUBaseGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
	
	
	if(TeamID -1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID-1];
	}
	UE_LOG(LogSTUGameModBase, Warning, TEXT("No color for TeamID: %i, set do default: %s"), TeamID, *GameData.DefalultTeamColor.ToString());
	return GameData.DefalultTeamColor;
}
void ASTUBaseGameModeBase::SetPlayerColor(AController* Controller)
{
	if(!Controller) return;
	const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
	if(!Character) return;

	
	
	const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if(!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUBaseGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
	const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

	if(KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}
	if(VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}


	StartRespawn(VictimController);
}

void ASTUBaseGameModeBase::LogPlayerInfo()
{
	if(!GetWorld()) return;
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;

		const auto PlayerState =Cast<ASTUPlayerState>( Controller->PlayerState);
		if(!PlayerState) {UE_LOG(LogSTUGameModBase, Warning, TEXT("Invalid PlayerState")); continue;}
	
		
		PlayerState->Loginfo();
	}
}

void ASTUBaseGameModeBase::StartRespawn(AController* Controller)
{
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);

	if(!RespawnComponent) return;
	if(GameData.RespawnTime > RoundCountDown) return;  // Фикс респавна
	
	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUBaseGameModeBase::RespawnRequest(AController* Controller)
{

	ResetOnePlayer(Controller);
}

void ASTUBaseGameModeBase::GameOver()
{
	UE_LOG(LogSTUGameModBase, Display, TEXT("-----GAME OVER-----"));
	LogPlayerInfo();
	
	{
		
		for(auto Pawn : TActorRange<APawn>(GetWorld()))
		{
			if(Pawn)
			{
				Pawn->TurnOff();
				Pawn->DisableInput(nullptr);
				
			}
		}
	}

	SetMatchState(ESTUMatchState::GameOver);
}

void ASTUBaseGameModeBase::SetMatchState(ESTUMatchState State)
{
	if(MatchState == State) return;
	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
	
}

bool ASTUBaseGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate );
	if(PauseSet)
	{
		StopAllFire();
		SetMatchState(ESTUMatchState::Pause);
	}
	
	return PauseSet;
}

bool ASTUBaseGameModeBase::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();

	if(PauseCleared)
	{
		SetMatchState(ESTUMatchState::InProgress);
	}
	return PauseCleared;
}

void ASTUBaseGameModeBase::StopAllFire()
{
	for(auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Pawn);
		if(!WeaponComponent) continue;

		WeaponComponent->StopFire();
		WeaponComponent->Zoom(false);
		
	}
}