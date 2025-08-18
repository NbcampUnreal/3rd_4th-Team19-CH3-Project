#include "KHY/NPC/ShopNPC.h"
#include "Blueprint/UserWidget.h"

AShopNPC::AShopNPC()
	:
	ShopWidgetClass(nullptr),
	ShopWidgetInstance(nullptr)
{
}

void AShopNPC::ShowShop()
{
	HideShop();

	if (ShopWidgetClass)
	{
		ShopWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);
		if (ShopWidgetInstance)
		{
			ShopWidgetInstance->AddToViewport();
		}
	}
}

void AShopNPC::HideShop()
{
	if (ShopWidgetInstance)
	{
		ShopWidgetInstance->RemoveFromParent();
		ShopWidgetInstance = nullptr;
	}
}

void AShopNPC::Interact(AActor* Interactor)
{
	ShowShop();
}
