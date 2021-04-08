#include "SLevelCreatorPanel.h"
#include "SMapCreatorPanel.h"
#include "SLevelList.h"
#include "Brushes/SlateColorBrush.h"


#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SLevelCreatorPanel::Construct(const SLevelCreatorPanel::FArguments& InArgs)
{
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
        		+SVerticalBox::Slot()
       			.FillHeight(1)
       			[
       				SNew(SLevelList)
       			]
       		]
		]
		// 关卡地图编辑器
		+SHorizontalBox::Slot()
		[
			SNew(SBorder)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.BorderImage(&MapCreatorBGBrush)
			[
				SNew(SMapCreatorPanel)
			]
		]
		// 关卡信息编辑器
		// +SHorizontalBox::Slot()
		// [
		// 	
		// ]
	];
}

#undef LOCTEXT_NAMESPACE