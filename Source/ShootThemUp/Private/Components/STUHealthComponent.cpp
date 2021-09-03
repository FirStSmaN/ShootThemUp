// Shoot Them Up game


#include "Components/STUHealthComponent.h"
#include <GameFramework/Actor.h>
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShake.h"
#include <Engine/World.h>
#include <TimerManager.h>
#include "STUBaseCharacter.h"
#include "STUBaseGameModeBase.h"
#include "Perception/AISense_Damage.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetHealth(MaxHealth);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
		
	}
	
	

	
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                          class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("On Any Damage: %f"), Damage);
	
	
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	
	
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	
	
	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
	

}

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;
	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager)return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

bool USTUHealthComponent::TryToAddHealth(float HealthToAdd)
{
	if (IsDead() || IsHealthFull() ) return false;
	
	SetHealth(Health + HealthToAdd);
	return true;
	

}


bool USTUHealthComponent::IsHealthFull()const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if(!GetWorld()) return;
	const auto GameMode = Cast<ASTUBaseGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!GameMode) return;
	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);

}


void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser )
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
 	
	UE_LOG(LogHealthComponent, Display, TEXT("On Point Damage: %f, bone: %s"), FinalDamage, *BoneName.ToString());

	ApplyDamage(FinalDamage, InstigatedBy);
	
}


void  USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser )
{
	UE_LOG(LogHealthComponent, Display, TEXT("On Radial Damage: %f"), Damage);
	
	ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	SetHealth(Health - Damage);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime,
                                               true, HealDelay);
	}
	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
	
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamagedActor);
	if(!Character ||											//
		!Character->GetMesh() ||								//
		!Character->GetMesh()->GetBodyInstance(BoneName) ||		//
		!Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial())
		return 1.0f;

	const auto Physmaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if( !Physmaterial ||!DamageModifiers.Contains(Physmaterial)) return 1.0;

	return DamageModifiers[Physmaterial]; 

}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if(!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
	
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
	
}
