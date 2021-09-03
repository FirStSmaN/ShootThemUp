// Shoot Them Up game


#include "UI/STUGameOverWidget.h"

#include <string>

#include "STUBaseGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "STUUtils.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void USTUGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if(GetWorld())
	{
		const auto GameMode = Cast<ASTUBaseGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
		}
	}	
	
	if(ResetLevelButton)
	{
		ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
	}
}
void  USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
	if(State == ESTUMatchState::GameOver)
	{
		UpdatePlayerStat();
	}
}
void  USTUGameOverWidget::UpdatePlayerStat()
{
	if(!GetWorld() || !PlayerStatBox) return;

	PlayerStatBox->ClearChildren();
	
	for(auto It = GetWorld()->GetControllerIterator();It; It++)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;
		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState) ;
		if(!PlayerState) continue;

		const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(),PlayerStatRowWidgetClass );
		FLinearColor PlayerColor = PlayerState->GetTeamColor();
		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()),PlayerColor);
		PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
		PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathNum()));
		PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
		PlayerStatRowWidget->SetPlayerIndicatorImage(Controller->IsPlayerController());
		
		PlayerStatRowWidget->SetTeamColor(PlayerColor);
		
		PlayerStatBox->AddChild(PlayerStatRowWidget);	
	}
}

void USTUGameOverWidget::OnResetLevel()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}