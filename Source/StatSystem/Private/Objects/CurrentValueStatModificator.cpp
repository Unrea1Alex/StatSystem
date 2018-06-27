// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Objects/CurrentValueStatModificator.h"

void UCurrentValueStatModificator::CalculateCurrentValue_Implementation()
{
	if (GetOwnerRole() < ROLE_Authority)
	{

	}
}

void UCurrentValueStatModificator::Calculate()
{
}
