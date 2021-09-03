// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()


public:
	ASTUAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float HealthVisibilityDistance = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	virtual void OnDeath() override;

	

	virtual void OnHealthChanged(float Health, float HealthDelta) override;

	private:
	//void StopChild();
	void UpdateHealWidgetVisibility();
};
