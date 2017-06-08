// Fill out your copyright notice in the Description page of Project Settings.

#include "Chest.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChestBottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestBottom"));
	SetRootComponent(ChestBottom);

	ChestLid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLid"));
	ChestLid->SetupAttachment(ChestBottom);

	Treasure = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Treasure"));
	Treasure->SetupAttachment(ChestBottom);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(ChestBottom);

	RelevancyRadiusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RelevancyRadiusMesh"));
	RelevancyRadiusMesh->SetupAttachment(ChestBottom);


	bIsChestOpen = false;
	bDoneOnce = false;

	bReplicates = true;

	// reduced for visual purposes
	NetCullDistanceSquared = 500000.0;
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();

	// On Component Begin Overlap
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AChest::OnOverlapBegin);

	// set relevancy mesh radius to visualize what is going on
	float XandY = FMath::Sqrt(NetCullDistanceSquared) / 50.f;
	FVector NewScale = FVector(XandY, XandY, 1);
	RelevancyRadiusMesh->SetWorldScale3D(NewScale);
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChest::MulticastPlayFX_Implementation()
{
	PlayGoldFX();
}

void AChest::OpenChest()
{
	ChestLid->SetRelativeRotation(FRotator(180, 0, 0));
}

void AChest::PlayGoldFX()
{
	if (EmitterTemplate)
	{
		UGameplayStatics::SpawnEmitterAttached(EmitterTemplate, Treasure);
	}
}

void AChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (bDoneOnce == false))
	{
		// BP DoOnce equivalent
		bDoneOnce = true;

		// [Server] - if I am server and I am not DS - play
		if (Role == ROLE_Authority)
		{
			// calls OnRep_ChestOpen() ONLY ON CLIENTS !
			bIsChestOpen = true;

			MulticastPlayFX();
			
			// manually call it on listen server
			OnRep_ChestOpen();
		}
	}
}

void AChest::OnRep_ChestOpen()
{
	UE_LOG(LogTemp, Error, TEXT("OnRep_ChestOpen"));
	if (bIsChestOpen)
	{
		OpenChest();
	}
}

// GetLifetimeReplicatedProps
void AChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AChest, bIsChestOpen);
}