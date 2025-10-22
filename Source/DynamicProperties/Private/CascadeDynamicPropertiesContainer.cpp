// Copyright Epic Games, Inc. All Rights Reserved.

#include "CascadeDynamicPropertiesContainer.h"

UCascadeDynamicPropertiesContainer::UCascadeDynamicPropertiesContainer()
{
}

float UCascadeDynamicPropertiesContainer::GetPropertyValueOrDefault(FGameplayTag PropertyTag, float DefaultValue)
{
	// If the property exists, return its calculated value
	if (UDynamicProperty* Property = GetProperty(PropertyTag))
	{
		return Property->GetValue();
	}

	// If property doesn't exist, walk up hierarchy to find a parent and calculate from there
	if (UDynamicProperty* ParentProperty = FindNearestParentProperty(PropertyTag))
	{
		return ParentProperty->GetValue();
	}

	// No properties in hierarchy found, return default
	return DefaultValue;
}

void UCascadeDynamicPropertiesContainer::OnPropertyAddedInternal(FGameplayTag PropertyTag, UDynamicProperty* Property)
{
	if (!Property)
	{
		return;
	}

	// When adding a new property, check if it should use a parent's value as base
	if (UDynamicProperty* ParentProperty = FindNearestParentProperty(PropertyTag))
	{
		// Set the parent's current value as this property's base value
		Property->SetBaseValue(ParentProperty->GetValue());
	}

	// Call parent implementation to fire the initial OnPropertyValueChanged event
	Super::OnPropertyAddedInternal(PropertyTag, Property);
}

void UCascadeDynamicPropertiesContainer::OnPropertyValueChangedInternal(FGameplayTag PropertyTag, float OldValue, float NewValue)
{
	// When a property changes, update base values of all direct children
	UpdateChildPropertiesBaseValue(PropertyTag, NewValue);

	// Call parent implementation to broadcast the event
	Super::OnPropertyValueChangedInternal(PropertyTag, OldValue, NewValue);
}

void UCascadeDynamicPropertiesContainer::UpdateChildPropertiesBaseValue(FGameplayTag ParentTag, float NewParentValue)
{
	// Get all children that should be updated (direct and non-direct without intermediate nodes)
	TArray<FGameplayTag> ChildrenToUpdate;
	GetChildrenToUpdate(ParentTag, ChildrenToUpdate);

	for (const FGameplayTag& ChildTag : ChildrenToUpdate)
	{
		if (UDynamicProperty* ChildProperty = GetProperty(ChildTag))
		{
			// Setting base value will trigger recalculation and fire events, creating cascade
			ChildProperty->SetBaseValue(NewParentValue);
		}
	}
}

UDynamicProperty* UCascadeDynamicPropertiesContainer::FindNearestParentProperty(FGameplayTag ChildTag)
{
	// Get the tag as a string and split by dots
	FString TagString = ChildTag.ToString();
	TArray<FString> TagParts;
	TagString.ParseIntoArray(TagParts, TEXT("."));

	// Walk up the hierarchy from immediate parent to root
	for (int32 i = TagParts.Num() - 1; i > 0; --i)
	{
		// Build parent tag string
		FString ParentTagString;
		for (int32 j = 0; j < i; ++j)
		{
			if (j > 0)
			{
				ParentTagString += TEXT(".");
			}
			ParentTagString += TagParts[j];
		}

		// Try to get property with this parent tag
		FGameplayTag ParentTag = FGameplayTag::RequestGameplayTag(FName(*ParentTagString), false);
		if (ParentTag.IsValid())
		{
			if (UDynamicProperty* ParentProperty = GetProperty(ParentTag))
			{
				return ParentProperty;
			}
		}
	}

	return nullptr;
}

void UCascadeDynamicPropertiesContainer::GetDirectChildTags(FGameplayTag ParentTag, TArray<FGameplayTag>& OutChildTags)
{
	OutChildTags.Empty();

	FString ParentTagString = ParentTag.ToString();
	int32 ParentDepth = GetTagDepth(ParentTagString);

	// Iterate through all properties to find direct children
	TArray<FGameplayTag> AllKeys;
	DynamicProperties.GetKeys(AllKeys);

	for (const FGameplayTag& Tag : AllKeys)
	{
		FString TagString = Tag.ToString();
		
		// Check if this tag starts with parent tag
		if (TagString.StartsWith(ParentTagString + TEXT(".")))
		{
			// Count depth of this tag
			int32 TagDepth = GetTagDepth(TagString);

			// Only include direct children (one level deeper)
			if (TagDepth == ParentDepth + 1)
			{
				OutChildTags.Add(Tag);
			}
		}
	}
}

int32 UCascadeDynamicPropertiesContainer::GetTagDepth(const FString& TagString)
{
	if (TagString.IsEmpty())
	{
		return 0;
	}

	// Count dots in the string - depth is dots + 1
	int32 DotCount = 0;
	for (int32 i = 0; i < TagString.Len(); ++i)
	{
		if (TagString[i] == '.')
		{
			++DotCount;
		}
	}

	return DotCount + 1;
}

void UCascadeDynamicPropertiesContainer::GetChildrenToUpdate(FGameplayTag ParentTag, TArray<FGameplayTag>& OutChildTags)
{
	OutChildTags.Empty();

	FString ParentTagString = ParentTag.ToString();
	int32 ParentDepth = GetTagDepth(ParentTagString);

	// Use a set to prevent duplicates
	TSet<FGameplayTag> UniqueChildTags;

	// Iterate through all properties to find children to update
	TArray<FGameplayTag> AllKeys;
	DynamicProperties.GetKeys(AllKeys);

	for (const FGameplayTag& Tag : AllKeys)
	{
		FString TagString = Tag.ToString();
		
		// Check if this tag starts with parent tag
		if (TagString.StartsWith(ParentTagString + TEXT(".")))
		{
			int32 TagDepth = GetTagDepth(TagString);

			// Check if this is a direct child (exactly one level deeper)
			if (TagDepth == ParentDepth + 1)
			{
				// Direct child - always include
				UniqueChildTags.Add(Tag);
			}
			// Check if this is a non-direct child without intermediate nodes
			else if (TagDepth > ParentDepth + 1)
			{
				// Check if there are any intermediate nodes between parent and this tag
				bool HasIntermediateNodes = false;
				
				// Find the next part of the tag
				FString RemainingPart = TagString.Mid(ParentTagString.Len() + 1);
				TArray<FString> RemainingParts;
				RemainingPart.ParseIntoArray(RemainingParts, TEXT("."));
				
				// Check each possible intermediate level
				for (int32 IntermediateDepth = ParentDepth + 1; IntermediateDepth < TagDepth; ++IntermediateDepth)
				{
					// Build the intermediate tag string
					FString IntermediateTagString = ParentTagString;
					for (int32 i = 0; i < IntermediateDepth - ParentDepth; ++i)
					{						
						if (i < RemainingParts.Num())
						{
							IntermediateTagString += TEXT(".") + RemainingParts[i];
						}
					}

					// Check if this intermediate tag exists in the collection
					FGameplayTag IntermediateTag = FGameplayTag::RequestGameplayTag(FName(*IntermediateTagString), false);
					if (IntermediateTag.IsValid() && GetProperty(IntermediateTag) != nullptr)
					{
						HasIntermediateNodes = true;
						break;
					}
				}

				// If no intermediate nodes exist, this is a valid non-direct child
				if (!HasIntermediateNodes)
				{
					UniqueChildTags.Add(Tag);
				}
			}
		}
	}

	// Convert set to array for output
	OutChildTags = UniqueChildTags.Array();
}

