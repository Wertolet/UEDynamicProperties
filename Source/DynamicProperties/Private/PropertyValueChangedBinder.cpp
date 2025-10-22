// Copyright Epic Games, Inc. All Rights Reserved.

#include "PropertyValueChangedBinder.h"
#include "DynamicProperty.h"

void UPropertyValueChangedBinder::BindToProperty(UDynamicProperty* Property, FGameplayTag Tag)
{
	// Ensure we can only bind once
	if (bIsBound)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPropertyValueChangedBinder::BindToProperty - Attempted to bind already bound binder. Ignoring."));
		return;
	}

	if (!Property)
	{
		return;
	}

	PropertyTag = Tag;
	Property->ValueChanged.AddDynamic(this, &UPropertyValueChangedBinder::OnValueChanged);
	bIsBound = true;
}

void UPropertyValueChangedBinder::OnValueChanged(float OldValue, float NewValue)
{
	OnBinderValueChanged.Broadcast(PropertyTag, OldValue, NewValue);
}

