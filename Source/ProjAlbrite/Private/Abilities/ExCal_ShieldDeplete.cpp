// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ExCal_ShieldDeplete.h"
#include "Stats/AlbriteAttributeSet.h"

struct StatCapture
{
	// Declares the relevant variable that will be captured from attribute of a target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

	StatCapture()
	{
		// Defines the relevant variable that will be captured from attribute of a target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, Shield, Target, false);
	}
};

static StatCapture& GetStatCapture()
{
	static StatCapture StatCapture;
	return StatCapture;
}

UExCal_ShieldDeplete::UExCal_ShieldDeplete()
{
	// Capture relevant variables here in constructor
	RelevantAttributesToCapture.Add(GetStatCapture().ShieldDef);
}

void UExCal_ShieldDeplete::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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
	float Shield = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetStatCapture().ShieldDef, EvalParams, Shield);
	//-------------------------------------------------------------

	// Regen increases 5% health per interval
	float ShieldToAdd = -2.f;
	
	// Stop regen when health reaches Max Health
	if (Shield <= 0)
	{
		UAbilitySystemComponent* ASC = ExecutionParams.GetSourceAbilitySystemComponent();
		if (!ASC) return;

		// Set gamelay effect with tags that needs to be removed
		FGameplayTagContainer TagsToRemove;
		TagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Status.Shield"));

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
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetStatCapture().ShieldProperty, EGameplayModOp::Additive, ShieldToAdd));
}
