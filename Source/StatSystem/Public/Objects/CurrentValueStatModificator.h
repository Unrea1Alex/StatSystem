// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Objects/StatModificator.h"
#include "CurrentValueStatModificator.generated.h"

/**
 * 
 */
UCLASS()
class STATSYSTEM_API UCurrentValueStatModificator : public UStatModificator
{
	GENERATED_BODY()


	
protected:

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatModificator")
		void CalculateCurrentValue();
		virtual void CalculateCurrentValue_Implementation();

	UFUNCTION()
		virtual void Calculate() override;
	
};
