// Shoot Them Up game



#include "STUGameHUD.h"

#include "DataTableUtils.h"
#include "Engine/Canvas.h"
#include "STUBaseGameModeBase.h"
#include "UI/STUBaseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All)

void ASTUGameHUD::DrawHUD()  
{
	Super::DrawHUD();

	//DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair() 
{
	int32 SizeX = Canvas->SizeX;
	int32 SizeY = Canvas->SizeY;

	const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor CrossHairColor = FLinearColor::Gray;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, CrossHairColor, LineThickness);
	DrawLine(Center.Min , Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, CrossHairColor, LineThickness);
}



void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();


	GameWidgets.Add(ESTUMatchState::InProgress,CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass) );
	GameWidgets.Add(ESTUMatchState::Pause,CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass) );
	
	GameWidgets.Add(ESTUMatchState::GameOver,CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass) );

	for(auto GameWidgetPair: GameWidgets)
	{
		const auto GameWidget = GameWidgetPair.Value;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

		
	if(GetWorld())
	{
		const auto GameMode = Cast<ASTUBaseGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnGameStateChanged);
		}
	}	
}


void ASTUGameHUD::OnGameStateChanged(ESTUMatchState State)
{
	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}

	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}

	
	UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}