// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/AlbritePlayerController.h"
#include "ProjAlbriteGameMode.generated.h"

UCLASS(minimalapi)
class AProjAlbriteGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjAlbriteGameMode();

	UPROPERTY(BlueprintReadWrite)
	TArray<AAlbritePlayerController*> AllControllers;
protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostLogin(APlayerController* NewPlayerController) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThisDedicatedServer() {
		return GetWorld()->GetNetMode() == NM_DedicatedServer;
	}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThisListenServer() {
		return GetWorld()->GetNetMode() == NM_ListenServer;
	}
	
public:
	
	/** Character class that will be spawned for players */
	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	TSubclassOf<ACharacter> LobbyCharacterClass;

	/** Simulate Host */
	void HostGame();
	
	/** Handle client spawn request */
	void RequestSpawn(AAlbritePlayerController* PlayerController);
	void SpawnCharacter(AAlbritePlayerController* PlayerController);

	/** Handle server spawn logic */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRequestSpawn(AAlbritePlayerController* PlayerController);
	void ServerRequestSpawn_Implementation(AAlbritePlayerController* PlayerController);
	bool ServerRequestSpawn_Validate();

	/** Makes auto spawning characters for local host (Debugging) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsAutoSpawn = false;
};



