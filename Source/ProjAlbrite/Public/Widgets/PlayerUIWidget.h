// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/AlbriteAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Enums/GameEnums.h"
#include "Player/AlbritePlayerState.h"
#include "PlayerUIWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCooldownTriggered, float, Duration, EAbilityInputID, AbilityId);
UCLASS()
class PROJALBRITE_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetHealth(float CurrHealth, float MaxHealth) const;
	UFUNCTION(BlueprintCallable)
	void SetShield(float CurrShield, float MaxShield) const;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ShieldBar;

	/** Widget stat display **/
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* XPText;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelText;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* KillsText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* StatusContainer;

	/** Cooldown progress bars **/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* SpecialAtkBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* DefenseBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* UltimateBar;

	/** Determines if imbue element tag has triggered **/
	void OnTagChanged(const FGameplayTag Tag, int32 NewCount);

	/** Notifies widget blueprint a cooldown is triggered **/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FCooldownTriggered CooldownTriggered;

	/* References ability component */
	UAlbriteAbilitySystemComponent* AbilitySystemComponent;

	/** Binding functions **/
	void BindPlayerStats(AAlbritePlayerState* PlayerState);
	UFUNCTION()
	void UpdateLevelText(int32 AddedLevel);
	UFUNCTION()
	void UpdateXPText(int32 AddedXP);
	UFUNCTION()
	void UpdateKillsText(int32 AddedKills);
};