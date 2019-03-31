// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GAMEFRWKSESSIONSPLUGIN_OnlineGameInstance_generated_h
#error "OnlineGameInstance.generated.h already included, missing '#pragma once' in OnlineGameInstance.h"
#endif
#define GAMEFRWKSESSIONSPLUGIN_OnlineGameInstance_generated_h

#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execJoin) \
	{ \
		P_GET_PROPERTY(UUInt32Property,Z_Param_Index); \
		P_GET_PROPERTY(UStrProperty,Z_Param_Team); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->Join(Z_Param_Index,Z_Param_Team); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execHost) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_ServerName); \
		P_GET_PROPERTY(UStrProperty,Z_Param_Team); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->Host(Z_Param_ServerName,Z_Param_Team); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execInGameLoadMenu) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->InGameLoadMenu(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execLoadMenuWidget) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->LoadMenuWidget(); \
		P_NATIVE_END; \
	}


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execJoin) \
	{ \
		P_GET_PROPERTY(UUInt32Property,Z_Param_Index); \
		P_GET_PROPERTY(UStrProperty,Z_Param_Team); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->Join(Z_Param_Index,Z_Param_Team); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execHost) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_ServerName); \
		P_GET_PROPERTY(UStrProperty,Z_Param_Team); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->Host(Z_Param_ServerName,Z_Param_Team); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execInGameLoadMenu) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->InGameLoadMenu(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execLoadMenuWidget) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->LoadMenuWidget(); \
		P_NATIVE_END; \
	}


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUOnlineGameInstance(); \
	friend struct Z_Construct_UClass_UOnlineGameInstance_Statics; \
public: \
	DECLARE_CLASS(UOnlineGameInstance, UGameInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/GameFrwkSessionsPlugin"), NO_API) \
	DECLARE_SERIALIZER(UOnlineGameInstance) \
	virtual UObject* _getUObject() const override { return const_cast<UOnlineGameInstance*>(this); }


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_INCLASS \
private: \
	static void StaticRegisterNativesUOnlineGameInstance(); \
	friend struct Z_Construct_UClass_UOnlineGameInstance_Statics; \
public: \
	DECLARE_CLASS(UOnlineGameInstance, UGameInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/GameFrwkSessionsPlugin"), NO_API) \
	DECLARE_SERIALIZER(UOnlineGameInstance) \
	virtual UObject* _getUObject() const override { return const_cast<UOnlineGameInstance*>(this); }


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UOnlineGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UOnlineGameInstance) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UOnlineGameInstance); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UOnlineGameInstance); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UOnlineGameInstance(UOnlineGameInstance&&); \
	NO_API UOnlineGameInstance(const UOnlineGameInstance&); \
public:


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UOnlineGameInstance(UOnlineGameInstance&&); \
	NO_API UOnlineGameInstance(const UOnlineGameInstance&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UOnlineGameInstance); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UOnlineGameInstance); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UOnlineGameInstance)


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_PRIVATE_PROPERTY_OFFSET
#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_18_PROLOG
#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_PRIVATE_PROPERTY_OFFSET \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_RPC_WRAPPERS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_INCLASS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_PRIVATE_PROPERTY_OFFSET \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_INCLASS_NO_PURE_DECLS \
	Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Wesley_S_Final_Plugins_GameFrwkSessionsPlugin_Source_GameFrwkSessionsPlugin_OnlineGameInstance_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
