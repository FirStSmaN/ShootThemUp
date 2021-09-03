// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"

#include "STUBaseWeapon.h"
#include "Pickups/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
	GENERATED_BODY()
	private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	int32 ClipsAmount = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	TSubclassOf<ASTUBaseWeapon> WeaponType;
};
