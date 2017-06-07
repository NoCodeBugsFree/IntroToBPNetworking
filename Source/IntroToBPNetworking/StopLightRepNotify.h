// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StopLightRepNotify.generated.h"

UCLASS()
class INTROTOBPNETWORKING_API AStopLightRepNotify : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStopLightRepNotify();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	
	UPROPERTY(ReplicatedUsing = OnRep_StreetLightScalar)
	float StreetLightScalar = 0.f;
	
	/** Remember! Server doesn't call it automatically like in BP version  */
	// Called whenever StreetLightScalar updated
	UFUNCTION()
	virtual void OnRep_StreetLightScalar();

	int32 ColorChangeTime = 1;

	FTimerHandle ColorChangeTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterial;
	
	void ChangeColor();
};
