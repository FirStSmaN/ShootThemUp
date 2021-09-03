// Shoot Them Up game


#include "STUGameInstance.h"

#include "Sound/STUSoundFuncLib.h"

void USTUGameInstance::ToggleVolume()
{
	USTUSoundFuncLib::TogleSoundClassVolume(MasterSoundClass);
}