// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Bomb.generated.h"

/**
 * 
 */
UCLASS()
class INTROTOBPNETWORKING_API ABomb : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:

	ABomb();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

protected:

	/** Apply explosion damage in area */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void ApplyExplosionDamage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float FuseTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float ExplosionDamage = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float ExlosionRadius = 1000.f;
	
	/** Calls when Fuse Time expires  */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void OnFuseExpired();

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetBombColor(FLinearColor NewColor);

	/** ExplosionFX  */
	UFUNCTION(NetMulticast, Unreliable) 
	void MulticastExplosionFX();
	void MulticastExplosionFX_Implementation();

	// -------- ReplicatedUsing --------------------------------------------------------------

	/** Shows is Bomb enable/disable now, 
	 * needs for firing OnRep_bIsArmed() ReplicatedUsing function
	 * and preventing multiple explosions - do explosion once
	**/
	UPROPERTY(ReplicatedUsing = OnRep_bIsArmed)
	uint32 bIsArmed : 1;
	
	UFUNCTION()
	virtual void OnRep_bIsArmed();

	// -----------------------------------------------------------------------------------

	/** Explosion Particles Template  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ExplosionParticlesTemplate;

	/** Dynamic Material  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DynamicMaterial;

	/**  Projectile Movement Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

	
};
