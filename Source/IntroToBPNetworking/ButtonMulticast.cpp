// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonMulticast.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AButtonMulticast::AButtonMulticast()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	SetRootComponent(Base);

	ButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBase"));
	ButtonBase->SetupAttachment(Base);

	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Button->SetupAttachment(ButtonBase);
	

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Base);
	Trigger->SetBoxExtent(FVector(45, 45, 25));

	LogoBase = CreateDefaultSubobject<USceneComponent>(TEXT("LogoBase"));
	LogoBase->SetupAttachment(Base);

	Logo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Logo"));
	Logo->SetupAttachment(LogoBase);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(LogoBase);
	PointLight->SetIntensity(1000.f);
	PointLight->AttenuationRadius = 250.f;
	PointLight->SourceLength = 150.f;
	PointLight->SourceRadius = 5.f;
	PointLight->bAffectsWorld = true;
	PointLight->CastShadows = false;
	PointLight->LightColor = FColor::Blue;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AButtonMulticast::BeginPlay()
{
	Super::BeginPlay();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AButtonMulticast::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AButtonMulticast::OnOverlapEnd);
}

// Called every frame
void AButtonMulticast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void  AButtonMulticast::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		// [Server]
		if (Role == ROLE_Authority)
		{
			ButtonPressed();
		}
	}
}

void AButtonMulticast::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		// [Server]
		if (Role == ROLE_Authority)
		{
			ButtonReleased();
		}
	}
}

void AButtonMulticast::ButtonPressed_Implementation()
{	
	Button->SetRelativeLocation(FVector(0, 0, -10));
	
	if (EmitterTemplate)
	{
		UGameplayStatics::SpawnEmitterAttached(EmitterTemplate, Logo);
	}

	// BlueprintImplementableEvent - BP version from Epic Games
	// UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	PlaySpinFX();
}

void AButtonMulticast::ButtonReleased_Implementation()
{
	Button->SetRelativeLocation(FVector(0, 0, 0));
}

