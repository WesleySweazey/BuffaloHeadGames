// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MasterTrappersAlpha1HUD.generated.h"

UCLASS()
class AMasterTrappersAlpha1HUD : public AHUD
{
	GENERATED_BODY()

public:
	AMasterTrappersAlpha1HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
        void Win();
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
        void Lose();

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
    
};

