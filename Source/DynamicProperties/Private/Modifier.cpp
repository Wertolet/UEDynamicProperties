// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modifier.h"

UModifier::UModifier()
{
	Priority = 0;
}

float UModifier::Apply_Implementation(float BaseValue, float CurrentValue)
{
	// Default implementation just returns the current value unchanged
	return CurrentValue;
}

