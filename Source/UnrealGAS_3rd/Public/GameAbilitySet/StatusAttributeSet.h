// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "StatusAttributeSet.generated.h"

/**
 * 
 */

// 어트리뷰트 Getter/Setter/Initializer를 쉽게 만들어주는 매크로
#define ATTRIBUTE_ACCESSIRS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UNREALGAS_3RD_API UStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UStatusAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttribute Health;
	ATTRIBUTE_ACCESSIRS(UStatusAttributeSet, Health)
};
