// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STATSYSTEM_API IStatComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		void SetCurrentValue(float value);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		float GetCurrentValue();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		void SetMaxValue(float value);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		float GetMaxValue();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		void SetBaseMaxValue(float value);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		float GetBaseMaxValue();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		void SetMinValue(float value);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = StatComponent)
		float GetMinValue();
};
