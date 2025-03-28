// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAlbriteAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAlbriteAttributeSet>(TEXT("Attributes"));
	
	// Create and attach the widget component
	CombatWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatWidget"));
	CombatWidgetComponent->SetupAttachment(RootComponent); // Attach to the character
	CombatWidgetComponent->SetWidgetSpace(EWidgetSpace::World); // Set to World Space
	CombatWidgetComponent->SetDrawSize(FVector2D(100, 10)); // Set UI size
	CombatWidgetComponent->SetRelativeLocation(FVector(0, 0, 100)); // Position above head
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent))
	{
		AttributeSet = AbilitySystemComponent->GetSet<UAlbriteAttributeSet>();
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAlbriteAttributeSet::GetHealthAttribute()).AddUObject(this, &ABaseAICharacter::OnHealthUpdated);
		
		AttributeSet = AbilitySystemComponent->GetSet<UAlbriteAttributeSet>();
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAlbriteAttributeSet::GetShieldAttribute()).AddUObject(this, &ABaseAICharacter::OnShieldUpdated);

		AbilitySystemComponent->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Status.Invulnerable")),
			EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &ABaseAICharacter::OnInvulnerableTagChanged);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Status.Stun")),
			EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &ABaseAICharacter::OnStunTagChanged);
	}

	CombatWidget = Cast<UCombatUnitWidget>(CombatWidgetComponent->GetWidget());
}

// Called every frame
void ABaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseAICharacter::OnHealthUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnHealthChange.Broadcast(OnAttributeChangeData.NewValue);
}

void ABaseAICharacter::OnShieldUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnShieldChange.Broadcast(OnAttributeChangeData.NewValue);
}

void ABaseAICharacter::OnInvulnerableTagChanged(FGameplayTag GameplayTag, int NewVal) const
{
	OnInvulnerableChange.Broadcast(NewVal == 0 ? false : true);
}

void ABaseAICharacter::OnStunTagChanged(FGameplayTag GameplayTag, int NewVal) const
{
	OnStunChange.Broadcast(NewVal == 0 ? false : true);
}
