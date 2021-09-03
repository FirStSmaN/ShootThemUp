// Shoot Them Up game


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
void ASTULauncherWeapon::StartFire()
{

	MakeShot();
}
void ASTULauncherWeapon::MakeShot()
{
	if (!GetWorld())
	{
		return;
	}

	if(IsAmmoEmpty())
	{
		StopFire();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return;
	}
	
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Diraction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();


	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
	DecreaseAmmo();
	if (Projectile)
	{
		Projectile->SetShotDiraction(Diraction);
		Projectile->FinishSpawning(SpawnTransform);
		Projectile->SetOwner(GetOwner());
	}
	SpawnMuzzleFX();
	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}



