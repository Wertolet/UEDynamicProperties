// Copyright Epic Games, Inc. All Rights Reserved.

#include "DynamicProperty.h"

UDynamicProperty::UDynamicProperty()
{
	BaseValue = 0.0f;
	Value = 0.0f;
}

float UDynamicProperty::CalculateForBaseValue(float InBaseValue)
{
	float CalculatedValue = InBaseValue;

	// Apply all modifiers sequentially
	for (UModifier* Modifier : Modifiers)
	{
		if (Modifier)
		{
			CalculatedValue = Modifier->Apply(InBaseValue, CalculatedValue);
		}
	}

	return CalculatedValue;
}

void UDynamicProperty::Recalculate()
{
	float OldValue = Value;
	Value = CalculateForBaseValue(BaseValue);

	// Fire event if value changed
	if (!FMath::IsNearlyEqual(OldValue, Value))
	{
		ValueChanged.Broadcast(OldValue, Value);
	}
}

void UDynamicProperty::AddModifier(UModifier* Modifier)
{
	if (Modifier)
	{
		Modifiers.Add(Modifier);
		SortModifiers();
		Recalculate();
	}
}

void UDynamicProperty::RemoveModifier(UModifier* Modifier)
{
	if (Modifier)
	{
		Modifiers.Remove(Modifier);
		Recalculate();
	}
}

void UDynamicProperty::SetBaseValue(float NewBaseValue)
{
	if (!FMath::IsNearlyEqual(BaseValue, NewBaseValue))
	{
		BaseValue = NewBaseValue;
		Recalculate();
	}
}

void UDynamicProperty::SortModifiers()
{
	// Sort modifiers by priority (ascending order - lower priority values are applied first)
	Modifiers.Sort([](const UModifier& A, const UModifier& B)
	{
		return A.Priority < B.Priority;
	});
}

