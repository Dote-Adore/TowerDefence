#include "SLevelCreatorPanel.h"


#include "Editor.h"
#include "SInfoParamsPanel.h"
#include "SMapCreatorPanel.h"
#include "SLevelList.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Level/LevelInfomation.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SWindow.h"

#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SLevelCreatorPanel::Construct(const SLevelCreatorPanel::FArguments& InArgs)
{
	MapCreatorPanel = SNew(SMapCreatorPanel);
	ChildSlot
    [
        SAssignNew(MainHorizontalBox, SHorizontalBox)
        // 所有的Level列表
        +SHorizontalBox::Slot()
        .AutoWidth()
        [
        	SNew(SBorder)
        	.Padding(4.f)
        	.BorderImage(&LevelPaddingBrush)
        	[
        		SNew(SVerticalBox)
        		+SVerticalBox::Slot()
       			.AutoHeight()
       			.HAlign(HAlign_Center)
       			[
       				SNew(SButton)
		            .Text(LOCTEXT("Create New Level", "Create New Level"))
		            .OnClicked(this, &SLevelCreatorPanel::OnCreateNewLevelBtnClicked)
        		]
        		// 所有的level列表在这里显示
        		+SVerticalBox::Slot()
       			.FillHeight(1)
       			[
       				SAssignNew(LevelListWidget, SLevelList)
       				.OnSelectionChanged(this, &SLevelCreatorPanel::OnSelectionChangedFunc)
       			]
       		]
		]
		// 选择关卡地图编辑器，
		+SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SBorder)
			.BorderImage(&MapCreatorBGBrush)
			[
				MapCreatorPanel.ToSharedRef()
			]
		]
		// 关卡信息编辑器
		 +SHorizontalBox::Slot()
		 .AutoWidth()
		 [
		 	SAssignNew(InfoParamContainterWidget, SBorder)
		 ]
	];
}

void SLevelCreatorPanel::OnSelectionChangedFunc(ULevelInfomation* LevelInfomation, ESelectInfo::Type Type)
{
	MapCreatorPanel->SetCurrentLevelInfo(LevelInfomation);
	MainHorizontalBox->RemoveSlot(InfoParamContainterWidget.ToSharedRef());
	MainHorizontalBox->AddSlot()
	.AutoWidth()
	[
		SAssignNew(InfoParamContainterWidget, SInfoParamsPanel, LevelInfomation)
		.OnShowPath_Lambda([&](TArray<int32>& InPath)->void
		{
			MapCreatorPanel->OnShowPath(InPath);
			// UE_LOG(LogTemp, Display, TEXT("OnShowPath"))
		})
		.OnRedrawPath_Lambda([&](TArray<int32>* InPath, bool EnableRedrawPath)->void
		{
			MapCreatorPanel->OnRedrawPath(InPath, EnableRedrawPath);
		})
	];
	
}

FReply SLevelCreatorPanel::OnCreateNewLevelBtnClicked()
{
	TSharedPtr<SWindow> NewLevelWindow ;
	SAssignNew(NewLevelWindow, SWindow)
	.Title(LOCTEXT("CreateNewLevelWindow", "Create New Level"))
	.ClientSize(FVector2D(300, 120))
	.Content()
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10, 5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(1)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("New Level Name","New Level Name:"))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1)
			[
				SNew(SEditableTextBox)
				.OnTextChanged_Lambda([&](const FText& InText)->void
				{
					NewLevelName = InText.ToString();
				})
			]
		]
		+SVerticalBox::Slot()
		.Padding(10, 5)
		.AutoHeight()
		[
		    SNew(SHorizontalBox)
		    +SHorizontalBox::Slot()
		    .FillWidth(1)
		    [
		        SNew(STextBlock)
		        .Text(LOCTEXT("New Map Width","Map Width:"))
		    ]
		    +SHorizontalBox::Slot()
		    .FillWidth(1)
		    [
		        SNew(SEditableTextBox)
		        .OnTextChanged_Lambda([&](const FText& InText)->void
		        {
			        if(InText.IsNumeric())
			        {
				        NewWidth = FCString::Atoi(*InText.ToString());
			        }
		        })
		    ]
		]
		+SVerticalBox::Slot()
		.Padding(10, 5)
		.AutoHeight()
		[
		    SNew(SHorizontalBox)
		    +SHorizontalBox::Slot()
		    .FillWidth(1)
		    [
		        SNew(STextBlock)
		        .Text(LOCTEXT("New Map Height","Map Height:"))
		    ]
		    +SHorizontalBox::Slot()
		    .FillWidth(1)
		    [
		        SNew(SEditableTextBox)
		        .OnTextChanged_Lambda([&](const FText& InText)->void
               {
                   if(InText.IsNumeric())
                   {
                       NewHeight = FCString::Atoi(*InText.ToString());
                   }
               })
		    ]
		]
		+SVerticalBox::Slot()
		.Padding(10, 5)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			[
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
				.Text(LOCTEXT("Confirm Create Btn", "Create"))
               .ForegroundColor(FSlateColor::UseForeground())
               .OnClicked_Lambda([&]()->FReply
               {
               		FString PackageName = GetDefault<UGlobalConfig>()->TargetLevelsPath + "/" + NewLevelName;
               		UPackage* Package = CreatePackage(*PackageName);
               		ULevelInfomation* NewLevelInfomation =
               			NewObject<ULevelInfomation>(Package, ULevelInfomation::StaticClass(),
               				*NewLevelName, RF_Public|RF_Standalone);
               		NewLevelInfomation->TileInfo.Height = NewHeight;
               		NewLevelInfomation->TileInfo.Witdh = NewWidth;
               		FAssetRegistryModule::AssetCreated(NewLevelInfomation);
               		NewLevelInfomation->MarkPackageDirty();
               		FString ContentPath = FPaths::ProjectContentDir();
               		FString TargetFilePath = PackageName.Replace(*FString("/Game/"), *ContentPath, ESearchCase::IgnoreCase);
               		TargetFilePath = FString::Printf(TEXT("%s%s"), *TargetFilePath, *FPackageName::GetAssetPackageExtension());
               		bool bSuccess = UPackage::SavePackage(Package, NewLevelInfomation,
               			RF_Public|RF_Standalone, *TargetFilePath);
               		NewLevelWindow->RequestDestroyWindow();
               		LevelListWidget->RefreshList();
               		LevelListWidget->SetSelection(NewLevelInfomation);
               		return FReply::Handled();
               })
			]
			+SHorizontalBox::Slot()
            .HAlign(HAlign_Right)
            [
                SNew(SButton)
                .Text(LOCTEXT("Cancel Create Btn", "Cancel"))
                .ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
               .ForegroundColor(FSlateColor::UseForeground())
               .OnClicked_Lambda([&]()->FReply
               {
               		NewLevelWindow->RequestDestroyWindow();
               		return FReply::Handled();
               })
            ]
			
		]
	];
	GEditor->EditorAddModalWindow(NewLevelWindow.ToSharedRef());
	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
