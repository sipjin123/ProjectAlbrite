// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/IAlbriteCharacter.h"
#include "Interfaces/IDamageable.h"
#include "ActorComponents/AlbriteAbilitySystemComponent.h"
#include "ProjAlbrite/ProjAlbriteCharacter.h"
#include "Stats/AlbriteAttributeSet.h"
#include "BaseAICharacter.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInvulnerableChange, bool, InvulnerableValue);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStunChange, bool, StunValue);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChange, int, AttributeValue);
UCLASS()
class PROJALBRITE_API ABaseAICharacter : public ACharacter, public IAbilitySystemInterface, public IIDamageable, public IIAlbriteCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Gameplay Ability System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess="true"))
	class UAlbriteAbilitySystemComponent* AbilitySystemComponent;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	
	/** Gameplay Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess="true"))
	const class UAlbriteAttributeSet* AttributeSet;
	
	/** Retrieves The base Attribute for Health **/
	virtual float OnGetCurrentHealth_Implementation() override { return AttributeSet ? AttributeSet->Health.GetBaseValue() : 0.1f; };
	
	/** Retrieves The base Attribute for Max Health **/
	virtual float OnGetMaxHealth_Implementation() override { return AttributeSet ? AttributeSet->MaxHealth.GetBaseValue() : 0.1f; };

	/** The determines if the unit is dead **/
	UPROPERTY(BlueprintReadWrite, Replicated, ReplicatedUsing = OnRep_IsDead, Category="Combat")
	bool IsDead;
	UFUNCTION()
	void OnRep_IsDead();
	
	/** This callback can be used by the UI. **/
	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnHealthChange;
	void OnHealthUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const;

	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnShieldChange;
	void OnShieldUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Stat callbacks")
	FInvulnerableChange OnInvulnerableChange;
	
	UPROPERTY(BlueprintAssignable, Category = "Stat callbacks")
	FStunChange OnStunChange;
	
	void OnInvulnerableTagChanged(FGameplayTag GameplayTag, int NewVal) const;
	void OnStunTagChanged(FGameplayTag GameplayTag, int NewVal) const;

	virtual bool IsStunned_Implementation() override { return AbilitySystemComponent ? AbilitySystemComponent->ComponentHasTag(FName("Status.Stun")) : false; };
	virtual bool IsInvulnerable_Implementation () override { return AbilitySystemComponent ? AbilitySystemComponent->ComponentHasTag(FName("Status.Invulnerable")) : false; };
	virtual bool IsUnitDead_Implementation() override { return AttributeSet ? AttributeSet->Health.GetBaseValue() <= 0 : false; };
	
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UCombatUnitWidget* CombatWidget;
	
	/** Widget Component for displaying UI above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* CombatWidgetComponent;

	virtual bool IsEnemyUnit_Implementation() override { return true; };
};
