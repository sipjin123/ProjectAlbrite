// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJALBRITE_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void HostButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void LoginButtonPressed(int32 CharacterChoice);
	
	UFUNCTION(BlueprintCallable)
	void SpawnPlayerPressed(int32 CharacterChoice);
};
