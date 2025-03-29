// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ExCal_DamageHealth.h"
#include "Stats/AlbriteAttributeSet.h"
#include "Subsystems/StatSubsystem.h"

struct CombatStatCapture
{
	// Declares the relevant variable that will be captured from attribute of a target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReceived);

	CombatStatCapture()
	{
		// Defines the relevant variable that will be captured from attribute of a target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlbriteAttributeSet, DamageReceived, Target, false);
	}
};

static CombatStatCapture& GetCombatStatCapture()
{
	static CombatStatCapture StatCapture;
	return StatCapture;
}

UExCal_DamageHealth::UExCal_DamageHealth()
{
	// Capture relevant variables here in constructor
	RelevantAttributesToCapture.Add(GetCombatStatCapture().HealthDef);
	RelevantAttributesToCapture.Add(GetCombatStatCapture().ShieldDef);
	RelevantAttributesToCapture.Add(GetCombatStatCapture().DamageReceivedDef);
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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatStatCapture().ShieldDef, EvalParams, CurrentShield);
	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatStatCapture().HealthDef, EvalParams, CurrentHealth);
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

	if (NewHealth <= 0)
	{
		// Get the Effect Context Handle
		FGameplayEffectContextHandle ContextHandle = ExecutionParams.GetOwningSpec().GetEffectContext();

		// Get the Source Actor
		AActor* SourceActor = Cast<AActor>(ContextHandle.GetOriginalInstigator());
		
		// ✅ Get the Target (Who Received the Effect)
		AActor* TargetActor = Cast<AActor>(ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor());
		
		if (SourceActor && TargetActor)
		{
			UWorld* World = SourceActor->GetWorld();
			UGameInstance* GameInstance = World->GetGameInstance();
			UStatSubsystem* MySubsystem = GameInstance->GetSubsystem<UStatSubsystem>();
			if(MySubsystem)
			{
				MySubsystem->KilledTarget.Broadcast(TargetActor, SourceActor);
			}
		}
	}

	// Override the Shield and Health
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCombatStatCapture().DamageReceivedProperty, EGameplayModOp::Override, IncomingDamage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCombatStatCapture().ShieldProperty, EGameplayModOp::Override, NewShield));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCombatStatCapture().HealthProperty, EGameplayModOp::Override, NewHealth));
}
