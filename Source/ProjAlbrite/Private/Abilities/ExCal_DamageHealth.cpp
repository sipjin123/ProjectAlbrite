// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ExCal_DamageHealth.h"
#include "Stats/AlbriteAttributeSet.h"

struct StatCapture
{
	// Declares the relevant variable that will be captured from attribute of a target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

	StatCapture()
	{
		// Defines the relevant variable that will be captured from attribute of a target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, Shield, Target, false);
	}
};

static StatCapture& GetStatCapture()
{
	static StatCapture StatCapture;
	return StatCapture;
}

UExCal_DamageHealth::UExCal_DamageHealth()
{
	// Capture relevant variables here in constructor
	RelevantAttributesToCapture.Add(GetStatCapture().HealthDef);
	RelevantAttributesToCapture.Add(GetStatCapture().ShieldDef);
}

void UExCal_DamageHealth::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                          FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Handle Tags
	const FGameplayEffectSpec& GESpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = GESpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GESpec.CapturedTargetTags.GetAggregatedTags();

	// Generate Parameters using tags fetched
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	// Capture relevant attributes
	float CurrentShield = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetStatCapture().ShieldDef, EvalParams, CurrentShield);
	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetStatCapture().HealthDef, EvalParams, CurrentHealth);
	//-------------------------------------------------------------

	// Retrieve the SetByCaller Magnitude for Damage
	float IncomingDamage = GESpec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Combat.Damage")), // Must match GE tag
		false,
		0.0f // Default value if not found
	);

	// Capture damage value set in the GE
	IncomingDamage = FMath::Max(0.0f, IncomingDamage); // Ensure damage is non-negative

	// Setup Shield and Incoming Damage
	float RemainingDamage = IncomingDamage;
	float NewShield = FMath::Max(0.0f, CurrentShield - RemainingDamage);

	// Reduce damage by absorbed shield
	RemainingDamage -= (CurrentShield - NewShield);
	RemainingDamage = FMath::Max(0.0f, RemainingDamage);

	// Compute final health after shield is broken
	float NewHealth = FMath::Max(0.0f, CurrentHealth - RemainingDamage);

	// Override the Shield and Health
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetStatCapture().ShieldProperty, EGameplayModOp::Override, NewShield));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetStatCapture().HealthProperty, EGameplayModOp::Override, NewHealth));
}
