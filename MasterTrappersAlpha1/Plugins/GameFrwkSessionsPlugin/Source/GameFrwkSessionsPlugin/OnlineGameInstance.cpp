// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

//TODO Week 9: Set the MenuClass and InGameMenuClass from the loaded assets
UOnlineGameInstance::UOnlineGameInstance(const FObjectInitializer & ObjectInitializer)
{
	//WidgetBlueprint'/GameFrwkSessionsPlugin/MenuSystem/WBP_InGameMenu.WBP_InGameMenu'
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/GameFrwkSessionsPlugin/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/GameFrwkSessionsPlugin/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

//TODO Week 9: Get the Online System for this game and initializes the session Interface
void UOnlineGameInstance::Init()
{
    //DECLARE a variable called Subsystem of type IOnlineSubsystem* and assign it to the return value of IOnlineSubsystem::Get()
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    //IF the Subsystem is not null
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
        //ASSIGN the SessionInterface to the return value of Subsystem->GetSessionInterface()
		SessionInterface = Subsystem->GetSessionInterface();
        //IF SessionInterface.IsValid()
		if (SessionInterface.IsValid())
        {
            /*Subscribe to SessionInterface delegates, this time the function is called "AddUObject()", NOT "AddDynamic()"*/
            //SUBSCRIBE to the OnCreateSessionCompleteDelegates and pass in this, &UOnlineGameInstance::OnCreateSessionComplete
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnCreateSessionComplete);
            //SUBSCRIBE to the OnDestroySessionCompleteDelegates and pass in this, &UOnlineGameInstance::OnDestroySessionComplete
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnDestroySessionComplete);
            //SUBSCRIBE to the OnFindSessionsCompleteDelegates and pass in this, &UOnlineGameInstance::OnFindSessionsComplete
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnFindSessionsComplete);
            //SUBSCRIBE to the OnJoinSessionCompleteDelegates and pass in this, &UOnlineGameInstance::OnJoinSessionComplete
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnJoinSessionComplete);
		}
        //ENDIF
	}
    //ELSE
	else 
    {
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}
    //ENDIF
}

//TODO Week 9: Initialize and Load the Menu Widget
void UOnlineGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;

    //CREATE the Menu by Assigning it to the return value of CreateWidget<UMainMenu>(this, MenuClass);
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

    //CALL Setup on Menu
	Menu->Setup();

    //CALL SetMenuInterface() on the Menu and pass in this
	Menu->SetMenuInterface(this);
}

//TODO Week 9: Initialize and Setup the InGameMenu widget
void UOnlineGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
    //DECLARE a variable of type UMenuWidget* called Menu and assign it to the retuen value of CreateWidget<UMenuWidget>(this, InGameMenuClass)
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	
    if (!ensure(Menu != nullptr)) return;

    //CALL Setup() on Menu
	Menu->Setup();

    //CALL SetMenuInterface() on Menu and pass in this
	Menu->SetMenuInterface(this);
}

//TODO Week 9: Host a game session
void UOnlineGameInstance::Host(FString ServerName, FString Team)
{
    //ASSIGN DesiredServerName to ServerName
	DesiredServerName = ServerName;
	DesiredTeamNum = Team;
    //IF SessionInterface.IsValid()
    if (SessionInterface.IsValid())
	{
        /*Check if there is a existing session and destroy it*/
        //DECLARE a auto variable called ExistingSession and assign it to the return value of SessionInterface->GetNamedSession(SESSION_NAME)
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
        //IF ExistingSession NOT equal to nullptr
		if (ExistingSession != nullptr) 
		{
            /*THis will trigger the OnDestroySessionComplete(...) which will then call CreateSession()*/
            //CALL DestroySession on SessionInterface and pass in SESSION_NAME
			SessionInterface->DestroySession(SESSION_NAME);
		}
        //ELSE
		else
		{
            //CALL CreateSession()
			CreateSession();
		}
        //ENDIF
	}
    //ENDIF
}

//TODO Week 9: Destroy a Game Session and Create a new one
void UOnlineGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
    //IF Success
	if (Success) 
    {
        //CALL CreateSession()
		CreateSession();
	}
    //ENDIF
}

//TODO Week 9: Create a new Game Session
void UOnlineGameInstance::CreateSession()
{
    //IF SessionInterface.IsValid()
	if (SessionInterface.IsValid()) 
    {
        //DECLARE a variable called SessionSettings of type FOnlineSessionSettings
		FOnlineSessionSettings SessionSettings;
        //DECLARE a variable called name and assign it to the return value of IOnlineSubsystem::Get()->GetSubsystemName()
		FName name = IOnlineSubsystem::Get()->GetSubsystemName();
        /*We will always be in a lan match, this will be something else if the subsystem is steam, XBox Live, PSN ....*/
        //IF the name.IsEqual("NULL")
		if (name.IsEqual("NULL"))
		{
            //ASSIGN bIsLANMatch in SessionSettings to true
			SessionSettings.bIsLANMatch = true;
		}
        //ELSE
		else
		{   
            //ASSIGN bIsLANMatch in SessionSettings to false
			SessionSettings.bIsLANMatch = false;
		}
        //ENDIF
        //ASSIGN NumPublicConnections in SessionSettings to 5 /** The number of publicly available connections advertised */
		SessionSettings.NumPublicConnections = 5;
        //ASSIGN bShouldAdvertise in SessionSettings to true /**this will advertise whether this match is advertized on the online service*/
		SessionSettings.bShouldAdvertise = true;
        //ASSIGN bUsesPresence in SessionSettings to true /** Whether to display user presence information or not */
		SessionSettings.bUsesPresence = true;
        //CALL Set on SessionSettings and pass in SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

        //CALL CreateSession() on the SessionInterface and pass in 0, SESSION_NAME, SessionSettings
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
    //ENDIF
}

//TODO Week 9: Handle/Callback when a Session Creation is Complete
void UOnlineGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

    //IF Menu NOT EQUAL to nullptr
	if (Menu != nullptr)
	{
        //CALL Teardown() on Menu
		Menu->Teardown();
	}
    //ENDIF

    //DECLARE a variable callled Engine of type UEngine* and assign it to the return of GetEngine()
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    //DECLARE a variable called World of type UWorld* and assign it to the return value of GetWorld()
	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	//World'/GameFrwkSessionsPlugin/Maps/Lobby.Lobby'
    /*Open the Lobby Level in listen mode*/
    //CALL ServerTravel() on the World and pass in the following string "/GameFrwkSessionsPlugin/Maps/Lobby?listen"
	World->ServerTravel("/GameFrwkSessionsPlugin/Maps/Lobby?listen");
}

//TODO Week 9: Refresh the List of Servers
void UOnlineGameInstance::RefreshServerList()
{
    //ASSIGN the SessionSearch variable to the return value of MakeShareable(new FOnlineSessionSearch())
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
    //IF SessionSearch.IsValid()
	if (SessionSearch.IsValid())
	{
        //ASSIGN bIsLanQuery in SessionSearch to true /** Whether the query is intended for LAN matches or not */
		SessionSearch->bIsLanQuery = true;
        //ASSIGN MaxSearchResults in SessionSearch to 100 /** Max number of queries returned by the matchmaking service */
		SessionSearch->MaxSearchResults = 100;
        //CALL SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals) /**Sets a key value pair combination that defines a search parameter*/
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
        //CALL FindSessions() on the SessionInterface and pass in 0, SessionSearch.ToSharedRef() 
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
    //ENDIF
}

//TODO Week 9: Handle/Callback when FInding Sessions has completed
void UOnlineGameInstance::OnFindSessionsComplete(bool Success)
{
    //IF Success AND SessionSearch.IsValid() AND Menu NOT EQUAL nullptr
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));
        //DECLARE a TArray of type FServerData called ServerNames
		TArray<FServerData> ServerNames;
        /*Loop through the found sessions and create server data for each found session*/
        //FOR ( const FOnlineSessionSearchResult& SearchResult IN SessionSearch->SearchResults )
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
            //DECLARE a variable called Data of type FServerData
			FServerData Data;
            //ASSIGN MaxPlayers in Data to SearchResult.Session.SessionSettings.NumPublicConnections
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            //ASSIGN CurrentPlayers in Data to Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections 
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
            //ASSIGN HostUsername in Data to SearchResult.Session.OwningUserName
			Data.HostUsername = SearchResult.Session.OwningUserName;

            //DECLARE a FString called ServerName
			FString ServerName;
            //IF SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName) /*Gets the Server name from the Session's Settings*/
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
                //ASSIGN Name in Data to ServerName
				Data.Name = ServerName;
			}
            //ELSE
			else
			{
                //ASSIGN Name in Data to "Could not find name."
				Data.Name = "Could not find name.";
			}
            //ENDIF

            //CALL Add in ServerNames and pass in Data
			ServerNames.Add(Data);
		}
        //ENDFOR

        //CALL SetServerList() on Menu and pass in ServerNames
		Menu->SetServerList(ServerNames);
	}
    //ENDIF
}

//TODO Week 9: Join a session
void UOnlineGameInstance::Join(uint32 Index, FString Team)
{
    DesiredTeamNum = Team;
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

    //CALL JoinSession in SessionInterface and pass in  0, SESSION_NAME, SessionSearch->SearchResults[Index]
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

//TODO Week 9: Handle?Callback when Joining a session has completed
void UOnlineGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
    /*Look at the documentation for GetResolvedConnectString() or go into OnlineSessionInterface.h and look up this function*/
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
    
    /*This will transfer the client to the network address of where the session is being hosted*/
    //CALL ClientTravel() on the PlayerController and pass in Address, ETravelType::TRAVEL_Absolute
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

//TODO Week 9: Start a Session
void UOnlineGameInstance::StartSession()
{
    //IF SessionInterface.IsValid()
	if (SessionInterface.IsValid())
	{
        //CALL StartSession() on SessionInterface and pass in SESSION_NAME
		SessionInterface->StartSession(SESSION_NAME);
	}
    //ENDIF
}

//TODO Week 9: Load the Main Menu
void UOnlineGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	//World'/GameFrwkSessionsPlugin/MenuSystem/MainMenu.MainMenu'
    /*Open the MainMenu Level Locally*/
    //CALL ClientTravel() on the PlayerController and pass in "/GameFrwkSessionsPlugin/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute
	PlayerController->ClientTravel("/GameFrwkSessionsPlugin/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
