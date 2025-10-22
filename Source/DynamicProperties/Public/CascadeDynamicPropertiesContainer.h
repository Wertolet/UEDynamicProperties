// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicPropertiesContainer.h"
#include "CascadeDynamicPropertiesContainer.generated.h"

/**
 * Actor component that manages dynamic properties with cascading/hierarchical tag relationships
 * When a property changes, it updates the base values of all direct child properties
 * For example: changing "A" will update base value of "A.B", which then cascades to "A.B.C"
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DYNAMICPROPERTIES_API UCascadeDynamicPropertiesContainer : public UDynamicPropertiesContainer
{
	GENERATED_BODY()

public:
	UCascadeDynamicPropertiesContainer();

	/**
	 * Gets the value of a property with cascade calculation
	 * Walks up the tag hierarchy to find parent properties and uses their values as base
	 * @param PropertyTag The gameplay tag identifying the property
	 * @param DefaultValue The default value to return if no properties exist in the hierarchy
	 * @return The calculated cascaded value
	 */
	virtual float GetPropertyValueOrDefault(FGameplayTag PropertyTag, float DefaultValue) override;

protected:
	/**
	 * Override to add cascade handling when properties are added
	 */
	virtual void OnPropertyAddedInternal(FGameplayTag PropertyTag, UDynamicProperty* Property) override;

	/**
	 * Override to add cascade handling when property values change
	 */
	virtual void OnPropertyValueChangedInternal(FGameplayTag PropertyTag, float OldValue, float NewValue) override;

private:

	/**
	 * Updates the base value of all direct child properties when a parent changes
	 * @param ParentTag The parent tag that changed
	 * @param NewParentValue The new value of the parent
	 */
	void UpdateChildPropertiesBaseValue(FGameplayTag ParentTag, float NewParentValue);

	/**
	 * Finds the nearest parent property in the hierarchy
	 * @param ChildTag The tag to find parent for
	 * @return The nearest parent property, or nullptr if none found
	 */
	UDynamicProperty* FindNearestParentProperty(FGameplayTag ChildTag);

	/**
	 * Gets all direct child tags of a parent tag from the properties map
	 * @param ParentTag The parent tag
	 * @param OutChildTags Array to fill with direct child tags
	 */
	void GetDirectChildTags(FGameplayTag ParentTag, TArray<FGameplayTag>& OutChildTags);

	/**
	 * Counts the depth of a gameplay tag (number of dots + 1)
	 * @param Tag The tag to measure
	 * @return The depth of the tag
	 */
	static int32 GetTagDepth(const FString& TagString);

	/**
	 * Gets all non-direct children that don't have intermediate nodes in the hierarchy
	 * @param ParentTag The parent tag
	 * @param OutChildTags Array to fill with non-direct child tags without intermediate nodes
	 */
	void GetChildrenToUpdate(FGameplayTag ParentTag, TArray<FGameplayTag>& OutChildTags);
};

