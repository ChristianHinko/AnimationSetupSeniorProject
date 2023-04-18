// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ASPortrayalDefinition.generated.h"



/**
 * Defines how to view our portrayed actor components and other portrayed actor components.
 */
UCLASS(Abstract, Const, Blueprintable)
class ANIMATIONSETUP_API UASPortrayalDefinition : public UObject
{
	GENERATED_BODY()

public:
	UASPortrayalDefinition();


	/**
	 * Whether or not to instantiate a unique portrayal definition per actor.
	 * Necessary for per-actor portrayal definition configuring.
	 */
	UPROPERTY(EditDefaultsOnly)
		uint8 bInstanced : 1;


	/**
	 * Set up the portraying of an actor component affiliated with this portrayal.
	 * 
	 * This is good for including viewers in seeing the portrayed component.
	 * 
	 * This is a place to configure the Primitive Component to be shown only under
	 * certain conditions, e.g., doing UPrimitiveComponent::SetOnlyOwnerSee() with true.
	 */
	UFUNCTION(BlueprintNativeEvent)
		void ApplyToActorComponent(UActorComponent* InActorComponent) const;

	/** Do the reverse of applying. */
	UFUNCTION(BlueprintNativeEvent)
		void UnapplyFromActorComponent(UActorComponent* InActorComponent) const;

	/**
	 * Set up the portraying of an actor component affiliated with a different portrayal.
	 * 
	 * This is good for excluding viewers from seeing other portrayed components.
	 * 
	 * This is a place to configure the Primitive Component to be hidden under
	 * certain conditions, e.g., doing UPrimitiveComponent::SetOnlyOwnerSee() with false.
	 */
	UFUNCTION(BlueprintNativeEvent)
		void ApplyToActorComponentOfDifferentPortrayal(UActorComponent* InActorComponent) const;

	/** Do the reverse of applying. */
	UFUNCTION(BlueprintNativeEvent)
		void UnapplyFromActorComponentOfDifferentPortrayal(UActorComponent* InActorComponent) const;


	virtual void ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const;
	virtual void UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const;
	virtual void ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const;
	virtual void UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const;
};
