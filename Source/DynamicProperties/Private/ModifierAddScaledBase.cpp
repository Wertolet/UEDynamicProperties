// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModifierAddScaledBase.h"

UModifierAddScaledBase::UModifierAddScaledBase()
{
	BaseMultiplier = 0.0f;
}

float UModifierAddScaledBase::Apply_Implementation(float BaseValue, float CurrentValue)
{
	return CurrentValue + (BaseValue * BaseMultiplier);
}

