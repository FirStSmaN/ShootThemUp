// Shoot Them Up game

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

class ASTUBaseGameModeBase;
class UProgressBar;
class UWidgetAnimation;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString FormatBullets(int32 BulletNum) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetKillsNum() const;

	virtual void NativeOnInitialized() override;
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage();

	protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentColorThereshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor GoodColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor BadColor = FLinearColor::Red;
	
	

	UPROPERTY(BlueprintReadWrite,meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation;
	
private:
	void OnHealthChanged(float Health, float HealthDelta);

	ASTUBaseGameModeBase* GetGameMode() const;

	void OnNewPawn(APawn* NewPawn);

	void UpdateHealthBar();
};
