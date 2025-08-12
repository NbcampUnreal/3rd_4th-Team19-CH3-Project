// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableKey.h"
#include "ItemDataStruct.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Coin,
};

USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FItemDataStruct : public FTableRowBase, public ITableKey
{
	GENERATED_BODY()
	
public:
	FItemDataStruct()
		: Index(1)
		, ItemType(EItemType::Coin)
		, StackSize(0)
		, ItemIconTexture(nullptr)
		, WorldMesh(nullptr)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 StackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSoftObjectPtr<UTexture2D> ItemIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSoftObjectPtr<UStaticMesh> WorldMesh;

	virtual int32 GetKey() const override
	{
		return (Index << 16) + static_cast<int32>(ItemType);
	}
	
};
