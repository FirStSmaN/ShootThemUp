// Shoot Them Up game


#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{
	if(!HealthBar) return;

	const auto HealthBarVisibility = (Percent > PercentVisibilityThereshold || FMath::IsNearlyZero(Percent) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	
	HealthBar->SetVisibility(HealthBarVisibility);
	const auto HealthBarColor = Percent>PercentColorThereshold ? GoodColor : BadColor;
	HealthBar->SetFillColorAndOpacity(HealthBarColor);

	HealthBar->SetPercent(Percent);
}