// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ghost.h"
#include "GhostReplicatedVariables.generated.h"

/**
 * 
 */
UCLASS()
class INTROTOBPNETWORKING_API AGhostReplicatedVariables : public AGhost
{
	GENERATED_BODY()
	
public:

	AGhostReplicatedVariables();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	FTimerHandle HealthDecrementTimer;

	/** Health Replicated/NonReplicated Variable  */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 Health = 0;

	/** Maximum available health value  */
	int32 MaxHealth = 100;

	/** Health Decrement Amount per timer tick */
	int32 HealthDecrementAmount = 1;

	/** Timer tick interval  */
	float HealthDecrementTime = 0.1;

private:

	/** Simple decrease health  */
	void UpdateHealth();

	/** Initialize Health  */
	void InitializeHealth();

	/** Update Text  */
	void UpdateText();
	
	
};
