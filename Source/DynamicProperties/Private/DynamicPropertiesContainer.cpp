// Copyright Epic Games, Inc. All Rights Reserved.

#include "DynamicPropertiesContainer.h"

UDynamicPropertiesContainer::UDynamicPropertiesContainer()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UDynamicProperty* UDynamicPropertiesContainer::GetProperty(FGameplayTag PropertyTag)
{
	if (UDynamicProperty** FoundProperty = DynamicProperties.Find(PropertyTag))
	{
		return *FoundProperty;
	}
	return nullptr;
}

UDynamicProperty* UDynamicPropertiesContainer::GetOrAddProperty(FGameplayTag PropertyTag, float BaseValue)
{
	// Check if property already exists
	if (UDynamicProperty** FoundProperty = DynamicProperties.Find(PropertyTag))
	{
		return *FoundProperty;
	}

	// Create new property
	UDynamicProperty* NewProperty = NewObject<UDynamicProperty>(this);
	if (NewProperty)
	{
		NewProperty->SetBaseValue(BaseValue);
		DynamicProperties.Add(PropertyTag, NewProperty);
		
		// Bind to the property's ValueChanged event
		BindPropertyValueChanged(PropertyTag, NewProperty);
		
		// Fire value changed event for the initial value (transition from non-existent to existent)
		OnPropertyValueChanged.Broadcast(PropertyTag, BaseValue, BaseValue);
	}

	return NewProperty;
}

void UDynamicPropertiesContainer::BindPropertyValueChanged(FGameplayTag PropertyTag, UDynamicProperty* Property)
{
	if (!Property)
	{
		return;
	}

	// Create a lambda that captures the PropertyTag and broadcasts the container's event
	Property->ValueChanged.AddLambda([this, PropertyTag](float OldValue, float NewValue)
	{
		OnPropertyValueChanged.Broadcast(PropertyTag, OldValue, NewValue);
	});
}

float UDynamicPropertiesContainer::GetPropertyValueOrDefault(FGameplayTag PropertyTag, float DefaultValue)
{
	if (UDynamicProperty** FoundProperty = DynamicProperties.Find(PropertyTag))
	{
		if (*FoundProperty)
		{
			return (*FoundProperty)->GetValue();
		}
	}
	return DefaultValue;
}

void UDynamicPropertiesContainer::GetPropertiesKeys(TArray<FGameplayTag>& OutKeys)
{
	DynamicProperties.GetKeys(OutKeys);
}

