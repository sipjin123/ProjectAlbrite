// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjAlbrite/ProjAlbriteCharacter.h"
#include "AlbritePlayerController.generated.h"

/**
 * 
 */

class APlayerSpawnPoint;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessAcknowledge);
UCLASS()
class PROJALBRITE_API AAlbritePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void AcknowledgePossession(APawn* P) override;
	
private:
	UPROPERTY()
	UUserWidget* CursorWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CursorWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FVector Offset;

public:
	AAlbritePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Client avatar spawn inquiry **/
	UFUNCTION(Client, Reliable)
	void Client_CanSpawnAvatar();

	/** Server avatar spawn **/
	UFUNCTION(Server, Reliable)
	void Server_ReqSpawnAvatar(const int32 SpawnPosIndex, const int32 AvatarIndex, const FString& ID);

	/** Reference to the character **/
	UPROPERTY(BlueprintReadWrite, Category="References")
	AActor* AlbriteCharacter;

	/** Target character reference **/
	UPROPERTY(BlueprintReadWrite, Category="References")
	AActor* CurrentTargetActor;

	/** Broadcast to blueprint the possession **/
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnPossessAcknowledge OnPossessAcknowledge;

	/** Cache Spawned Characters **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, TSubclassOf<AProjAlbriteCharacter>> SpawnCharacterPair;
	
	/** Spawn point class reference **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnPointClass = nullptr;

	/** Cache all Spawn point reference **/
	TArray<AActor*> AllSpawnPoints;

	/** Lobby Join  */
	UFUNCTION(Client, Reliable)
	void Client_JoinLobby(const FString& ServerAddress);
	void Client_JoinLobby_Implementation(const FString& ServerAddress);

	/** Server Spawning **/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestSpawn();
	void ServerRequestSpawn_Implementation();
	bool ServerRequestSpawn_Validate();

	/** Join with character Spawning **/
	UFUNCTION(Client, Reliable)
	void Client_JoinAsCharacter();
	void Client_JoinAsCharacter_Implementation();
};