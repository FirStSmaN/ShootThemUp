// Shoot Them Up game
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text, const FLinearColor& TeamColor)
{
	if(!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
	PlayerNameTextBlock->SetColorAndOpacity(FSlateColor(TeamColor));
}
void USTUPlayerStatRowWidget::SetKills(const FText& Text)
{
	if(!KillsTextBlock) return;
	KillsTextBlock->SetText(Text);
}
void USTUPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if(!DeathsTextBlock) return;
	DeathsTextBlock->SetText(Text);
}
void USTUPlayerStatRowWidget::SetTeam(const FText& Text)
{
	if(!TeamTextBlock) return;
	TeamTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetPlayerIndicatorImage(bool Visible)
{
	if(!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USTUPlayerStatRowWidget::SetTeamColorToText(const FLinearColor& TeamColor)
{
	if(!TeamTextBlock) return;

	
	//TeamTextBlock->SetColorAndOpacity(SlateColor.GetSpecifiedColor());
	
	TeamTextBlock->SetColorAndOpacity(FSlateColor(TeamColor));

	
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor& TeamColor)
{
	if(!TeamImage) return;
	TeamImage->SetColorAndOpacity(TeamColor);
}