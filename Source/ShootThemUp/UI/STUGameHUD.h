// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"

class USTUBaseWidget;
UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
private:
	void DrawCrossHair();
	TMap<ESTUMatchState, USTUBaseWidget*> GameWidgets;

	USTUBaseWidget* CurrentWidget = nullptr;
	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="UI" )
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="UI" )
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="UI" )
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	
	virtual void BeginPlay() override;
	void OnGameStateChanged(ESTUMatchState State);

};
