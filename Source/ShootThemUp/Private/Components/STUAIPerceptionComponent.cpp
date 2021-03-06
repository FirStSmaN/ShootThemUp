// Shoot Them Up game


#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy()const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	if(PercieveActors.Num() == 0)
	{
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
	
		if(PercieveActors.Num() == 0) return nullptr;
	}

	const auto Controller = Cast<AAIController>(GetOwner());
	if(!Controller) return nullptr;
	const auto Pawn = Controller->GetPawn();
	if(!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	for(const auto PercieveActor: PercieveActors)
	{
		const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);

		const auto PercievePawn = Cast<APawn>(PercieveActor);
		const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->GetController());
		

		if(HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = ((PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size());
			if(CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}

		}
		
	 if(!BestPawn) //  САМЫЙ КОСТЫЛЬНЫЙ КОСТЫЛЬ
	 {
		const auto WeaponComponent = Cast<USTUWeaponComponent>(Controller->GetPawn()->GetComponentByClass(USTUWeaponComponent::StaticClass())) ;
	 	WeaponComponent->GetCurrentWeapon()->StopFire();
	 }

	}
return BestPawn;	
}