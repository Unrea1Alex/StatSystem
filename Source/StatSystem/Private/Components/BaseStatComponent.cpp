// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/BaseStatComponent.h"
#include "UnrealNetwork.h"
#include "Blueprint/UserWidget.h"

#include "Public/Objects/StatModificator.h"

void UBaseStatComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	//DOREPLIFETIME_CONDITION(UHealthComponent, health, COND_OwnerOnly);
	DOREPLIFETIME(UBaseStatComponent, MinValue);
	DOREPLIFETIME(UBaseStatComponent, MaxValue);
	DOREPLIFETIME(UBaseStatComponent, CurrentValue);
	DOREPLIFETIME(UBaseStatComponent, Modificators);
	//DOREPLIFETIME(UBaseStatComponent, statEffect);
	//DOREPLIFETIME_CONDITION(UBaseStatComponent, statEffect, COND_None);
}


// Sets default values for this component's properties
UBaseStatComponent::UBaseStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bReplicates = true;
	// ...

	MinValue = 0;
	BaseMaxValue = 100;

}


// Called when the game starts
void UBaseStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutorestoreEnabled)
	{
		if (AutorestoreModificatorClass)
		{
			AutorestoreModificator = NewObject<UStatModificator>(GetOuter(), AutorestoreModificatorClass);
		}
		else
		{
			AutorestoreModificator = NewObject<UStatModificator>(GetOuter());
		}
	}

	MaxValue = BaseMaxValue;
	CurrentValue = MaxValue;
}


void UBaseStatComponent::SetCurrentValue_Implementation(float value)
{
	CurrentValue = FMath::Clamp(value, MinValue, MaxValue);
}

float UBaseStatComponent::GetCurrentValue_Implementation()
{
	return CurrentValue;
}

void UBaseStatComponent::SetMaxValue_Implementation(float value)
{
	MaxValue = value;
}

float UBaseStatComponent::GetMaxValue_Implementation()
{
	return MaxValue;
}

void UBaseStatComponent::SetBaseMaxValue_Implementation(float value)
{
	BaseMaxValue = value;
}

float UBaseStatComponent::GetBaseMaxValue_Implementation()
{
	return BaseMaxValue;
}

void UBaseStatComponent::SetMinValue_Implementation(float value)
{
	MinValue = value;
}

float UBaseStatComponent::GetMinValue_Implementation()
{
	return MinValue;
}

// Called every frame
void UBaseStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBaseStatComponent::ReplicateSubobjects(UActorChannel * Channel, FOutBunch * Bunch, FReplicationFlags * RepFlags)
{
	check(Channel);
	check(Bunch);
	check(RepFlags);

	bool WroteSomething = false;

	for (auto modificator : Modificators)
	{
		WroteSomething |= Channel->ReplicateSubobject(modificator, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

//new comment

void UBaseStatComponent::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (!bAutorestoreEnabled)
	{
		AutorestoreModificatorClass = nullptr;
	}
}

