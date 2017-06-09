// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IntroToBPNetworkingGameMode.h"
#include "IntroToBPNetworkingCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Ghost.h"

AIntroToBPNetworkingGameMode::AIntroToBPNetworkingGameMode()
{
	
}

void AIntroToBPNetworkingGameMode::SpawnActor(TSubclassOf<AGhost> GhostType, FTransform SpawnTransform)
{
	if (GhostType)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			AGhost* SpawnedGhost = World->SpawnActor<AGhost>(GhostType, SpawnTransform, SpawnParams);
		}
	}
}
