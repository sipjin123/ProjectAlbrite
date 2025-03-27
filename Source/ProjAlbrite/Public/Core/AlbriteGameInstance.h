// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjAlbrite/ProjAlbriteCharacter.h"
#include "AlbriteGameInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJALBRITE_API UAlbriteGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Cached reference to all Characters
	UPROPERTY(BlueprintReadWrite, Category="Players")
	TSet<AProjAlbriteCharacter*> Characters;
	
	UFUNCTION(BlueprintCallable, Category="Players")
	TArray<AProjAlbriteCharacter*> GetCharacters() const { return Characters.Array(); };

	UFUNCTION(BlueprintCallable, Category="Players")
	void RegisterCharacter(AProjAlbriteCharacter* NewChar) { Characters.Add(NewChar); };
};
