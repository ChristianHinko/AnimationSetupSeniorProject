// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ASActorComponent_PortrayalAssignment.generated.h"


class UASPortrayalDefinition;



/**
 * Determines how the actor component will be portrayed.
 */
USTRUCT()
struct ANIMATIONSETUP_API FASPortrayedActorComponent
{
	GENERATED_BODY()

	/** The actor component to portray. */
	UPROPERTY(EditAnywhere)
		FComponentReference ActorComponent;

	/** How to portray this component. */
	UPROPERTY(EditAnywhere)
		FGameplayTag PortrayalTag;
};

/**
 * This is a single representation of several differently-portrayed actor components.
 */
USTRUCT()
struct ANIMATIONSETUP_API FASActorComponentDepiction
{
	GENERATED_BODY()

	/** The different portrayed components that make up this completed representation. */
	UPROPERTY(EditAnywhere)
		TArray<FASPortrayedActorComponent> PortrayedActorComponents;
};

/**
 * Holds actor component depictions and applies their portrayals when registered.
 */
UCLASS(ClassGroup="AnimationSetup", meta=(BlueprintSpawnableComponent))
class ANIMATIONSETUP_API UASActorComponent_PortrayalAssignment : public UActorComponent
{
	GENERATED_BODY()

public:
	UASActorComponent_PortrayalAssignment(const FObjectInitializer& ObjectInitializer);

	/**
	 * List of actor component depictions.
	 */
	UPROPERTY(EditDefaultsOnly)
		TArray<FASActorComponentDepiction> ActorComponentDepictions;


	UFUNCTION(BlueprintCallable)
		void ApplyPortrayals();

	UFUNCTION(BlueprintCallable)
		void UnapplyPortrayals();


	UFUNCTION(BlueprintPure)
		UASPortrayalDefinition* GetInstancedPortrayalDefinition(const FGameplayTag& InPortrayalTag) const;

protected:
	//  BEGIN UActorComponent interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	//  END UActorComponent interface

	/** Holder of instantiated portrayal definitions. */
	UPROPERTY()
		TMap<FGameplayTag, TObjectPtr<UASPortrayalDefinition>> InstancedPortrayalDefinitions;

	void CreateInstancedPortrayalDefinitions();
	void DestoryInstancedPortrayalDefinitions();
};
