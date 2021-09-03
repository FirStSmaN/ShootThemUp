// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()

public:

	ASTUProjectile();
	void SetShotDiraction(const FVector& Diraction) { ShotDiraction = Diraction; }
protected:

	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnyWhere, Category = "Weapon")
		USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnyWhere, Category = "Weapon")
		UProjectileMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere,  Category = "VFX")
		USTUWeaponFXComponent* WeaponFXComponent;

	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
		float DamageRadius = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 90.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool DoFullDamage = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float LifeSeconds = 5.0f;
	





private:

	FVector ShotDiraction;
	UFUNCTION()
		void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
};
