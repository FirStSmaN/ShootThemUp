// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUBaseGameModeBase.generated.h"

class AAIController;


UCLASS()
class SHOOTTHEMUP_API ASTUBaseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASTUBaseGameModeBase();

	FOnMatchStateChangedSignature OnMatchStateChanged;
	virtual void StartPlay() override;
	virtual  UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	FGameData GetGameData() const {return GameData;}
	int32 GetCurrentRoundNum() const {return CurrentRound;}
	int32 GetRoundSecondsRemaining() const {return RoundCountDown;}
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	void RespawnRequest(AController* Controller);
	void Killed(AController* KillerController, AController* VictimController);

	virtual bool ClearPause() override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;
	FTimerHandle GameRoundTimerHandle;

	private:
	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	
	
	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

	void CreateTeamsInfo();
	FLinearColor DetermineColorByTeamID(int32 TeamID) const;
	void SetPlayerColor(AController* Controller);

	void LogPlayerInfo();

	void StartRespawn(AController* Controller);

	void SetMatchState(ESTUMatchState State);

	void GameOver();

	void StopAllFire();
};
