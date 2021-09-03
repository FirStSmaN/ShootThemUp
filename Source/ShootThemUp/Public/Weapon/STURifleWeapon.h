// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"
class UNiagaraComponent;
class UNiagaraSystem;
class USTUWeaponFXComponent;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:
	ASTURifleWeapon();
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Zoom(bool Enabled)override; 
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float BulletsSpread = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float TimeBetweenShots = 0.1f;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float FOVZoomAngle = 50.0f;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category = "VFX")
	FString TraceTargetName = "TraceTarget";
	
	virtual void BeginPlay() override;
	void MakeDamage(const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    UAudioComponent* FireAudioComponent;
private:
	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	float DefaultFOV = 90.0f;
	
	FTimerHandle ShotTimerHandle;
	
	
	
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
	void InitFX();
	void SetFXActive(bool IsActive);

	AController* GetController() const;
	
};
