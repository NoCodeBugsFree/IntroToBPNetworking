// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ghost.generated.h"

UCLASS()
class INTROTOBPNETWORKING_API AGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**  Ghost Static Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GhostMesh;
	
	/** Help Text  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* GhostText;

	/** Dynamic Material  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DynamicMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
