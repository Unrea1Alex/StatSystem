// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/ActorChannel.h"
#include "StatModificator.generated.h"


class UBaseStatComponent;
class UTexture2D;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EEffectState : uint8
{
	BS_Start 	UMETA(DisplayName = "Start"),
	BS_Change 	UMETA(DisplayName = "Change"),
	BS_Remove	UMETA(DisplayName = "Remove")
};


/**
 * 
 */
UCLASS(Blueprintable)
class STATSYSTEM_API UStatModificator : public UObject
{
	GENERATED_BODY()

public: 

	UStatModificator();

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		FName Name;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		float BaseValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		float ImpactInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		float BaseImpactTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator | UI")
		UTexture2D* EffectIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator | UI")
		bool bShowUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		TSubclassOf<UBaseStatComponent> AffectedStatClass;

	// массив модификаторов при наличии которых данный модификатор не применится
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		TArray<TSubclassOf<UBaseStatComponent>> StatModificatorsClass;

	//массив модификаторов которые данный модификатор удаляет
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		TArray<TSubclassOf<UBaseStatComponent>> DeletingStatModificatorsClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		bool bInfinity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		bool bValueInPercent;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator")
		bool bStacked;

	UPROPERTY(EditDefaultsOnly, EditDefaultsOnly, BlueprintReadOnly, Category = "StatModificator", meta = (EditCondition = "bStacked"))
		int MaxStackCount;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "StatModificator")
		int CurrentStackCount;

	// актор создавший эффект
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "StatModificator")
		AActor* Instigator;

	// актор владеющий эффектом (на кого воздействует)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "StatModificator")
		AActor* Owner;
	
protected:

	UPROPERTY()
		UBaseStatComponent* AffectedStat;

	UPROPERTY()
		float ImpactTime;

	UPROPERTY()
		FTimerHandle ModificatorTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Replication")
		uint8 bReplicates : 1;

public:

	UStatModificator();

	UFUNCTION()
		void Start();

	UFUNCTION()
		void AddStack();

	UFUNCTION()
		void RemoveStack();

	UFUNCTION()
		void End();

	/** Returns whether replication is enabled or not. */
	FORCEINLINE bool GetIsReplicated() const
	{
		return bReplicates;
	}

	ENetRole GetOwnerRole() const;

	AActor* GetOwner() const;

	virtual bool ReplicateSubobjects(UActorChannel * Channel, FOutBunch * Bunch, FReplicationFlags * RepFlags);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	UFUNCTION()
		virtual void Calculate();

	UFUNCTION()
		bool CanStart();

	/** IsSupportedForNetworking means an object can be referenced over the network */
	virtual bool IsSupportedForNetworking() const;

	virtual bool CallRemoteFunction(UFunction * Function, void * Parms, FOutParmRec * OutParms, FFrame * Stack) override;

	virtual int32 GetFunctionCallspace(UFunction * Function, void * Parameters, FFrame * Stack) override;
};
