// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFrwkSessionsPlugin.h"
#include "CoreMinimal.h"
#include "MenuInterface.generated.h"

//TODO Week 9:
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
 //TODO Week 9:
class GAMEFRWKSESSIONSPLUGIN_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Host(FString ServerName, FString Team) = 0;
	virtual void Join(uint32 Index, FString Team) = 0;
	virtual void LoadMainMenu() = 0;
	virtual void RefreshServerList() = 0;

};
