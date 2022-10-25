// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ASSkeletalPartDefinition.generated.h"



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
 * Skeletal part definition
 * 
 * Defines a skeletal part Actor and how it is attached
 */
USTRUCT()
struct ANIMATIONSETUP_API FASSkeletalPartDefinition
{
	GENERATED_BODY()

	FASSkeletalPartDefinition()
		: PartActorClass(nullptr)
		, AttachmentTransformRules(FAttachmentTransformRules::SnapToTargetNotIncludingScale)
		, SocketName(NAME_None)
	{
	}

	/** Actor class to use */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> PartActorClass;

	/** Method of attachment to the owner component */
	UPROPERTY(EditDefaultsOnly)
		FASBlueprintAttachmentTransformRules AttachmentTransformRules;

	/** Socket to attach to */
	UPROPERTY(EditDefaultsOnly)
		FName SocketName;


	/**
	 * Spawn and attach the skeletal part Actor based on the part definition
	 */
	AActor* SpawnAndAttachPartActor(AActor* InOwner, USceneComponent* InComponentToAttachTo) const;
};
