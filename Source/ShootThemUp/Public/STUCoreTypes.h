#pragma once
//#include "STUWeaponFXComponent.h"
#include "STUCoreTypes.generated.h"

//weapon

class ASTUBaseWeapon;
class USoundCue;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeapon*)



USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta=(EditCondition = "!Infinite"))
	int32 Clips;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
	
};

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,  Category = "Weapon")
	TSubclassOf<ASTUBaseWeapon> WeaponClasses;

	UPROPERTY(EditDefaultsOnly,  Category = "Weapon")
	UAnimMontage* RealoadAnimMontage;

	
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "UI")
	UTexture2D* MainIcon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "UI")
	UTexture2D* CrossHairIcon;
};


//health
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float)



// VFX

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "VFX")
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "VFX")
	FVector Size = FVector(10.0f);

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "VFX")
	float LifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "VFX")
	float FadeOutTime = 0.5f;

	
};


USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "VFX")
	UNiagaraSystem* NaigaraEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "VFX")
	FDecalData DecalData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundCue* ImpactSound;


};


USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
	int32  PlayersNum = 2;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 RoundsNum = 2;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
	int32 RoundsTime = 10; //in sec
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
	FLinearColor DefalultTeamColor = FLinearColor::Red;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
	TArray<FLinearColor> TeamColors = {FLinearColor::Red,FLinearColor::Blue, FLinearColor::Yellow, FLinearColor::Gray};
	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
	int32 RespawnTime = 5; //in sec
};

UENUM(BlueprintType)
enum class ESTUMatchState :uint8
{
	WaitingToStart = 0,
	InProgress,
	Pause,
	GameOver
	
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature,ESTUMatchState);

//TMap<int32,FLinearColor> STUTeamColorsMap;  = [{0, FLinearColor::Red;},{ 1,FLinearColor::Green; }]

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game")
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game")
	FName LevelDisplayName = NAME_None;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category = "Game")
	UTexture2D* LevelThumb;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);
