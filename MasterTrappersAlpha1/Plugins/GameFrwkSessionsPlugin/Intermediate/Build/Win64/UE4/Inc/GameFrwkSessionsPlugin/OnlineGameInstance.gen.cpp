// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameFrwkSessionsPlugin/OnlineGameInstance.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOnlineGameInstance() {}
// Cross Module References
	GAMEFRWKSESSIONSPLUGIN_API UClass* Z_Construct_UClass_UOnlineGameInstance_NoRegister();
	GAMEFRWKSESSIONSPLUGIN_API UClass* Z_Construct_UClass_UOnlineGameInstance();
	ENGINE_API UClass* Z_Construct_UClass_UGameInstance();
	UPackage* Z_Construct_UPackage__Script_GameFrwkSessionsPlugin();
	GAMEFRWKSESSIONSPLUGIN_API UFunction* Z_Construct_UFunction_UOnlineGameInstance_Host();
	GAMEFRWKSESSIONSPLUGIN_API UFunction* Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu();
	GAMEFRWKSESSIONSPLUGIN_API UFunction* Z_Construct_UFunction_UOnlineGameInstance_Join();
	GAMEFRWKSESSIONSPLUGIN_API UFunction* Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget();
	GAMEFRWKSESSIONSPLUGIN_API UClass* Z_Construct_UClass_UMenuInterface_NoRegister();
// End Cross Module References
	void UOnlineGameInstance::StaticRegisterNativesUOnlineGameInstance()
	{
		UClass* Class = UOnlineGameInstance::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "Host", &UOnlineGameInstance::execHost },
			{ "InGameLoadMenu", &UOnlineGameInstance::execInGameLoadMenu },
			{ "Join", &UOnlineGameInstance::execJoin },
			{ "LoadMenuWidget", &UOnlineGameInstance::execLoadMenuWidget },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UOnlineGameInstance_Host_Statics
	{
		struct OnlineGameInstance_eventHost_Parms
		{
			FString ServerName;
			FString Team;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Team;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ServerName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::NewProp_Team = { UE4CodeGen_Private::EPropertyClass::Str, "Team", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(OnlineGameInstance_eventHost_Parms, Team), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::NewProp_ServerName = { UE4CodeGen_Private::EPropertyClass::Str, "ServerName", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(OnlineGameInstance_eventHost_Parms, ServerName), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::NewProp_Team,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::NewProp_ServerName,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "OnlineGameInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UOnlineGameInstance, "Host", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00020601, sizeof(OnlineGameInstance_eventHost_Parms), Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::PropPointers), 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UOnlineGameInstance_Host()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UOnlineGameInstance_Host_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "OnlineGameInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UOnlineGameInstance, "InGameLoadMenu", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UOnlineGameInstance_Join_Statics
	{
		struct OnlineGameInstance_eventJoin_Parms
		{
			uint32 Index;
			FString Team;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Team;
		static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_Index;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::NewProp_Team = { UE4CodeGen_Private::EPropertyClass::Str, "Team", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(OnlineGameInstance_eventJoin_Parms, Team), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUInt32PropertyParams Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::NewProp_Index = { UE4CodeGen_Private::EPropertyClass::UInt32, "Index", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(OnlineGameInstance_eventJoin_Parms, Index), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::NewProp_Team,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::NewProp_Index,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "OnlineGameInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UOnlineGameInstance, "Join", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00020601, sizeof(OnlineGameInstance_eventJoin_Parms), Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::PropPointers), 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UOnlineGameInstance_Join()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UOnlineGameInstance_Join_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "OnlineGameInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UOnlineGameInstance, "LoadMenuWidget", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UOnlineGameInstance_NoRegister()
	{
		return UOnlineGameInstance::StaticClass();
	}
	struct Z_Construct_UClass_UOnlineGameInstance_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UOnlineGameInstance_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UGameInstance,
		(UObject* (*)())Z_Construct_UPackage__Script_GameFrwkSessionsPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UOnlineGameInstance_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UOnlineGameInstance_Host, "Host" }, // 159332934
		{ &Z_Construct_UFunction_UOnlineGameInstance_InGameLoadMenu, "InGameLoadMenu" }, // 934987483
		{ &Z_Construct_UFunction_UOnlineGameInstance_Join, "Join" }, // 3872339496
		{ &Z_Construct_UFunction_UOnlineGameInstance_LoadMenuWidget, "LoadMenuWidget" }, // 96762285
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UOnlineGameInstance_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "OnlineGameInstance.h" },
		{ "ModuleRelativePath", "OnlineGameInstance.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
		{ "ToolTip", "//TODO Week 9:" },
	};
#endif
		const UE4CodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_UOnlineGameInstance_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UMenuInterface_NoRegister, (int32)VTABLE_OFFSET(UOnlineGameInstance, IMenuInterface), false },
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UOnlineGameInstance_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOnlineGameInstance>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UOnlineGameInstance_Statics::ClassParams = {
		&UOnlineGameInstance::StaticClass,
		DependentSingletons, ARRAY_COUNT(DependentSingletons),
		0x001000A8u,
		FuncInfo, ARRAY_COUNT(FuncInfo),
		nullptr, 0,
		nullptr,
		&StaticCppClassTypeInfo,
		InterfaceParams, ARRAY_COUNT(InterfaceParams),
		METADATA_PARAMS(Z_Construct_UClass_UOnlineGameInstance_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UOnlineGameInstance_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UOnlineGameInstance()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UOnlineGameInstance_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UOnlineGameInstance, 3340359911);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UOnlineGameInstance(Z_Construct_UClass_UOnlineGameInstance, &UOnlineGameInstance::StaticClass, TEXT("/Script/GameFrwkSessionsPlugin"), TEXT("UOnlineGameInstance"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UOnlineGameInstance);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
