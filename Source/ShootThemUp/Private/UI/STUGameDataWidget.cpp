// Shoot Them Up game


#include "UI/STUGameDataWidget.h"
#include "STUBaseGameModeBase.h"
#include "Player/STUPlayerState.h"


int32 USTUGameDataWidget::GetRoundSecondsRemaining() const
{
	const auto GameMode = GetGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}
int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}
int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetGameMode();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

ASTUBaseGameModeBase* USTUGameDataWidget::GetGameMode() const
{
	if(!GetWorld())return nullptr;
	const auto GameMode = Cast<ASTUBaseGameModeBase>(GetWorld()->GetAuthGameMode());
	return GameMode;
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
	return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
