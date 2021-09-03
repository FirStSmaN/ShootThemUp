// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

class USoundClass;

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
	FLevelData GetStartupLevelName() const {return StartupLevel;}
	void SetStartupLevelName(const FLevelData& Data){ StartupLevel = Data;}
	TArray<FLevelData> GetLevelsData() const{return LevelsData;}
	FName GetMenuLevelName() const {return MenuLevelName;}

	void ToggleVolume();

	protected:
	UPROPERTY(EditDefaultsOnly,Category = "Game", meta = (Tooltip = "Level names must ne uniqe!"))
	TArray<FLevelData> LevelsData;
	

	UPROPERTY(EditDefaultsOnly,Category = "Game")
	FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly,Category = "Sound")
	USoundClass* MasterSoundClass;

	private:
	FLevelData StartupLevel;
};
