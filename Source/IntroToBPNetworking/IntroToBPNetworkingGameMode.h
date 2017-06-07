// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IntroToBPNetworkingGameMode.generated.h"

UCLASS(minimalapi)
class AIntroToBPNetworkingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AIntroToBPNetworkingGameMode();

	// [Server] always - GameMode exists only on Server
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnActor(TSubclassOf<AGhost> GhostType, FTransform SpawnTransform);

protected:

	
	

};



