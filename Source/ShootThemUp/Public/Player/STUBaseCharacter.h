// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Character.h>
#include "STUBaseCharacter.generated.h"


class USTUHealthComponent;
class USoundCue;

class USTUWeaponComponent;


UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjectInit);
	virtual void OnHealthChanged(float Health, float HealthDelta);

	virtual void TurnOff() override;
	virtual void Reset() override;

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USTUHealthComponent* HealthComponent;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FName MaterialColorName = "Paint Color";

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite ,Category = "Sound")
	USoundCue* DeathSound;
	
	virtual void OnDeath();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	void SetPlayerColor(const FLinearColor& Color);
private:
	

	

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
	

	
};
