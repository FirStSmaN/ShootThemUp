// Shoot Them Up game


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}





void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains((PhysMat)))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}

	// Niagara
	 UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NaigaraEffect, Hit.ImpactPoint,
	                                        Hit.ImpactNormal.Rotation());
	//CurrentNiagaraSystem = ImpactData.NaigaraEffect;  

	//Decals
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), //
	                                       ImpactData.DecalData.Material, //
	                                       ImpactData.DecalData.Size, //
	                                       Hit.ImpactPoint, //
	                                       Hit.ImpactNormal.Rotation());
	if(DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
		
	}
	//sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.ImpactSound, Hit.ImpactPoint);

}

/*void USTUWeaponFXComponent::StopSpawnFX()
{
	if(!CurrentNiagaraSystem) return;

	for(int EmiterIndex : CurrentNiagaraSystem->GetEmitterHandles().Num())
	CurrentNiagaraSystem->GetEmitterHandle(EmiterIndex).GetInstance()->bInterpolatedSpawning = false;
}*/


