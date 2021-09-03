// Shoot Them Up game


#include "AI/Servises/STUFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUFireService, All, All);
USTUFireService::USTUFireService()
{
	NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto Controller = OwnerComp.GetAIOwner();
	
	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if(Controller)
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
		if(WeaponComponent)
		{
			if(HasAim)
			{
				
				 WeaponComponent->StartFire();
			}else
			{
				WeaponComponent->StopFire();
			}
			UE_LOG(LogSTUFireService, Error, TEXT("Has Aim: %i"), HasAim);
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
