// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
	GENERATED_BODY()
private:	
	virtual bool GivePickupTo(APawn* PlayerPawn)override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Pickup", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float HealthToAdd;

	
};
