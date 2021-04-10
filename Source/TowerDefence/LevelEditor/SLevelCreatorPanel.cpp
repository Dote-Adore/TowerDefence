#include "SLevelCreatorPanel.h"

#include "SInfoParamsPanel.h"
#include "SMapCreatorPanel.h"
#include "SLevelList.h"
#include "Brushes/SlateColorBrush.h"


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
	];
	
}
#undef LOCTEXT_NAMESPACE
