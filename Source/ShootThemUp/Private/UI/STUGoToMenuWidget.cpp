// Shoot Them Up game


#include "UI/STUGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"


DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenuWidget, All, All);
void USTUGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(GoToMenuButton)
	{
		
		GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::GoToMenu);
	}
}

void USTUGoToMenuWidget::GoToMenu()
{
	
	if(!GetWorld()) return;
	const auto GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();

	if(!GameInstance) return;
	if(GameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogSTUGoToMenuWidget, Error, TEXT("Level name is NONE"));
		return;
	}

	
	
	UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());


	
}