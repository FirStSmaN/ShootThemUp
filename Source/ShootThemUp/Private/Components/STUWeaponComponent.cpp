// Shoot Them Up game


#include "Components/STUWeaponComponent.h"

#include <string>

#include "Animations/AnimUtils.h"
#include "STUReloadFinishedAnimNotify.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, Display, Error)
constexpr int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}



void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can carry only %i weapons"), WeaponNum);
	SpawnWeapons();
	CurrentWeaponIndex = 0;
	EquipWeapon(CurrentWeaponIndex);
	InitAnimations();
	CurrentWeapon->StopFire();
	
	

}

void USTUWeaponComponent::SpawnWeapons()
{	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!GetWorld() || !Character) return;

	for (auto OneWeaponData : WeaponData) 
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClasses);
			if (!Weapon) continue;
			Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
			Weapon->SetOwner(Character);
			Weapon->SetOwner(Character);
			Weapons.Add(Weapon);

			AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	
	}
	
	


	
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent,const FName& SocketName) 
{
	if (!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
	

}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) 
{
	if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		return;
		
	}
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	
	if (CurrentWeapon) 
	{
		CurrentWeapon->StopFire();
		Zoom(false);
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}
	CurrentWeapon = Weapons[WeaponIndex];
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)
	{
		return Data.WeaponClasses == CurrentWeapon->GetClass();
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->RealoadAnimMontage : nullptr;
	
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	PlayAnimMontage(EquipAnimMontage);
	EquipAnimInProgress = true;
	UE_LOG(LogTemp, Display, TEXT("EquipWeapon"))
	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::StartFire() 
{
	
	if (!CanFire()) return;
	
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon() 
{
	if (!CanEquip()) return;
	
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons) 
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
 }

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) 
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations() 
{


	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify) 
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	} else
	{
		UE_LOG(LogWeaponComponent,Error, TEXT("Equip anim motify is forgotten to set") );
		checkNoEntry();
		
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.RealoadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent,Error, TEXT("Reload anim motify is forgotten to set") );
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}
}
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	if (!Character || MeshComp != Character->GetMesh()) return;
	 
	EquipAnimInProgress = false;
	
}

bool USTUWeaponComponent::CanFire() const
{
	UE_LOG(LogTemp, Display, TEXT("CurrentWeapon %i,  EquipAnimInProgress %i"),CurrentWeapon, EquipAnimInProgress )
		
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

void USTUWeaponComponent::Reload()
{
	
	ChangeClip();
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	if (!Character || MeshComp != Character->GetMesh()) return;
	 
	ReloadAnimInProgress = false;
	
}


bool USTUWeaponComponent::CanReload()
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
	}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if(CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for(const auto Weapon: Weapons)
		{
			if(CurrentWeapon == AmmoEmptyWeapon)
			{
				ChangeClip();
			}
		}
	}
	
}
void USTUWeaponComponent::ChangeClip()
{
	if(!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	PlayAnimMontage(CurrentReloadAnimMontage);
	ReloadAnimInProgress = true;
	
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
	if(CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for(const auto Weapon: Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
		
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
	for(const auto Weapon: Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}

void USTUWeaponComponent::Zoom(bool Enabled)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled);
	}
}