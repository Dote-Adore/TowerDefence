#include "SLevelCreatorPanel.h"
#include "SMapCreatorPanel.h"
#include "SLevelList.h"
#include "Brushes/SlateColorBrush.h"


#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SLevelCreatorPanel::Construct(const SLevelCreatorPanel::FArguments& InArgs)
{
	MapCreatorPanel = SNew(SMapCreatorPanel);
	ChildSlot
    [
        SNew(SHorizontalBox)
        // 所有的Level列表
        +SHorizontalBox::Slot()
        .AutoWidth()
        [
        	SNew(SBorder)
        	.Padding(2.f)
        	.BorderImage(&LevelPaddingBrush)
        	[
        		SNew(SVerticalBox)
        		+SVerticalBox::Slot()
       			.AutoHeight()
       			.HAlign(HAlign_Center)
       			[
       				SNew(SButton)
		            .Text(LOCTEXT("Create New Level", "Create New Level"))
        		]
        		// 所有的level列表在这里显示
        		+SVerticalBox::Slot()
       			.FillHeight(1)
       			[
       				SNew(SLevelList)
       				.OnSelectionChanged(this, &SLevelCreatorPanel::OnSelectionChangedFunc)
       			]
       		]
		]
		// 选择关卡地图编辑器，
		+SHorizontalBox::Slot()
		// .AutoWidth()
		[
			SNew(SBorder)
			.BorderImage(&MapCreatorBGBrush)
			[
				MapCreatorPanel.ToSharedRef()
			]
		]
		// 关卡信息编辑器
		// +SHorizontalBox::Slot()
		// [
		// 	
		// ]
	];
}

void SLevelCreatorPanel::OnSelectionChangedFunc(ULevelInfomation* LevelInfomation, ESelectInfo::Type Type)
{
	MapCreatorPanel->SetCurrentLevelInfo(LevelInfomation);
}
#undef LOCTEXT_NAMESPACE
