// Shoot Them Up game


#include "Menu/UI/STULevelItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

void USTULevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemUnhovered);
	}
}

void USTULevelItemWidget::SetSelected(bool IsSelected)
{

	if(LevelImage)
	{
		LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Red : FLinearColor::White);
	}
}

void USTULevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;
	if(LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(LevelData.LevelDisplayName));
	}
	
	if(LevelImage)
	{
		LevelImage->SetBrushFromTexture(LevelData.LevelThumb);
	}
	
}
FLevelData USTULevelItemWidget::GetLevelData() const
{
	return LevelData;
}

void USTULevelItemWidget::OnLevelItemHovered()
{
	if(FrameImage)
	{
	FrameImage->SetVisibility(ESlateVisibility::Visible );
	}
}


void USTULevelItemWidget::OnLevelItemUnhovered()
{
	if(FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	}	
}