#include "SInfoParamsPanel.h"

#include "PropertyCustomizationHelpers.h"
#include "SWaveSettingsPanel.h"
#include "TowerDefence/Level/LevelInfomation.h"
#include "EditorWidgets/Public/SAssetDropTarget.h"
#include "Styling/SlateStyle.h"

#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SInfoParamsPanel::Construct(const SInfoParamsPanel::FArguments& InArgs, ULevelInfomation*LevelInfomation)
{
	this->CurrentLevelInfomation = LevelInfomation;
	InitWaveArray();
	TSharedPtr<FTextBlockStyle> CategoryTextStyle =
        MakeShared<FTextBlockStyle>(FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>( "NormalText" ));
	CategoryTextStyle->SetFontSize(15);
	TSharedPtr<FTextBlockStyle> SubCategoryTextStyle =
    MakeShared<FTextBlockStyle>(FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>( "NormalText" ));
	SubCategoryTextStyle->SetFontSize(12);
	ChildSlot
    [
        SNew(SBorder)
        .Padding(10)
        [
            SAssignNew(MainVerticalBoxPanel, SVerticalBox)
            +SVerticalBox::Slot()
            .Padding(4)
            .AutoHeight()
            [
                SNew(STextBlock)
                .Text(LOCTEXT("BaseInfomation", "Base Infomation"))
                .TextStyle(CategoryTextStyle.Get())
            ]
            +SVerticalBox::Slot()
            .Padding(4)
            .AutoHeight()
            [
                SNew(STextBlock)
                .Text_Lambda([&]()->FText
                {
                    if(CurrentLevelInfomation)
                    {
                    return FText::Format(
                        LOCTEXT("Width Height Info", "Width:{0}    Height:{1}" ),
                        CurrentLevelInfomation->TileInfo.Witdh,
                        CurrentLevelInfomation->TileInfo.Height );
                    }
                    return FText();
                })
            ]
            +SVerticalBox::Slot()
            .Padding(4)
            .AutoHeight()
            [
                SNew(SHorizontalBox)
                +SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("Background Level Asset", "Background Level Asset: "))
                ]
                +SHorizontalBox::Slot()
                .AutoWidth()
                [
                        SAssignNew(BGSelectorSelctorButton, SComboButton)
                        .ButtonStyle( FEditorStyle::Get(), "PropertyEditor.AssetComboStyle" )
                        .ForegroundColor(FEditorStyle::GetColor("PropertyEditor.AssetName.ColorAndOpacity"))
                        .OnGetMenuContent( this, &SInfoParamsPanel::OnGetBGSelectorMenuContent)
                        .OnMenuOpenChanged( this, &SInfoParamsPanel::OnBGSelectorMenuOpenChanged)
                        .ContentPadding(2.0f)
                        .ButtonContent()
                        [
                            SNew(STextBlock)
                            .Text_Lambda([&]()->FText
                            {
                                if(!CurrentLevelInfomation->BackgroundWorld.IsNull())
                                	return FText::FromString(CurrentLevelInfomation->BackgroundWorld.GetAssetName());
                                else
                                	return FText::FromString("None");
                            })
                        ]
                    //.OnAssetDropped(this, )
                ]
            ]
            // 设置wave
			
            +SVerticalBox::Slot()
            .Padding(4)
            .AutoHeight()
            [
            SNew(STextBlock)
            .Text(LOCTEXT("WaveSettings", "Wave Settings"))
            .TextStyle(CategoryTextStyle.Get())
            ]
            +SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(STextBlock)
                .Text(LOCTEXT("AllWaveList", "All Waves"))
                .TextStyle(SubCategoryTextStyle.Get())
            ]
            +SVerticalBox::Slot()
            .Padding(4)
            .AutoHeight()
            [
            SNew(SBorder)
            .Padding(4.f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
            		SNew(SHorizontalBox)
            		+SHorizontalBox::Slot()
            		.Padding(4)
            		.AutoWidth()
            		.HAlign(HAlign_Left)
					[
						SNew(SButton)
						.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
						.Text(LOCTEXT("Add a new Wave", "Add a new Wave"))
						.ForegroundColor(FSlateColor::UseForeground())
						.OnClicked(this, &SInfoParamsPanel::OnCreateNewWaveBtnClicked)
            		]
            		+SHorizontalBox::Slot()
	                .AutoWidth()
	                .Padding(4)
	                .HAlign(HAlign_Right)
	                [
	                    SNew(SButton)
	                    .ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
	                    .Text(LOCTEXT("Delete a Selected Wave", "Delete A Selected Wave"))
	                    .ForegroundColor(FSlateColor::UseForeground())
	                    .OnClicked(this, &SInfoParamsPanel::OnDeleteSelectedWaveBtnClicked)
	                ]
				]
	            +SVerticalBox::Slot()
	            .AutoHeight()
	            [
	            	SAssignNew(WaveSelectorList, SListView<FWaveItemEntry>)
	            	.ListItemsSource(&CurrentWaveItemArray)
	            	.OnGenerateRow(this, &SInfoParamsPanel::OnGenerateWaveListItemRow)
	            	.SelectionMode(ESelectionMode::Single)
	            	.OnSelectionChanged(this, &SInfoParamsPanel::OnWaveItemListSelectedChanged)
	                ]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 4)
			[
			    SNew(STextBlock)
			    .Text(LOCTEXT("Current Selected Wave Settings", "Current Selected Wave Settings"))
			    .TextStyle(SubCategoryTextStyle.Get())
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(4,10)
			[
				SAssignNew(EachWaveSettingWidget, SBorder)
			]
        ]
    ];
	// 默认选中一个
	if(CurrentWaveItemArray.Num()>0)
	{
		WaveSelectorList->SetSelection(CurrentWaveItemArray[0]);
	}
}

TSharedRef<SWidget> SInfoParamsPanel::OnGetBGSelectorMenuContent()
{
	 return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		FAssetData(),
		false,
		{UWorld::StaticClass()},
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses({UWorld::StaticClass()}),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateSP(this, &SInfoParamsPanel::OnBackgroundLevelSelected),
		FSimpleDelegate());
}

void SInfoParamsPanel::OnBGSelectorMenuOpenChanged(bool bOpen)
{
	if ( bOpen == false )
	{
		BGSelectorSelctorButton->SetMenuContent(SNullWidget::NullWidget);
	}
}

void SInfoParamsPanel::OnWaveItemListSelectedChanged(FWaveItemEntry InItem, ESelectInfo::Type Type)
{
	MainVerticalBoxPanel->RemoveSlot(EachWaveSettingWidget.ToSharedRef());
	MainVerticalBoxPanel->AddSlot()
	.AutoHeight()
	[
		SAssignNew(EachWaveSettingWidget, SWaveSettingsPanel, &InItem->WaveItem, CurrentLevelInfomation)
	];
	CurrentSelectedWaveItem = InItem;
}

void SInfoParamsPanel::InitWaveArray()
{
	CurrentWaveItemArray.Empty();
	int32 i = 1;
	for(auto& InItem: CurrentLevelInfomation->Waves)
	{
		CurrentWaveItemArray.Add(MakeShared<FWaveWidgetItem>(FWaveWidgetItem(InItem, i)));
		i++;
	}
}

FReply SInfoParamsPanel::OnCreateNewWaveBtnClicked()
{
	// 在原来的文件添加一个波，并在目前的array加一个引用波
	FEnemyGenerationInfo NewEnemyGenerationInfo;
	CurrentLevelInfomation->Waves.Add(NewEnemyGenerationInfo);
	CurrentWaveItemArray.Add(MakeShared<FWaveWidgetItem>(
		FWaveWidgetItem(NewEnemyGenerationInfo, CurrentWaveItemArray.Num()+1)));
	CurrentLevelInfomation->MarkPackageDirty();
	WaveSelectorList->RequestListRefresh();
	return FReply::Handled();
}

FReply SInfoParamsPanel::OnDeleteSelectedWaveBtnClicked()
{
	if(!CurrentSelectedWaveItem.IsValid())
		return FReply::Handled();

	// 直接将原来的Remove掉,然后重新载入wave

	CurrentLevelInfomation->Waves.RemoveAt(CurrentSelectedWaveItem->Idx-1);
	InitWaveArray();
	WaveSelectorList->RequestListRefresh();
	if(CurrentLevelInfomation->Waves.Num()>0)
	{
		WaveSelectorList->SetSelection(CurrentWaveItemArray[0]);
	}
	return FReply::Handled();

}

void SInfoParamsPanel::OnBackgroundLevelSelected(const FAssetData& AssetData)
{
	UE_LOG(LogTemp, Display, TEXT("OnBackgroundLevelSelected"));
	CurrentLevelInfomation->BackgroundWorld = TSoftObjectPtr<UWorld>(AssetData.ToSoftObjectPath());
	CurrentLevelInfomation->MarkPackageDirty();
	BGSelectorSelctorButton->SetIsOpen(false);
}

TSharedRef<ITableRow> SInfoParamsPanel::OnGenerateWaveListItemRow(FWaveItemEntry InWaveItem,
	const TSharedRef<STableViewBase>& TableViewBase)
{
	return SNew(STableRow<FWaveItemEntry>, TableViewBase)
	[
		SNew(SBorder)
		.BorderImage(&WaveListItemBorderBrush)
		.Padding(2.f)
		[
			SNew(STextBlock)
			.Text(FText::Format(LOCTEXT("WaveItemDisplayName", "Wave{0}"),InWaveItem->Idx))
		]
	];
}

bool SInfoParamsPanel::OnBackgroundIsAssetAcceptableForDrop(const UObject* InObject)
{
	if(InObject->GetClass()->IsChildOf(UWorld::StaticClass()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

#undef LOCTEXT_NAMESPACE
