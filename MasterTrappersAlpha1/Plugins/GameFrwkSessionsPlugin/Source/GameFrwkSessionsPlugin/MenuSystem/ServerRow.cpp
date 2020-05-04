// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"

#include "Components/Button.h"

#include "MainMenu.h"
//TODO Week 9: Setup this ServerRow widget and Subscribe tot the OnClicked Event of the RowButton with the OnClicked() Function
void UServerRow::Setup(class UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}
//TODO Week 9: The OnClicked() Callback function
void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}



