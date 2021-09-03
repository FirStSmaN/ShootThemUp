// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"


class UPhysicalMaterial;
class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USTUHealthComponent();
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
		bool IsDead() const { return FMath::IsNearlyEqual(Health, 0.0f) ? true : false; }
    UFUNCTION(BlueprintCallable, Category = "Health")
        float GetHealthPercent() const {return Health / MaxHealth;}
    bool TryToAddHealth(float HealthToAdd);
	
	bool IsHealthFull()const;
	
	void ApplyDamage(float Damage, AController* InstigatedBy);
	
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;
protected:

	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealModifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealUpdateTime = 0.02f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	TMap<UPhysicalMaterial* , float> DamageModifiers;
	
private:
	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser );

	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser );

	void HealUpdate();

	void SetHealth(float NewHealth);

	FTimerHandle HealTimerHandle;

	void PlayCameraShake();
	

	void Killed(AController* KillerController);

	float GetPointDamageModifier(AActor* DamagedActor, FName& BoneName);

	void ReportDamageEvent(float Damage, AController* InstigatedBy);

};
