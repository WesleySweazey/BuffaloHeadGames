// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameFrwkSessionsPlugin/MenuSystem/InGameMenu.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInGameMenu() {}
// Cross Module References
	GAMEFRWKSESSIONSPLUGIN_API UClass* Z_Construct_UClass_UInGameMenu_NoRegister();
	GAMEFRWKSESSIONSPLUGIN_API UClass* Z_Construct_UClass_UInGameMenu();
	GAMEFRWKSESSIONSPLUGIN_API UClass* Z_Construct_UClass_UMenuWidget();
	UPackage* Z_Construct_UPackage__Script_GameFrwkSessionsPlugin();
	GAMEFRWKSESSIONSPLUGIN_API UFunction* Z_Construct_UFunction_UInGameMenu_CancelPressed();
	GAMEFRWKSESSIONSPLUGIN_API UFunction* Z_Construct_UFunction_UInGameMenu_QuitPressed();
	UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
// End Cross Module References
	void UInGameMenu::StaticRegisterNativesUInGameMenu()
	{
		UClass* Class = UInGameMenu::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CancelPressed", &UInGameMenu::execCancelPressed },
			{ "QuitPressed", &UInGameMenu::execQuitPressed },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UInGameMenu_CancelPressed_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UInGameMenu_CancelPressed_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "MenuSystem/InGameMenu.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UInGameMenu_CancelPressed_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UInGameMenu, "CancelPressed", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00040401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UInGameMenu_CancelPressed_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UInGameMenu_CancelPressed_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UInGameMenu_CancelPressed()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UInGameMenu_CancelPressed_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UInGameMenu_QuitPressed_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UInGameMenu_QuitPressed_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "MenuSystem/InGameMenu.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UInGameMenu_QuitPressed_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UInGameMenu, "QuitPressed", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00040401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UInGameMenu_QuitPressed_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UInGameMenu_QuitPressed_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UInGameMenu_QuitPressed()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UInGameMenu_QuitPressed_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UInGameMenu_NoRegister()
	{
		return UInGameMenu::StaticClass();
	}
	struct Z_Construct_UClass_UInGameMenu_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QuitButton_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_QuitButton;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CancelButton_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_CancelButton;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UInGameMenu_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UMenuWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_GameFrwkSessionsPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UInGameMenu_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UInGameMenu_CancelPressed, "CancelPressed" }, // 2429430512
		{ &Z_Construct_UFunction_UInGameMenu_QuitPressed, "QuitPressed" }, // 2263054248
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UInGameMenu_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MenuSystem/InGameMenu.h" },
		{ "ModuleRelativePath", "MenuSystem/InGameMenu.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UInGameMenu_Statics::NewProp_QuitButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "MenuSystem/InGameMenu.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UInGameMenu_Statics::NewProp_QuitButton = { UE4CodeGen_Private::EPropertyClass::Object, "QuitButton", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0040000000080008, 1, nullptr, STRUCT_OFFSET(UInGameMenu, QuitButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UInGameMenu_Statics::NewProp_QuitButton_MetaData, ARRAY_COUNT(Z_Construct_UClass_UInGameMenu_Statics::NewProp_QuitButton_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UInGameMenu_Statics::NewProp_CancelButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "MenuSystem/InGameMenu.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UInGameMenu_Statics::NewProp_CancelButton = { UE4CodeGen_Private::EPropertyClass::Object, "CancelButton", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0040000000080008, 1, nullptr, STRUCT_OFFSET(UInGameMenu, CancelButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UInGameMenu_Statics::NewProp_CancelButton_MetaData, ARRAY_COUNT(Z_Construct_UClass_UInGameMenu_Statics::NewProp_CancelButton_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UInGameMenu_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UInGameMenu_Statics::NewProp_QuitButton,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UInGameMenu_Statics::NewProp_CancelButton,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UInGameMenu_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UInGameMenu>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UInGameMenu_Statics::ClassParams = {
		&UInGameMenu::StaticClass,
		DependentSingletons, ARRAY_COUNT(DependentSingletons),
		0x00B010A0u,
		FuncInfo, ARRAY_COUNT(FuncInfo),
		Z_Construct_UClass_UInGameMenu_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UClass_UInGameMenu_Statics::PropPointers),
		nullptr,
		&StaticCppClassTypeInfo,
		nullptr, 0,
		METADATA_PARAMS(Z_Construct_UClass_UInGameMenu_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UInGameMenu_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UInGameMenu()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UInGameMenu_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UInGameMenu, 3206424048);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UInGameMenu(Z_Construct_UClass_UInGameMenu, &UInGameMenu::StaticClass, TEXT("/Script/GameFrwkSessionsPlugin"), TEXT("UInGameMenu"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UInGameMenu);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
