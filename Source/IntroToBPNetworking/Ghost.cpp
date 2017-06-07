// Fill out your copyright notice in the Description page of Project Settings.

#include "Ghost.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GhostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GhostMesh"));
	SetRootComponent(GhostMesh);

	GhostText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("GhostText"));
	GhostText->SetupAttachment(GhostMesh);
	
	// Replication 1.1
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();

	// [Server] 1.2
	if (Role == ROLE_Authority)
	{
		GhostText->SetText(FText::FromString("Network Authority"));
	}
	else // [Client]
	{
		GhostText->SetText(FText::FromString("Remote Client"));
		DynamicMaterial = GhostMesh->CreateDynamicMaterialInstance(0);
		DynamicMaterial->SetVectorParameterValue("Upper color", FLinearColor::Blue);
	}
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

