// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MasterTrappersAlpha1GameMode.generated.h"

UCLASS(minimalapi)
class AMasterTrappersAlpha1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMasterTrappersAlpha1GameMode();
    // Network new player
    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
private:
    // Handle the new player
    void HandleNewPlayer(APlayerController* NewPlayer);
};



