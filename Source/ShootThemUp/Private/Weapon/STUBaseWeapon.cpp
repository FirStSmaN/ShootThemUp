// Shoot Them Up game


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);


ASTUBaseWeapon::ASTUBaseWeapon()
{

	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}


void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));
	CurrentAmmo = DefaultAmmo;
	UE_LOG(LogBaseWeapon, Display, TEXT("%i"),DefaultAmmo.Clips );
}

void ASTUBaseWeapon::StartFire()
{


	
}

void ASTUBaseWeapon::StopFire()
{
	
}

void ASTUBaseWeapon::MakeShot()
{
	

}



bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto STUChatacter = Cast<ACharacter>(GetOwner());
	if(!STUChatacter) return false;
	if(STUChatacter->IsPlayerControlled())
	{
			const auto Controller = STUChatacter->GetController<APlayerController>();
        	if (!Controller) return false;
        
        
        	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
        
        	
	}else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	return true;

}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;


	TraceStart = ViewLocation;
	
	const FVector ShootDiraction =ViewRotation.Vector() ;
	TraceEnd = TraceStart + ShootDiraction * TraceMaxDistance;
	return true;

}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{

	if (!GetWorld())return;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, CollisionParams);

}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if(CurrentAmmo.Bullets > 0)
	{
		CurrentAmmo.Bullets --;
		LogAmmo();
		if(IsClipEmpty() && !IsAmmoEmpty())
		{
			StopFire();
			OnClipEmpty.Broadcast(this);	
		}
	}
}
bool ASTUBaseWeapon::IsAmmoEmpty()const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips <= 0 && IsClipEmpty(); 
	
}


bool ASTUBaseWeapon::IsClipEmpty()const
{
return CurrentAmmo.Bullets <= 0 ; 
	
}


void ASTUBaseWeapon::ChangeClip()
{
	
	
	if(!CurrentAmmo.Infinite)
	{
		
		CurrentAmmo.Clips--;
	}
	this->CurrentAmmo.Bullets = this->DefaultAmmo.Bullets;
}


void ASTUBaseWeapon::LogAmmo()
{

	FString AmmoInfo = "Ammo " + FString::FromInt(CurrentAmmo.Bullets) +  " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite " : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"),*AmmoInfo )
}

bool ASTUBaseWeapon::CanReload()
{
	return CurrentAmmo.Bullets <DefaultAmmo.Bullets && CurrentAmmo.Clips;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	return	UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMesh, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == CurrentAmmo.Bullets;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if(CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0 ) return false;

	if(IsAmmoEmpty())
	{
		CurrentAmmo.Clips =FMath::Clamp(CurrentAmmo.Clips+ClipsAmount, 0, DefaultAmmo.Clips+1);
		OnClipEmpty.Broadcast(this);
	}
	else if(CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
		if(DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}
	return true;

}

