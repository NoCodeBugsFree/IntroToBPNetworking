// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 500.f;
	ProjectileMovementComponent->MaxSpeed = 500.f;
	ProjectileMovementComponent->bShouldBounce = true;

	/** Called when projectile impacts something and bounces are enabled. */
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ABomb::OnProjectileBounce);

	bReplicates = true;
	bIsArmed = false;
}

void ABomb::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DynamicMaterial = GetStaticMeshComponent()->CreateDynamicMaterialInstance(0);

	SetBombColor(FLinearColor::Black);
}

void ABomb::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// [Server]
	if (Role == ROLE_Authority && bIsArmed == false) // apply damage once check
	{
		bIsArmed = true; // apply damage once
		OnRep_bIsArmed(); // for listen server

		FTimerHandle ExplosionTimer;
		GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ABomb::OnFuseExpired, FuseTime, false);
	}
}

void ABomb::ApplyExplosionDamage()
{
	if (Role == ROLE_Authority)
	{
		TArray<AActor*> IgnoreActors;
		FVector ExplosionLocation = GetStaticMeshComponent()->GetComponentLocation();
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, ExplosionLocation , ExlosionRadius, UDamageType::StaticClass(), IgnoreActors);
	}
}

void ABomb::OnFuseExpired()
{
	if (Role == ROLE_Authority)
	{
		ApplyExplosionDamage();

		MulticastExplosionFX();

		SetLifeSpan(2.f);
	}
}

void ABomb::SetBombColor(FLinearColor NewColor)
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", NewColor);
	}
}

void ABomb::MulticastExplosionFX_Implementation()
{
	/** Particles FX  */
	if (ExplosionParticlesTemplate)
	{
		FTransform SpawnTransform = GetActorTransform();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticlesTemplate, SpawnTransform);
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ExplosionParticlesTemplate == NULL!"));
	}

	/** Hide the bomb  */
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void ABomb::OnRep_bIsArmed()
{	
	if (bIsArmed)
	{
		SetBombColor(FLinearColor::Red);
	}
}

// GetLifetimeReplicatedProps
void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABomb, bIsArmed);
}
