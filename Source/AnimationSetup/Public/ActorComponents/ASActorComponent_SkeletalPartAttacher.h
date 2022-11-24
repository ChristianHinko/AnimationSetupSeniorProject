// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/ASSkeletalPartDefinition.h"

#include "ASActorComponent_SkeletalPartAttacher.generated.h"


struct FASSkeletalPartDefinition;



/**
 * Makes a Skeletal Mesh Component the invisible driver for attachable skeletal parts.
 */
UCLASS(ClassGroup=(AnimationSetup))
class ANIMATIONSETUP_API UASActorComponent_SkeletalPartAttacher : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Skeletal Mesh made up of Skeleton data only */
	UPROPERTY(Transient)
		TObjectPtr<USkeletalMesh> SkinlessSkeletalMesh;

	/** Spawned part Actors attached to the Skeletal Mesh Component */
	UPROPERTY(Transient)
		TArray<TObjectPtr<AActor>> SkeletalPartActors;

public:
	UASActorComponent_SkeletalPartAttacher(const FObjectInitializer& ObjectInitializer);


	/**
	 * Make the given Skeletal Mesh Component the invisible driver for the attached part Actors.
	 * 
	 * Recomended callsite: in your constructor.
	 */
	virtual void UseSkeletalMeshComponent(USkeletalMeshComponent* InSkeletalMeshComponent);

	/**
	 * Spawn the skeletal part Actors and attach them to the Skeletal Mesh Component.
	 * 
	 * Recomended callsite: after the Super in your PostRegisterAllComponents() function.
	 */
	virtual void SpawnSkeletalPartActors();

protected:
	//  BEGIN UObject interface
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	//  END UObject interface

	//  BEGIN UActorComponent interface
	virtual void OnUnregister() override;
	//  END UActorComponent interface


	/** The Skeleton to use for driving our attached part Actors */
	UPROPERTY(EditDefaultsOnly, Category = "SkeletalParts")
		TObjectPtr<USkeleton> Skeleton;

	/** The Skeletal Mesh Component that we are using as an invisible skeleton with animation (using the SkinlessSkeletalMesh) */
	UPROPERTY(Transient)
		TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	/** Definitions of skeletal part Actors and how they are attached */
	UPROPERTY(EditDefaultsOnly, Category = "SkeletalParts")
		TArray<FASSkeletalPartDefinition> SkeletalPartDefinitions;


	void CreateSkinlessSkeletalMesh();
};
