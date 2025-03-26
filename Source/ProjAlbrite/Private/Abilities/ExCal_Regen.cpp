// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ExCal_Regen.h"
#include "Stats/AlbriteAttributeSet.h"

struct DamageCapture
{
	// Declares the relevant variable that will be captured from attribute of a target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

	DamageCapture()
	{
		// Defines the relevant variable that will be captured from attribute of a target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, MaxHealth, Target, false);
	}
};

static DamageCapture& GetDamageCapture()
{
	static DamageCapture DamageCapture;
	return DamageCapture;
}

UExCal_Regen::UExCal_Regen()
{
	// Capture relevant variables here in constructor
	RelevantAttributesToCapture.Add(GetDamageCapture().HealthDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().MaxHealthDef);
}

void UExCal_Regen::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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
	float Health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().HealthDef, EvalParams, Health);
	float MaxHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().MaxHealthDef, EvalParams, MaxHealth);
	//-------------------------------------------------------------

	// Regen increases 5% health per interval
	float HealthToAdd = FMath::Clamp(MaxHealth *.05f,0.0f,MaxHealth);
	
	// Stop regen when health reaches Max Health
	if (Health >= MaxHealth)
	{
		UAbilitySystemComponent* ASC = ExecutionParams.GetSourceAbilitySystemComponent();
		if (!ASC) return;

		// Set gamelay effect with tags that needs to be removed
		FGameplayTagContainer TagsToRemove;
		TagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Status.Regen"));

		// MakeQuery_MatchAllEffectTags
		TArray<FActiveGameplayEffectHandle> ActiveEffects =
			ASC->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(TagsToRemove));
		
		// Remove Gameplay Effect
		for (const FActiveGameplayEffectHandle EffectHandle : ActiveEffects)
		{
			ASC->RemoveActiveGameplayEffect(EffectHandle);
		}
		return;
	}
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().HealthProperty, EGameplayModOp::Additive, HealthToAdd));
}
