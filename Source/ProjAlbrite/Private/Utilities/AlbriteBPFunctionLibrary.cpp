// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/AlbriteBPFunctionLibrary.h"
#include "AI/BaseAICharacter.h"
#include "Engine/OverlapResult.h"
#include "Enums/FactionType.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjAlbrite/ProjAlbriteCharacter.h"

UAlbriteBPFunctionLibrary::UAlbriteBPFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

TArray<AActor*> UAlbriteBPFunctionLibrary::GetActorsWithinRadius(FVector Origin, float Radius,
	UObject* WorldContextObject, EFactionType FactionType, bool ShowDebug)
{
	const UWorld * world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	TArray<AActor*> OutActors;
	const FCollisionShape DetectionSphere = FCollisionShape::MakeSphere(Radius);

	// Define object types to detect (only detect Pawns, like enemies)
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	// Store detected actors
	TArray<FOverlapResult> OverlapResults;

	// Perform Overlap
	const bool bHasOverlap = world->OverlapMultiByObjectType(
		OverlapResults, 
		Origin, 
		FQuat::Identity, 
		ObjectQueryParams, 
		DetectionSphere
	);

	// Draw debug sphere (DEBUG)
	if(ShowDebug)
	{
		DrawDebugSphere(world, Origin, Radius, 16, FColor::Green, false, 2.0f);
	}
	
	// Use a set to track unique actors
	TSet<AActor*> UnitsHit;
	
	// Process detected actors
	if (bHasOverlap)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* DetectedActor = Result.GetActor();

			// Filter actors here
			bool IsValidActorHit =
					FactionType == EFactionType::Heroes && DetectedActor->IsA(AProjAlbriteCharacter::StaticClass()) || 
					FactionType == EFactionType::Enemies && DetectedActor->IsA(ABaseAICharacter::StaticClass()) ||
					FactionType == EFactionType::None && DetectedActor->IsA(ACharacter::StaticClass());

			// Check validity and prevent duplicates
			if (IsValidActorHit && !UnitsHit.Contains(DetectedActor))
			{
				if(ShowDebug)
				{
					// Log Collision (DEBUG)
					UE_LOG(LogTemp, Warning, TEXT("Enemy detected: %s"), *DetectedActor->GetName());
				}
				UnitsHit.Add(DetectedActor);
			}
		}
	}

	return UnitsHit.Array();
}
