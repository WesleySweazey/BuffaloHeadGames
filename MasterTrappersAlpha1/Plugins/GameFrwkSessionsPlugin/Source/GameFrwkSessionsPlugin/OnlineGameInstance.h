// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFrwkSessionsPlugin.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"
#include "OnlineGameInstance.generated.h"

/**
 * 
 */

 //TODO Week 9:
UCLASS()
class GAMEFRWKSESSIONSPLUGIN_API UOnlineGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UOnlineGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec)
	void Host(FString ServerName, FString Team) override;

	UFUNCTION(Exec)
	void Join(uint32 Index, FString Team) override;

	void StartSession();

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;
    
	 FString DesiredTeamNum;
	 
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;
   
	void CreateSession();
};
