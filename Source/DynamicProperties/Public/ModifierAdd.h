// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"
#include "ModifierAdd.generated.h"

/**
 * Modifier that adds a constant value to the current value
 */
UCLASS(Blueprintable, BlueprintType)
class DYNAMICPROPERTIES_API UModifierAdd : public UModifier
{
	GENERATED_BODY()

public:
	UModifierAdd();

	/** The value to add */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier", meta = (ExposeOnSpawn = "true"))
	float AdditiveValue;

	virtual float Apply_Implementation(float BaseValue, float CurrentValue) override;
};

