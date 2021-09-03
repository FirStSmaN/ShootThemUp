// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTURespawnComponent();

	int32 GetRespawnCountDown() const {return RespawnCountDown;}
bool IsRespawnInProgress() const;
	
void Respawn(int32 RespawnTime);
	
protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle RespawnTimerHandle;
	int32 RespawnCountDown = 0;
	
	void RespawnTimeUpdate();
		
};
