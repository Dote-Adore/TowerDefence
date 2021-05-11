#include "SWaveSettingsPanel.h"
#include "TowerDefence/Level/LevelInfomation.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Views/STileView.h"
#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SWaveSettingsPanel::Construct(const SWaveSettingsPanel::FArguments& InArgs,
	FEnemyGenerationInfo* EnemyGenerationInfo, ULevelInfomation* ParentLevelInfomation)
{
	OnShowPath = InArgs._OnShowPath;
	OnRedrawPath = InArgs._OnRedrawPath;
	CurrentWaveInfo = EnemyGenerationInfo;
	CurrentLevelInfomation = ParentLevelInfomation;
	OnShowPath.ExecuteIfBound(CurrentWaveInfo->Path);
	InitGenerateIDArray();
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.Padding(0,5)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Grap Time", "Grap Time:"))
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
		    SAssignNew(GrapTimeInputWidget, SEditableTextBox)
		    .Text(FText::Format(LOCTEXT("Grap Input Text","{0}"), EnemyGenerationInfo->GrapTime))
		    .OnTextChanged(this, &SWaveSettingsPanel::OnGrapTimeInputChanged)
		]
		+SVerticalBox::Slot()
		.Padding(0,5)
		.AutoHeight()
		[
		   SNew(SHorizontalBox)
		   +SHorizontalBox::Slot()
		   .HAlign(HAlign_Left)
		   .AutoWidth()
		   [
		   		SNew(STextBlock)
				.Text(LOCTEXT("Generated IDs", "Generated IDs:"))
		   ]
		   +SHorizontalBox::Slot()
		   .FillWidth(1)
		   +SHorizontalBox::Slot()
		   .AutoWidth()
		   .Padding(5,0)
		   .HAlign(HAlign_Right)
		   [
				SNew(SButton)
                .ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
                .ForegroundColor(FSlateColor::UseForeground())
                .Text(LOCTEXT("Add new Generated ID", "+"))
                .OnClicked(this, &SWaveSettingsPanel::OnAddNewGeneratedIDBtnClicked)
		   ]
		   +SHorizontalBox::Slot()
           .AutoWidth()
           .HAlign(HAlign_Right)
           [
	           SNew(SButton)
	            .ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
	            .ForegroundColor(FSlateColor::UseForeground())
	            .Text(LOCTEXT("Delete Selected Generated ID", "-"))
	            .OnClicked(this, &SWaveSettingsPanel::OnDeleteSelectedIDBtnClicked)
           ]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(GeneratedIDListTileWidget, STileView<FGeneratedIDEntry>)
			.ItemWidth(60)
			.ItemHeight(25)
			.OnGenerateTile(this, &SWaveSettingsPanel::OnGenerateIDTile)
			.ListItemsSource(&GeneratedIDArray)
		]
		+SVerticalBox::Slot()
		.Padding(0,5)
		.AutoHeight()
		[
		    SNew(STextBlock)
		    .Text(LOCTEXT("Redraw Path", "Redraw Path"))
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0,5)
		[
			SNew(SCheckBox)
			.OnCheckStateChanged(this, &SWaveSettingsPanel::OnRedrawPathCheckStateChanged)
		]
		
	];
}

void SWaveSettingsPanel::OnGrapTimeInputChanged(const FText& InText)
{
	if(InText.IsNumeric())
	{
		float TargetInputFloat = FCString::Atof(*InText.ToString());
		CurrentWaveInfo->GrapTime = TargetInputFloat;
		CurrentLevelInfomation->MarkPackageDirty();
	}
	else
	{
		GrapTimeInputWidget->SetText(FText::Format(LOCTEXT("Grap Input Text","{0}"), CurrentWaveInfo->GrapTime));
	}
}

TSharedRef<ITableRow> SWaveSettingsPanel::OnGenerateIDTile(FGeneratedIDEntry InEntry, const TSharedRef<STableViewBase>& TableViewBase)
{
	return SNew(STableRow<FGeneratedIDEntry>, TableViewBase)
	[
		SNew(SBorder)
		.Padding(5)
		.BorderImage(&GenerateIDItemBorderBrush)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SEditableTextBox)
		        .Text_Lambda([InEntry]()->FText
		        {
		            return FText::FromString(FString::FromInt(InEntry->ItemRef));
		        })
		        // .Text(FText::Format(LOCTEXT("Each Generate ID","{0}"), InEntry.Get()))
		        .OnTextChanged_Lambda([&, InEntry](const FText& InText)->void
		        {
		            if(InText.IsNumeric())
		            {
		                int32 TargetInputInt = FCString::Atoi(*InText.ToString());
		                InEntry->ItemRef = TargetInputInt;
		                CurrentLevelInfomation->MarkPackageDirty();
		            }
		        })
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("NextSplit", "->"))
				.ColorAndOpacity(FLinearColor(1,1,1,1))
			]
		]
	];
}

void SWaveSettingsPanel::InitGenerateIDArray()
{
	GeneratedIDArray.Empty();
	for(int32& InIDItem: CurrentWaveInfo->GeneratedID)
	{
		GeneratedIDArray.Add(MakeShared<FGenerateIDWidgetItem>(FGenerateIDWidgetItem(InIDItem)));
	}
}

FReply SWaveSettingsPanel::OnAddNewGeneratedIDBtnClicked()
{
	int32 NewID = 0;
	CurrentWaveInfo->GeneratedID.Add(NewID);
	CurrentLevelInfomation->MarkPackageDirty();
	InitGenerateIDArray();
	GeneratedIDListTileWidget->RequestListRefresh();
	return FReply::Handled();
}

FReply SWaveSettingsPanel::OnDeleteSelectedIDBtnClicked()
{
	TArray<FGeneratedIDEntry> SelectedItems = GeneratedIDListTileWidget->GetSelectedItems();
	if(SelectedItems.Num()==0)
		return FReply::Handled();
	for(auto& EachItem:SelectedItems)
	{
		CurrentWaveInfo->GeneratedID.RemoveAll([EachItem](int32& TargetItem)->bool
		{
			if(&EachItem->ItemRef == &TargetItem)
				return true;
			return false;
		});
	}
	InitGenerateIDArray();
	GeneratedIDListTileWidget->RequestListRefresh();
	CurrentLevelInfomation->MarkPackageDirty();
	return FReply::Handled();
}

void SWaveSettingsPanel::OnRedrawPathCheckStateChanged(ECheckBoxState InNewState)
{
	if(InNewState == ECheckBoxState::Checked)
	{
		OnRedrawPath.ExecuteIfBound(&CurrentWaveInfo->Path, true);
	}
	else
	{
		OnRedrawPath.ExecuteIfBound(&CurrentWaveInfo->Path, false);
	}
}

#undef LOCTEXT_NAMESPACE
