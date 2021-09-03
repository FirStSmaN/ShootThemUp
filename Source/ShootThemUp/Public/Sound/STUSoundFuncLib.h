// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "STUSoundFuncLib.generated.h"

class USoundClass;

UCLASS()
class SHOOTTHEMUP_API USTUSoundFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable)
	static void SetSoundClassVolume(USoundClass* SoundClass, float Volume);

	UFUNCTION(BlueprintCallable)
	static void TogleSoundClassVolume(USoundClass* SoundClass);
};
