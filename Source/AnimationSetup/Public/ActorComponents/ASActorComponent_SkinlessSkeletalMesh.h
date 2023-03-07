// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ASActorComponent_SkinlessSkeletalMesh.generated.h"



/**
 * Makes a specified Skeleton animatable through a given Skeletal Mesh Component.
 */
UCLASS(ClassGroup=(AnimationSetup))
class ANIMATIONSETUP_API UASActorComponent_SkinlessSkeletalMesh : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Skeletal Mesh made up of Skeleton data only */
	UPROPERTY(Transient)
		TObjectPtr<USkeletalMesh> SkinlessSkeletalMesh;

public:
	UASActorComponent_SkinlessSkeletalMesh(const FObjectInitializer& ObjectInitializer);


	/**
	 * Reference to the Skeletal Mesh Component to make into an invisible and animatable skeleton. Assign
	 * the ComponentProperty to component's member name. E.g., "Mesh" for ACharacter::Mesh. Note that
	 * this is not "CharacterMesh0" - as that would be the name of the object and not the object pointer member.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "SkinlessSkeletalMesh") // NOTE: We are not using the UseComponentPicker meta because it does not make sense to pick a Component out of a World when all we want is a reference to the Component member by name. The UseComponentPicker meta typically makes sense for EditInstanceOnly, in which case there would be an appropriate World to choose from because there is an instance of our owner Actor in that World.
		FComponentReference SkeletalMeshComponentReference;

protected:
	//  BEGIN UObject interface
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	//  END UObject interface


	/** The Skeleton to use and make animatable for the SkinlessSkeletalMesh. */
	UPROPERTY(EditDefaultsOnly, Category = "SkinlessSkeletalMesh")
		TObjectPtr<USkeleton> Skeleton;

	/** If true, include the mesh as a primitive in the render scene. This is not necessary since we are skinless. */
	UPROPERTY(EditDefaultsOnly, Category = "SkinlessSkeletalMesh")
		uint8 bAddToRendererScene : 1;

	/**
	 * Cached reference to the component from SkeletalMeshComponentReference.
	 * The Skeletal Mesh Component that we are using as an invisible skeleton with animation (using the SkinlessSkeletalMesh).
	 */
	UPROPERTY(Transient)
		TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;


	void CreateSkinlessSkeletalMesh();
};
