// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"
#include "ModifierAddScaledBase.generated.h"

/**
 * Modifier that adds a scaled percentage of the base value to the current value
 */
UCLASS(Blueprintable, BlueprintType)
class DYNAMICPROPERTIES_API UModifierAddScaledBase : public UModifier
{
	GENERATED_BODY()

public:
	UModifierAddScaledBase();

	/** The multiplier to scale the base value by before adding */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier", meta = (ExposeOnSpawn = "true"))
	float BaseMultiplier;

	virtual float Apply_Implementation(float BaseValue, float CurrentValue) override;
};

