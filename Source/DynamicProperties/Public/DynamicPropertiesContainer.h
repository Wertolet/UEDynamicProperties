// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DynamicProperty.h"
#include "DynamicPropertiesContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewPropertyAdded, FGameplayTag, PropertyTag, UDynamicProperty*, Property);

/**
 * Actor component that manages a collection of dynamic properties identified by gameplay tags
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DYNAMICPROPERTIES_API UDynamicPropertiesContainer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDynamicPropertiesContainer();

protected:
	/** Map of dynamic properties indexed by gameplay tags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Properties")
	TMap<FGameplayTag, UDynamicProperty*> DynamicProperties;

public:
	/** Event fired when a new property is added to the container */
	UPROPERTY(BlueprintAssignable, Category = "Dynamic Properties")
	FOnNewPropertyAdded NewPropertyAdded;

	/**
	 * Gets a property by its gameplay tag
	 * @param PropertyTag The gameplay tag identifying the property
	 * @return The dynamic property, or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Properties")
	UDynamicProperty* GetProperty(FGameplayTag PropertyTag);

	/**
	 * Gets a property by its gameplay tag, or creates it if it doesn't exist
	 * @param PropertyTag The gameplay tag identifying the property
	 * @param BaseValue The base value to use if creating a new property
	 * @return The dynamic property (existing or newly created)
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Properties")
	UDynamicProperty* GetOrAddProperty(FGameplayTag PropertyTag, float BaseValue);

	/**
	 * Gets the value of a property, or returns a default value if the property doesn't exist
	 * @param PropertyTag The gameplay tag identifying the property
	 * @param DefaultValue The default value to return if the property doesn't exist
	 * @return The property's value, or the default value
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Properties")
	float GetPropertyValueOrDefault(FGameplayTag PropertyTag, float DefaultValue);

	/**
	 * Gets all property tags in the container
	 * @param OutKeys Array to be filled with all property tags
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Properties")
	void GetPropertiesKeys(TArray<FGameplayTag>& OutKeys);
};

