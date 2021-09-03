// Shoot Them Up game


#include "Components/STURespawnComponent.h"
#include "STUBaseGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTURespawnComponent, All, All);

USTURespawnComponent::USTURespawnComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	
}



void USTURespawnComponent::BeginPlay()
{
	Super::BeginPlay();


	
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
	if(!GetWorld())return;


	
	
	
	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimeUpdate, 1.0, true);
	
}

void USTURespawnComponent::RespawnTimeUpdate()
{
	if(--RespawnCountDown != 0) return;
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
	const auto GameMode = Cast<ASTUBaseGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!GameMode) return;

	GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
	
	
	return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}




