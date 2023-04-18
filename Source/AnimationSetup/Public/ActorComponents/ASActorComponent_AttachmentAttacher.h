// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/ASAttachmentDefinition.h"

#include "ASActorComponent_AttachmentAttacher.generated.h"


struct FASAttachmentDefinition;



/**
 * Spawns and attaches the attachments defined to a specified attachee.
 */
UCLASS(ClassGroup=(AnimationSetup))
class ANIMATIONSETUP_API UASActorComponent_AttachmentAttacher : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Spawned attachment Actors attached to the attachee. */
	UPROPERTY(Transient)
		TArray<TObjectPtr<AActor>> Attachments;

public:
	UASActorComponent_AttachmentAttacher(const FObjectInitializer& ObjectInitializer);


	/**
	 * Specify a component to be the attachee of attached attachments.
	 */
	UPROPERTY(EditDefaultsOnly, Category="AttachmentAttacher")
		FComponentReference AttacheeReference;

	/**
	 * Spawn the attachments and attach them to the specified attachee.
	 * 
	 * Recomended callsite: after the Super in your PostRegisterAllComponents() function.
	 */
	UFUNCTION(BlueprintCallable)
		virtual void SpawnAttachments();

	/** Destroy all spawned attachments. */
	UFUNCTION(BlueprintCallable)
		virtual void DestroyAttachments();

	/** Get all spawned attachments */
	UFUNCTION(BlueprintPure)
		UPARAM(Ref, Const) const TArray<TObjectPtr<AActor>>& GetAttachments() const { return Attachments; }

protected:
	//  BEGIN UObject interface
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	//  END UObject interface
	//  BEGIN UActorComponent interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	//  END UActorComponent interface


	/**
	 * Cached reference to the component from AttacheeReference.
	 * The component that the spawned attachments will attach to.
	 */
	UPROPERTY(Transient)
		TWeakObjectPtr<USceneComponent> Attachee;

	/** Definitions of attachments. */
	UPROPERTY(EditDefaultsOnly, Category="AttachmentAttacher")
		TArray<FASAttachmentDefinition> AttachmentDefinitions;
};
