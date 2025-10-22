// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Modifier.h"
#include "DynamicProperty.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, float, OldValue, float, NewValue);

/**
 * A dynamic property that can have modifiers applied to modify its value
 */
UCLASS(Blueprintable, BlueprintType)
class DYNAMICPROPERTIES_API UDynamicProperty : public UObject
{
	GENERATED_BODY()

public:
	UDynamicProperty();

protected:
	/** The current calculated value after all modifiers */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dynamic Property")
	float Value;

	/** The base value before modifiers are applied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter = GetBaseValue, BlueprintSetter = SetBaseValue, Category = "Dynamic Property")
	float BaseValue;

	/** List of modifiers applied to this property */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Property")
	TArray<UModifier*> Modifiers;

public:
	/** Event fired when the value changes */
	UPROPERTY(BlueprintAssignable, Category = "Dynamic Property")
	FOnValueChanged ValueChanged;

	/**
	 * Calculates the value for a given base value by applying all modifiers
	 * @param InBaseValue The base value to calculate from
	 * @return The calculated value after all modifiers
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Property")
	float CalculateForBaseValue(float InBaseValue);

	/**
	 * Recalculates the current value using the current base value
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Property")
	void Recalculate();

	/**
	 * Adds a modifier to the property and recalculates
	 * @param Modifier The modifier to add
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Property")
	void AddModifier(UModifier* Modifier);

	/**
	 * Removes a modifier from the property and recalculates
	 * @param Modifier The modifier to remove
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Property")
	void RemoveModifier(UModifier* Modifier);

	/**
	 * Gets the current calculated value
	 * @return The current value
	 */
	UFUNCTION(BlueprintGetter, Category = "Dynamic Property")
	float GetValue() const { return Value; }

	/**
	 * Gets the base value
	 * @return The base value
	 */
	UFUNCTION(BlueprintGetter, Category = "Dynamic Property")
	float GetBaseValue() const { return BaseValue; }

	/**
	 * Sets the base value and recalculates
	 * @param NewBaseValue The new base value
	 */
	UFUNCTION(BlueprintSetter, Category = "Dynamic Property")
	void SetBaseValue(float NewBaseValue);

private:
	/**
	 * Sorts the modifiers array by priority
	 */
	void SortModifiers();
};

