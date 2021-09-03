// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "Components/Button.h"
#include "STUPauseWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

	private:
	UFUNCTION()
	void OnClearPause();
};