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
		
		// Call virtual hook for derived classes (also fires initial OnPropertyValueChanged)
		OnPropertyAddedInternal(PropertyTag, NewProperty);
	}

	return NewProperty;
}

void UDynamicPropertiesContainer::BindPropertyValueChanged(FGameplayTag PropertyTag, UDynamicProperty* Property)
{
	if (!Property)
	{
		return;
	}

	// Create a binder object that will forward the event with the property tag
	UPropertyValueChangedBinder* Binder = NewObject<UPropertyValueChangedBinder>(this);
	if (Binder)
	{
		// Bind the binder to the property
		Binder->BindToProperty(Property, PropertyTag);
		
		// Store the binder to prevent garbage collection
		ValueChangedBinders.Add(Binder);

		// Bind our handler to the binder's event
		Binder->OnBinderValueChanged.AddDynamic(this, &UDynamicPropertiesContainer::HandleBinderValueChanged);
	}
}

void UDynamicPropertiesContainer::HandleBinderValueChanged(FGameplayTag PropertyTag, float OldValue, float NewValue)
{
	// Call virtual hook for derived classes (also broadcasts OnPropertyValueChanged)
	OnPropertyValueChangedInternal(PropertyTag, OldValue, NewValue);
}

void UDynamicPropertiesContainer::OnPropertyValueChangedInternal(FGameplayTag PropertyTag, float OldValue, float NewValue)
{
	// Base implementation broadcasts the event
	OnPropertyValueChanged.Broadcast(PropertyTag, OldValue, NewValue);
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

void UDynamicPropertiesContainer::OnPropertyAddedInternal(FGameplayTag PropertyTag, UDynamicProperty* Property)
{
	if (!Property)
	{
		return;
	}

	// Fire value changed event for the initial value (transition from non-existent to existent)
	float InitialValue = Property->GetValue();
	OnPropertyValueChanged.Broadcast(PropertyTag, InitialValue, InitialValue);
}

