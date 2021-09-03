// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
	ASTUPlayerController();
	virtual void SetupInputComponent() override;
	protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
	USTURespawnComponent* RespawnComponent;


	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	private:
	void OnGamePaused();
	void OnMatchStateChanged(ESTUMatchState State);
	void OnMuteSound();
};
