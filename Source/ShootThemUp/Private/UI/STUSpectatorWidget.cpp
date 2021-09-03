// Shoot Them Up game


#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "STURespawnComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUSpectatorWidget, All, All);

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer());	   //Cast<USTURespawnComponent>(GetOwningPlayer());
	if(!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) { return false;}

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}