// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GAMEFRWKSESSIONSPLUGIN_ServerRow_generated_h
#error "ServerRow.generated.h already included, missing '#pragma once' in ServerRow.h"
#endif
#define GAMEFRWKSESSIONSPLUGIN_ServerRow_generated_h

#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnClicked) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->OnClicked(); \
		P_NATIVE_END; \
	}


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnClicked) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->OnClicked(); \
		P_NATIVE_END; \
	}


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUServerRow(); \
	friend struct Z_Construct_UClass_UServerRow_Statics; \
public: \
	DECLARE_CLASS(UServerRow, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameFrwkSessionsPlugin"), NO_API) \
	DECLARE_SERIALIZER(UServerRow)


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_INCLASS \
private: \
	static void StaticRegisterNativesUServerRow(); \
	friend struct Z_Construct_UClass_UServerRow_Statics; \
public: \
	DECLARE_CLASS(UServerRow, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameFrwkSessionsPlugin"), NO_API) \
	DECLARE_SERIALIZER(UServerRow)


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UServerRow(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UServerRow) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UServerRow); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UServerRow); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UServerRow(UServerRow&&); \
	NO_API UServerRow(const UServerRow&); \
public:


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UServerRow(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UServerRow(UServerRow&&); \
	NO_API UServerRow(const UServerRow&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UServerRow); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UServerRow); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UServerRow)


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__RowButton() { return STRUCT_OFFSET(UServerRow, RowButton); } \
	FORCEINLINE static uint32 __PPO__Parent() { return STRUCT_OFFSET(UServerRow, Parent); }


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_14_PROLOG
#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_PRIVATE_PROPERTY_OFFSET \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_RPC_WRAPPERS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_INCLASS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_PRIVATE_PROPERTY_OFFSET \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_INCLASS_NO_PURE_DECLS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_MenuSystem_ServerRow_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
