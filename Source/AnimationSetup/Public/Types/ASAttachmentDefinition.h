// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ASAttachmentDefinition.generated.h"



/**
 * Blueprint type version of Engine's FAttachmentTransformRules
 */
USTRUCT(BlueprintType)
struct ANIMATIONSETUP_API FASBlueprintAttachmentTransformRules
{
	GENERATED_BODY()

	FASBlueprintAttachmentTransformRules() // default constructor required by USTRUCT
		: LocationRule(EAttachmentRule())
		, RotationRule(EAttachmentRule())
		, ScaleRule(EAttachmentRule())
		, bWeldSimulatedBodies(false)
	{
	}
	FASBlueprintAttachmentTransformRules(const FAttachmentTransformRules& InAttachmentTransformRules)
		: LocationRule(InAttachmentTransformRules.LocationRule)
		, RotationRule(InAttachmentTransformRules.RotationRule)
		, ScaleRule(InAttachmentTransformRules.ScaleRule)
		, bWeldSimulatedBodies(InAttachmentTransformRules.bWeldSimulatedBodies)
	{
	}


	UPROPERTY(EditAnywhere)
		EAttachmentRule LocationRule;
	UPROPERTY(EditAnywhere)
		EAttachmentRule RotationRule;
	UPROPERTY(EditAnywhere)
		EAttachmentRule ScaleRule;

	UPROPERTY(EditAnywhere)
		bool bWeldSimulatedBodies;


	// Implicit conversion to the native type
	operator FAttachmentTransformRules() const
	{
		return FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, bWeldSimulatedBodies);
	}
};

/**
 * Attachment definition
 * 
 * Defines an attachment - the Actor to spawn how it attaches
 */
USTRUCT()
struct ANIMATIONSETUP_API FASAttachmentDefinition
{
	GENERATED_BODY()

	FASAttachmentDefinition()
		: ActorClass(nullptr)
		, AttachmentTransformRules(FAttachmentTransformRules::SnapToTargetNotIncludingScale)
		, SocketName(NAME_None)
	{
	}

	/** Actor class to use */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> ActorClass;

	/** Method of attachment to the owner component */
	UPROPERTY(EditDefaultsOnly)
		FASBlueprintAttachmentTransformRules AttachmentTransformRules;

	/** Socket to attach to */
	UPROPERTY(EditDefaultsOnly)
		FName SocketName;


	/**
	 * Spawn and attach the attachment based on the definition
	 */
	AActor* SpawnAndAttach(AActor* InOwner, USceneComponent* InAttachee) const;
};
