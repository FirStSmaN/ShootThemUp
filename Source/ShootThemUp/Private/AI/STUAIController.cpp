// Shoot Them Up game


#include "AI/STUAIController.h"

#include "AI/STUAICharacter.h"
#include "STUUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include  "Components/STUAIPerceptionComponent.h"
#include "STURespawnComponent.h"
#include "STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAIController, All, All);

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);


	
	
	bWantsPlayerState = true;
	
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}


void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
	if(STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if(!GetBlackboardComponent())
	{		
		return nullptr;
	}
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

