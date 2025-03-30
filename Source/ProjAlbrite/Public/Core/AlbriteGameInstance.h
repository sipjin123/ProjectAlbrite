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

	// Returns the array of characters
	UFUNCTION(BlueprintCallable, Category="Players")
	TArray<AProjAlbriteCharacter*> GetCharacters() const { return Characters.Array(); };

	// Registers the character to the TSet
	UFUNCTION(BlueprintCallable, Category="Players")
	void RegisterCharacter(AProjAlbriteCharacter* NewChar) { Characters.Add(NewChar); };

	// Joins the lobby of the host through server address
	UFUNCTION(Client, Reliable)
	void ClientJoinHostLobby(const FString& ServerAddress);

	// Returns the local IP
	UFUNCTION(BlueprintCallable)
	FString GetLocalIPAddress();

	// The current Hero the user selected in the loadout
	UPROPERTY(BlueprintReadWrite)
	ECharacterType HeroSelected;
};
