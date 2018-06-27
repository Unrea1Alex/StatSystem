// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Objects/StatModificator.h"
#include "UnrealNetwork.h"
#include "Public/Components/BaseStatComponent.h"

void UStatModificator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(UStatModificator, Name, COND_None);
	DOREPLIFETIME_CONDITION(UStatModificator, BaseValue, COND_None);
	DOREPLIFETIME_CONDITION(UStatModificator, ImpactInterval, COND_None);
	DOREPLIFETIME_CONDITION(UStatModificator, BaseImpactTime, COND_None);
	DOREPLIFETIME_CONDITION(UStatModificator, bStacked, COND_None);
	DOREPLIFETIME_CONDITION(UStatModificator, CurrentStackCount, COND_None);

	DOREPLIFETIME_CONDITION(UStatModificator, Owner, COND_None);
	DOREPLIFETIME_CONDITION(UStatModificator, Instigator, COND_None);
}

UStatModificator::UStatModificator()
{
	BaseValue = 1.f;
	ImpactInterval = 1.f;
	BaseImpactTime = 5.f;
	bShowUI = true;
	bInfinity = false;
	bValueInPercent = false;
	bStacked = false;
	MaxStackCount = 0;
	CurrentStackCount = 0;
	ImpactTime = BaseImpactTime;
}

void UStatModificator::Start()
{
	if (!CanStart())
	{
		return;
	}

	ImpactTime = BaseImpactTime;

	if (bStacked && CurrentStackCount == 0)
	{
		CurrentStackCount = 1;
	}

	Owner->GetWorld()->GetTimerManager().ClearTimer(ModificatorTimer);

	Owner->GetWorld()->GetTimerManager().SetTimer(ModificatorTimer, this, &UStatModificator::Calculate, ImpactInterval, true, 0.01f);

}

void UStatModificator::AddStack()
{
	CurrentStackCount = FMath::Clamp(CurrentStackCount + 1, 0, MaxStackCount);

	ImpactTime = BaseImpactTime;

	Owner->GetWorld()->GetTimerManager().ClearTimer(ModificatorTimer);

	Owner->GetWorld()->GetTimerManager().SetTimer(ModificatorTimer, this, &UStatModificator::Calculate, ImpactInterval, true, 0.01f);
}

void UStatModificator::RemoveStack()
{
	CurrentStackCount = FMath::Clamp(CurrentStackCount - 1, 0, MaxStackCount);

	ImpactTime = BaseImpactTime;

	Owner->GetWorld()->GetTimerManager().ClearTimer(ModificatorTimer);

	Owner->GetWorld()->GetTimerManager().SetTimer(ModificatorTimer, this, &UStatModificator::Calculate, ImpactInterval, true, 0.01f);
}

void UStatModificator::End()
{
	Owner->GetWorld()->GetTimerManager().ClearTimer(ModificatorTimer);
	ModificatorTimer.Invalidate();
	CurrentStackCount = 0;
}

ENetRole UStatModificator::GetOwnerRole() const
{
	AActor* MyOwner = GetOwner();
	return (MyOwner ? MyOwner->Role.GetValue() : ROLE_None);
}

AActor * UStatModificator::GetOwner() const
{
	return Owner;
}

bool UStatModificator::ReplicateSubobjects(UActorChannel * Channel, FOutBunch * Bunch, FReplicationFlags * RepFlags)
{
	check(Channel);
	check(Bunch);
	check(RepFlags);

	bool WroteSomething = false;

	WroteSomething |= Owner->ReplicateSubobjects(Channel, Bunch, RepFlags);

	return WroteSomething;
}

UStatModificator::UStatModificator()
{
	bReplicates = true;
}

void UStatModificator::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (!bStacked)
	{
		MaxStackCount = 0;
	}
}

void UStatModificator::Calculate()
{
	if (!CanStart())
	{
		return;
	}

	ImpactTime -= ImpactInterval;

	if (ImpactTime <= 0.f)
	{
		if (bStacked && CurrentStackCount > 0)
		{
			RemoveStack();
		}
		else
		{
			End();
		}
	}
}

bool UStatModificator::CanStart()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		return false;
	}

	if (!AffectedStat)
	{
		return false;
	}

	return true;
}

bool UStatModificator::IsSupportedForNetworking() const
{
	return true;
}

bool UStatModificator::CallRemoteFunction(UFunction * Function, void * Parms, FOutParmRec * OutParms, FFrame * Stack)
{
	if (!Owner) return false;
	UNetDriver* NetDriver = Owner->GetNetDriver();
	if (!NetDriver) return false;

	NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);

	return true;
}

int32 UStatModificator::GetFunctionCallspace(UFunction * Function, void * Parameters, FFrame * Stack)
{
	return (Owner ? Owner->GetFunctionCallspace(Function, Parameters, Stack) : FunctionCallspace::Local);
}
