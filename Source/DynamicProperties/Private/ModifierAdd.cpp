// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModifierAdd.h"

UModifierAdd::UModifierAdd()
{
	AdditiveValue = 0.0f;
}

float UModifierAdd::Apply_Implementation(float BaseValue, float CurrentValue)
{
	return CurrentValue + AdditiveValue;
}

