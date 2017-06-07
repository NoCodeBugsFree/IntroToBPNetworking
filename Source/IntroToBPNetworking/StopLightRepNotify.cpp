// Fill out your copyright notice in the Description page of Project Settings.

#include "StopLightRepNotify.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AStopLightRepNotify::AStopLightRepNotify()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AStopLightRepNotify::BeginPlay()
{
	Super::BeginPlay();
	
	// [Server]
	if (Role == ROLE_Authority)
	{
		// start timer which is changing the light color
		GetWorldTimerManager().SetTimer(ColorChangeTimer, this, &AStopLightRepNotify::ChangeColor, ColorChangeTime, true);
	}
}

// Called every frame
void AStopLightRepNotify::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStopLightRepNotify::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DynamicMaterial = Mesh->CreateDynamicMaterialInstance(1);
}

void AStopLightRepNotify::OnRep_StreetLightScalar()
{
	// change material every time StreetLightScalar variable changes
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("State"), StreetLightScalar);
	}
}

void AStopLightRepNotify::ChangeColor()
{
	StreetLightScalar = FMath::FRandRange(0.f, 1.f);

	// [Server] Remember! Server doesn't call it automatically like in BP version
	// Lets call it manually !
	if (Role == ROLE_Authority)
	{
		OnRep_StreetLightScalar();
	}
}

// GetLifetimeReplicatedProps
void AStopLightRepNotify::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStopLightRepNotify, StreetLightScalar);
}