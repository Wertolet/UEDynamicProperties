// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "PropertyValueChangedBinder.generated.h"

// Forward declaration
class UDynamicProperty;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBinderValueChanged, FGameplayTag, PropertyTag, float, OldValue, float, NewValue);

/**
 * Helper class to bind property value changed events with gameplay tag context
 */
UCLASS()
class DYNAMICPROPERTIES_API UPropertyValueChangedBinder : public UObject
{
	GENERATED_BODY()

public:
	/** The gameplay tag for the property this binder is associated with */
	FGameplayTag PropertyTag;

	/** Event fired when the bound property's value changes */
	UPROPERTY(BlueprintAssignable)
	FOnBinderValueChanged OnBinderValueChanged;

	/**
	 * Binds this binder to a dynamic property
	 * Can only be called once. Subsequent calls will be ignored.
	 * @param Property The property to bind to
	 * @param Tag The gameplay tag to associate with this property
	 */
	void BindToProperty(UDynamicProperty* Property, FGameplayTag Tag);

private:
	/** Flag to track if this binder has already been bound to a property */
	bool bIsBound = false;

	/**
	 * Called when a property's value changes - forwards to OnBinderValueChanged with PropertyTag
	 */
	UFUNCTION()
	void OnValueChanged(float OldValue, float NewValue);
};

