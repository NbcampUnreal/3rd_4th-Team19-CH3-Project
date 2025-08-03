#include "TPSTeamProjectEditorModule.h"
#include "CSVToDataTableToolSubsystem.h"
#include "DataTableToolSettings.h"

#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "FPlayfabTestEditorModule"

IMPLEMENT_MODULE(FTPSTeamProjectEditor, TPSTeamProjectEditor);

void FTPSTeamProjectEditor::StartupModule()
{
	ToolMenuExtensibilityManagerDelegateHandle = UToolMenus::RegisterStartupCallback(
		FSimpleDelegate::CreateRaw(this, &FTPSTeamProjectEditor::RegisterMenus)
	);
}

void FTPSTeamProjectEditor::ShutdownModule()
{
	UToolMenus::UnregisterOwner(TEXT("TPSTeamProjectEditorModule"));

	if (ToolMenuExtensibilityManagerDelegateHandle.IsValid())
	{
		UToolMenus::UnRegisterStartupCallback(ToolMenuExtensibilityManagerDelegateHandle);
		ToolMenuExtensibilityManagerDelegateHandle.Reset();
	}
}

void FTPSTeamProjectEditor::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(TEXT("TPSTeamProjectEditorModule"));

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	{
		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
		Section.AddSubMenu(
			"MyTool",
			LOCTEXT("MyToolMenuTitle", "My Tools"),
			LOCTEXT("MyToolsMenuTooltip", "Common custom editor tools for my project."),
			FNewMenuDelegate::CreateStatic(&FTPSTeamProjectEditor::RegisterMyToolsMenu)
		);
	}
}

void FTPSTeamProjectEditor::RegisterMyToolsMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Convert CSV to Data Tables"),
		LOCTEXT("ConvertCSVToDataTablesTooltip", "Converts all CSV files in the specified folder to Data Tables."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "ContentBrowser.ImportAsset"),
		FUIAction(FExecuteAction::CreateStatic(&FTPSTeamProjectEditor::OnConvertCSVToDataTablesClicked))
	);
}

void FTPSTeamProjectEditor::OnConvertCSVToDataTablesClicked()
{
	UCSVToDataTableToolSubsystem* Subsystem = GEditor->GetEditorSubsystem<UCSVToDataTableToolSubsystem>();
	if (Subsystem)
	{
		Subsystem->ConvertAllCSVsToDataTables();
	}
	else
	{
		FNotificationInfo Info(LOCTEXT("SubsystemNotFound", "CSV to DataTable Tool: Subsystem not found or not initialized!"));
		Info.ExpireDuration = 5.f;
		Info.Image = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Notification.Fail").GetIcon();
		FSlateNotificationManager::Get().AddNotification(Info);
		UE_LOG(LogTemp, Error, TEXT("UCSVToDataTableToolSubsystem not found or not initialized!"));
	}
}

#undef LOCTEXT_NAMESPACE

