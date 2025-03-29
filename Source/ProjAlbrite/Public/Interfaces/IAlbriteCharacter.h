// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAlbriteCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIAlbriteCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJALBRITE_API IIAlbriteCharacter
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	UCharacterMovementComponent* GetCharacterMovementComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	ECharacterType GetCharacterType();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void PurgeElementImbue();
};