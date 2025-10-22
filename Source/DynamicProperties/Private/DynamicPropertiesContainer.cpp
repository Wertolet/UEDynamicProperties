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
		
		// Fire event
		NewPropertyAdded.Broadcast(PropertyTag, NewProperty);
	}

	return NewProperty;
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

