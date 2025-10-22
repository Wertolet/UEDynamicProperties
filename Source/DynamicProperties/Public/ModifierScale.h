// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"
#include "ModifierScale.generated.h"

/**
 * Modifier that multiplies the current value by a constant multiplier
 */
UCLASS(Blueprintable, BlueprintType)
class DYNAMICPROPERTIES_API UModifierScale : public UModifier
{
	GENERATED_BODY()

public:
	UModifierScale();

	/** The multiplier to scale the value by */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier", meta = (ExposeOnSpawn = "true"))
	float Multiplier;

	virtual float Apply_Implementation(float BaseValue, float CurrentValue) override;
};

