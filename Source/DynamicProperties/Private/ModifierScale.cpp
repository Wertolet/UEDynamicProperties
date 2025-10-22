// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModifierScale.h"

UModifierScale::UModifierScale()
{
	Multiplier = 1.0f;
}

float UModifierScale::Apply_Implementation(float BaseValue, float CurrentValue)
{
	return CurrentValue * Multiplier;
}

