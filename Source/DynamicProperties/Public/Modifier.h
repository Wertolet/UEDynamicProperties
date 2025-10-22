// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Modifier.generated.h"

/**
 * Abstract base class for modifiers that can be applied to dynamic properties
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class DYNAMICPROPERTIES_API UModifier : public UObject
{
	GENERATED_BODY()

public:
	UModifier();

	/** Priority for modifier application order (lower values are applied first) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	int32 Priority;

	/**
	 * Applies the modifier to a value
	 * @param BaseValue The original base value
	 * @param CurrentValue The current value after previous modifiers
	 * @return The modified value
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modifier")
	float Apply(float BaseValue, float CurrentValue);
	virtual float Apply_Implementation(float BaseValue, float CurrentValue);
};

