// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

UCLASS()
class INTROTOBPNETWORKING_API AChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	/** Chest open state */
	UPROPERTY(ReplicatedUsing = OnRep_ChestOpen)
	uint32 bIsChestOpen : 1;

	/**  BP DoOnce equivalent */ 
	uint32 bDoneOnce : 1;

	/** Play Gold FX when you are in relevant radius from chest   */
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayFX();
	void MulticastPlayFX_Implementation();

	/** Simple chest lid rotation */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void OpenChest();

	/** FX to play when chest is opening  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void PlayGoldFX();

	/** On Chest Begin Overlap    */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	/** Remember! Server doesn't call it automatically like in BP version ! Call it manually */
	// Called whenever bIsActive updated
	UFUNCTION()
	virtual void OnRep_ChestOpen();
	


	/** Chest components  */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* EmitterTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ChestBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ChestLid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Treasure;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RelevancyRadiusMesh;;
	
};
