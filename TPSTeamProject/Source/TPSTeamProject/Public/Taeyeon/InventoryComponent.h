#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

enum class EItemType : uint8;
struct FItemDataStruct;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot()
		:ItemName(NAME_None)
		, Amount(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSTEAMPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(FName ItemName, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemName, int32 Amount);

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;
	
	UFUNCTION(BlueprintPure)
	const TArray<FInventorySlot>& GetInventory() const { return Inventory; }

	UFUNCTION(BlueprintPure)
	const UDataTable* GetItemDataTable() const { return ItemDataTable.Get(); }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UDataTable> ItemDataTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 InventorySize = 20;

};
