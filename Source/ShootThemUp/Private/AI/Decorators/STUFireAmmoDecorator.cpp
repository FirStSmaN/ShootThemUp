// Shoot Them Up game


#include "AI/Decorators/STUFireAmmoDecorator.h"
#include "STUUtils.h"
#include "AI/STUAIController.h"
#include "Components/STUWeaponComponent.h"


USTUFireAmmoDecorator::USTUFireAmmoDecorator()
{
	NodeName =  "Need Ammo";
}

bool USTUFireAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if(!Controller) return false;

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if(!WeaponComponent) return false;

	
	return WeaponComponent->NeedAmmo(WeaponClasses);

}