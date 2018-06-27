// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ActorComponent.h"
#include "Widgets/Layout/Anchors.h"
#include "Engine/ActorChannel.h"
#include "Public/Interfaces/StatComponentInterface.h"
#include "BaseStatComponent.generated.h"

class UUserWidget;
class UStatModificator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STATSYSTEM_API UBaseStatComponent : public UActorComponent, public IStatComponentInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseStatComponent();

	// минимальное значение атрибута
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = StatComponent)
		float MinValue;

	// базовое (не изменяемое эффектами) максимальное значение атрибута
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = StatComponent)
		float BaseMaxValue;

	/*UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = StatComponent)
		UStatModificator* mod;*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// виджет (создается в BeginPlay)
	UPROPERTY()
		UUserWidget* statUI;

	// массив примененных к стату эффектов
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stat | Component")
		TArray<UStatModificator*> Modificators;

	// текущее значение 
	UPROPERTY(Replicated, BlueprintReadOnly)
		float CurrentValue;

	// рассчитанное максимальное значение (изменяемое эффектами. рассчитывается на основании BaseMaxValue) 
	UPROPERTY(Replicated, BlueprintReadOnly)
		float MaxValue;

	// Autorestore-----------------------------------------------------------------

	// эффект автовосстановления (создается в BeginPlay)
	UPROPERTY()
		UStatModificator* AutorestoreModificator;

	UPROPERTY(EditDefaultsOnly, Category = "StatComponent")
		bool bAutorestoreEnabled;

	// Класс модификатора автовосстановления (не обязателен) 
	UPROPERTY(EditDefaultsOnly, Category = "StatComponent", meta = (EditCondition = "bAutorestoreEnabled"))
		TSubclassOf<UStatModificator> AutorestoreModificatorClass;

	UPROPERTY()
		bool bCanAutorestore;

	//-----------------------------------------------------------------------------

public:	

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		void SetCurrentValue(float value);
		virtual void SetCurrentValue_Implementation(float value) override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		float GetCurrentValue();
		virtual float GetCurrentValue_Implementation() override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		void SetMaxValue(float value);
		virtual void SetMaxValue_Implementation(float value) override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		float GetMaxValue();
		virtual float GetMaxValue_Implementation() override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		void SetBaseMaxValue(float value);
		virtual void SetBaseMaxValue_Implementation(float value) override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		float GetBaseMaxValue();
		virtual float GetBaseMaxValue_Implementation() override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		void SetMinValue(float value);
		virtual void SetMinValue_Implementation(float value) override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintNativeEvent, Category = "StatComponent")
		float GetMinValue();
		virtual float GetMinValue_Implementation() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReplicateSubobjects(UActorChannel * Channel, FOutBunch * Bunch, FReplicationFlags * RepFlags);
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
