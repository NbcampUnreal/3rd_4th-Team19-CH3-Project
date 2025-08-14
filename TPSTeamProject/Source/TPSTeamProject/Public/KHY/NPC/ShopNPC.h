// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KHY/NPC/BaseNPC.h"
#include "ShopNPC.generated.h"


UCLASS()
class TPSTEAMPROJECT_API AShopNPC : public ABaseNPC
{
	GENERATED_BODY()
public:
	AShopNPC();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UUserWidget> ShopWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	UUserWidget* ShopWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void ShowShop();
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void HideShop();

	virtual void Interact(AActor* Interactor) override;
};
