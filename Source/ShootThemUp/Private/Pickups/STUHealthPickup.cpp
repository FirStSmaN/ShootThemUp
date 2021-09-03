// Shoot Them Up game


#include "Pickups/STUHealthPickup.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

 bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn) 
 {
 	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
 	if(!HealthComponent) return false;
 	
 	UE_LOG(LogHealthPickup, Display, TEXT("Health was taken"));
	return HealthComponent->TryToAddHealth(HealthToAdd);
 }
