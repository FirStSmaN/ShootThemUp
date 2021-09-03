// Shoot Them Up game


#include "AI/Servises/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUBaseWeapon.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"



USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = "Find Enemy";
}


void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		const auto controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(controller);
		if(PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
			
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}