// Shoot Them Up game


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjectInit) : Super(ObjectInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
	{
		AutoPossessAI = EAutoPossessAI::Disabled;
		AIControllerClass = ASTUAIController::StaticClass();

		bUseControllerRotationYaw = false;
		if(GetCharacterMovement())
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0, 200.0f, 0.0f);
		}
	
	
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
	
	}

void ASTUAICharacter::BeginPlay()
{
	Super::BeginPlay();


	
	check(HealthWidgetComponent);
}


void ASTUAICharacter::OnDeath()
{
	Super::OnDeath();
	const auto STUController = Cast<AAIController>(Controller);

	if(STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();
	}



}

 void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
	Super::OnHealthChanged(Health, HealthDelta);

	const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if(!HealthBarWidget) return;

	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTUAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealWidgetVisibility();
}

void ASTUAICharacter::UpdateHealWidgetVisibility()
{
	if(!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) return;
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();

	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());

	HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}

/*void ASTUAICharacter::StopChild()
{
	for(auto Child : RootComponent->GetNumChildrenComponents())
	{
		if(!Child) continue;
		auto ChildComponent = Cast<UActorComponent>(Child);
		if(ChildComponent)
		{
			ChildComponent->Stop();
		}
	}
}*/