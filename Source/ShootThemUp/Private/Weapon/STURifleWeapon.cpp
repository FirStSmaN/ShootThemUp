// Shoot Them Up game


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "STUBaseCharacter.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
	
	
	
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
	
	
}


void ASTURifleWeapon::StartFire()
{
	
	InitFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();

	
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetFXActive(false);
}

void ASTURifleWeapon::MakeShot()
{
	const auto Player = Cast<ASTUBaseCharacter>(GetOwner());
	
	if (!GetWorld()  || Player->IsRunning())
	{
		StopFire();
		return;
	}

	if(IsAmmoEmpty() || IsClipEmpty())
	{
		StopFire();
		
		UGameplayStatics::SpawnSoundAttached(NoAmmoSound, WeaponMesh, MuzzleSocketName);
		return;
	}
	

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		WeaponFXComponent->PlayImpactFX(HitResult);
		
		MakeDamage(HitResult);

	}

	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();

};

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;


	TraceStart = ViewLocation;

	const FVector ShootDiraction = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDiraction * TraceMaxDistance;
	return true;

}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamagedActor->TakeDamage(BaseDamage, PointDamageEvent, GetController(), this);
}


void ASTURifleWeapon::InitFX()
{
	if(!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	if(!FireAudioComponent )
	{
		UE_LOG(LogRifleWeapon, Warning, TEXT("NEW AUDIOCOMPONENT"))
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}
	SetFXActive(true);
}

void ASTURifleWeapon::SetFXActive(bool IsActive)
{
	if(MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}
	
	if(FireAudioComponent)
	{
		FireAudioComponent->SetPaused(!IsActive);

		/*if(IsActive)
		{
			UE_LOG(LogRifleWeapon, Display, TEXT("Play audio"));
			FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
		}else
		{
			UE_LOG(LogRifleWeapon, Display, TEXT("Stop audio"));
			FireAudioComponent->Deactivate();
			
			FireAudioComponent = nullptr;
			//FireAudioComponent->Deactivate(); // костыль
		}*/
		   
	}
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);

	if(TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

AController* ASTURifleWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn ->GetController() : nullptr;
}

void ASTURifleWeapon::Zoom(bool Enabled)
{
	const auto Controller = Cast<APlayerController>(GetController());
	if(!Controller || !Controller->PlayerCameraManager) return;

	if(Enabled)
	{
		DefaultFOV = Controller->PlayerCameraManager->GetFOVAngle();
	}

	Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultFOV);
}