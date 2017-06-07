// Fill out your copyright notice in the Description page of Project Settings.

#include "GhostReplicatedVariables.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"

AGhostReplicatedVariables::AGhostReplicatedVariables()
{
	PrimaryActorTick.bCanEverTick = true;

	// 
	bReplicates = true;
}

void AGhostReplicatedVariables::BeginPlay()
{
	Super::BeginPlay();

	// [Server] - Initialization and timer Server Only
	if (Role == ROLE_Authority)
	{	
		InitializeHealth();
		GetWorldTimerManager().SetTimer(HealthDecrementTimer, this, &AGhostReplicatedVariables::UpdateHealth, HealthDecrementTime, true);
	}
}

void AGhostReplicatedVariables::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateText();
}

void AGhostReplicatedVariables::UpdateHealth()
{
	Health -= HealthDecrementAmount;
	if (Health <= 0)
	{
		InitializeHealth();
	}
	UpdateText();
}

void AGhostReplicatedVariables::InitializeHealth()
{
	Health = MaxHealth;
}

void AGhostReplicatedVariables::UpdateText()
{
	GhostText->SetText(FText::FromString(FString("Health: ") + FString::FromInt(Health)));
}

// GetLifetimeReplicatedProps
void AGhostReplicatedVariables::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGhostReplicatedVariables, Health);
}