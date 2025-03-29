// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AlbriteAttributeSet.generated.h"

#define ATTRIBUTE_ASSESSORS(ClassName,PropertyName)\
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)
/**
 * The primary attribute data class that is used to manipulate character stats using GAS
 * Author: Burlin
 */
UCLASS(BlueprintType)
class PROJALBRITE_API UAlbriteAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	
	UAlbriteAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing = OnRep_DamageReceived)
	FGameplayAttributeData DamageReceived;
	ATTRIBUTE_ASSESSORS(UAlbriteAttributeSet, DamageReceived);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ASSESSORS(UAlbriteAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ASSESSORS(UAlbriteAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ASSESSORS(UAlbriteAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ASSESSORS(UAlbriteAttributeSet, Damage);

public:
	
	UFUNCTION(BlueprintCallable)
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION(BlueprintCallable)
	void OnRep_MaxHealth(const FGameplayAttributeData& OldHMaxealth);
	
	UFUNCTION(BlueprintCallable)
	void OnRep_Damage(const FGameplayAttributeData& OldDamage);
	
	UFUNCTION(BlueprintCallable)
	void OnRep_Shield(const FGameplayAttributeData& OldShield);
	
	UFUNCTION(BlueprintCallable)
	void OnRep_DamageReceived(const FGameplayAttributeData& OldDamageReceived);
};
