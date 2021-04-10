#include "SMapCreatorPanel.h"

#include "Brushes/SlateColorBrush.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Entities/Base/Tile.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Views/STileView.h"
#include "TowerDefence/Level/LevelInfomation.h"
#define LOCTEXT_NAMESPACE "LevelCreatorEditor"


// 由几个部分组成：1.tile选择， 2.map大小
void SMapCreatorPanel::Construct(const SMapCreatorPanel::FArguments& InArgs)
{
	GetDefaultBaseTiles();
	SAssignNew(SNoSelectedTips, SBorder)
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SNew(STextBlock)
		.Text(LOCTEXT("None Selected Tip",
			"There is no Levels Selected, please selected one from Left List"))
	];
	
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNoSelectedTips.ToSharedRef()
		]
		+SOverlay::Slot()
		[
			SNew(SVerticalBox)
			// 显示所有的tile类型列表
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.Padding(2.f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Tiles Selected", "Selected Tiles:   "))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(TileTypeSelectList , SListView<const ABaseTile*>)
						.ListItemsSource(&AllBaseTileTypes)
						.Orientation(EOrientation::Orient_Horizontal)
						.OnGenerateRow(this, &SMapCreatorPanel::OnGenerateTileTypesRow)
						.SelectionMode(ESelectionMode::Single)
						.OnSelectionChanged(this, &SMapCreatorPanel::OnTileTypeSelectionChanged)
						.ClearSelectionOnClick(false)
					]
				]
			]
			+SVerticalBox::Slot()
			.FillHeight(1)
			// 地图主要显示、编辑功能界面
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(MapEditorOuterBox,SBox)
					.WidthOverride_Lambda([&]()->FOptionalSize
					{
						return FOptionalSize(OuterBoxWidth);
					})
					.HeightOverride_Lambda([&]()->FOptionalSize
                    {
                        return FOptionalSize(OuterBoxHeight);
                    })
					[
						SAssignNew(MapEditorTileView, STileView<FMapEditorItemType>)
						.ItemWidth(EachTileSize)
						.ItemHeight(EachTileSize)
						.OnGenerateTile(this, &SMapCreatorPanel::OnGenerteTileMapItem)
						.ListItemsSource(&CurrentMapEditorTiles)
						.SelectionMode(ESelectionMode::Single)
						.OnMouseButtonClick_Lambda([](FMapEditorItemType InItem)->void 
						{
							UE_LOG(LogTemp,Display, TEXT("On Map Item Click"));
						})
					]
				]
				+SHorizontalBox::Slot()
				.FillWidth(1)
			]
			+SVerticalBox::Slot()
			.FillHeight(1)
		]
	];
	// 默认选择第一个选项
	if(AllBaseTileTypes.Num()>0)
	{
		TileTypeSelectList->SetSelection(AllBaseTileTypes[0]);
	}
}

void SMapCreatorPanel::SetCurrentLevelInfo(ULevelInfomation* LevelInformation)
{
	CurrentLevelInfomation = LevelInformation;
	if(LevelInformation)
	{
	    SNoSelectedTips->SetVisibility(EVisibility::Collapsed);
		OuterBoxWidth = LevelInformation->TileInfo.Witdh*EachTileSize;
		OuterBoxHeight = LevelInformation->TileInfo.Height* EachTileSize;
		// 如果不符合大小，则全部删除重新建立一个
		int32 AllNum = LevelInformation->TileInfo.Witdh*LevelInformation->TileInfo.Height;
		CurrentMapEditorTiles.Empty();
		if(LevelInformation->TileInfo.Tiles.Num()!=AllNum)
		{
			LevelInformation->TileInfo.Tiles.Empty();
			for(int32 i = 0;i<AllNum;i++)
			{
				LevelInformation->TileInfo.Tiles.Add(nullptr);
				//CurrentMapEditorTiles.Add(MakeShared<FMapTileEntry>(FMapTileEntry("",i)));
			}
			LevelInformation->MarkPackageDirty();
		}
		for(int32 i = 0; i<AllNum;i++)
		{
			auto& InItem = LevelInformation->TileInfo.Tiles[i];
			CurrentMapEditorTiles.Add(
				MakeShared<FMapTileEntry>(InItem, i));
		}
		MapEditorTileView->RequestListRefresh();
	}
	else
	{
		SNoSelectedTips->SetVisibility(EVisibility::Visible);
	}
}

void SMapCreatorPanel::GetDefaultBaseTiles()
{
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	for(const auto BaseTileClass:Config->AllTiles)
	{
		const ABaseTile* BaseTile = GetDefault<ABaseTile>(BaseTileClass.Value);
		AllBaseTileTypes.Add(BaseTile);
	}
}

void SMapCreatorPanel::OnTileTypeSelectionChanged(const ABaseTile* InTime, ESelectInfo::Type Type)
{
	CurrentSelectionTileTypeClass = InTime->GetClass();
}

TSharedRef<ITableRow> SMapCreatorPanel::OnGenerateTileTypesRow(const ABaseTile* InItem,
                                                               const TSharedRef<STableViewBase>& TableViewBase)
{
	return SNew(STableRow<const ABaseTile*>, TableViewBase)
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2)
			[
				SNew(SColorBlock)
				.Color(InItem->DebugColor)
				.IgnoreAlpha(true)
				.Size(FVector2D(20.f, 20.f))
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(InItem->GetClass()->GetName().Left(InItem->GetClass()->GetName().Len()-2)))
			]
		]
	];
}

TSharedRef<ITableRow> SMapCreatorPanel::OnGenerteTileMapItem(FMapEditorItemType InTileTypeItem,
	const TSharedRef<STableViewBase>& TableViewBase)
{
	TSharedPtr<SWidget> ShowPathWidget;
	TSharedRef<SColorBlock> CurrentColorWidget =
		SNew(SColorBlock)
           .Color_Lambda([InTileTypeItem]()-> FLinearColor
           {
	           if(IsValid(InTileTypeItem->TileClass.Get()))
	           {
                   return GetDefault<ABaseTile>(*InTileTypeItem->TileClass)->DebugColor;
               }
               return FLinearColor(1,1,1,0.2);
           });
	
	TSharedRef<STableRow<FMapEditorItemType>> TargetMap = SNew(STableRow<FMapEditorItemType>, TableViewBase)
	[
		SNew(SBorder)
	    .OnMouseButtonDown_Lambda([&](const FGeometry&, const FPointerEvent&)-> FReply
	    {
	    	IsMouseBtnDown = true;
	        UE_LOG(LogTemp, Display, TEXT("OnMouseMove Down, nullptr"));
	        return FReply::Handled();
	    })
	    .OnMouseButtonUp_Lambda([&](const FGeometry&, const FPointerEvent&)-> FReply
        {
            IsMouseBtnDown = false;
            return FReply::Handled();
        })
	    .OnMouseMove_Lambda([&,InTileTypeItem](const FGeometry&, const FPointerEvent&)-> FReply
	    {
	    	// 如果一直是按下去的，则将当前的class改掉
	    	if(IsMouseBtnDown == true)
	    	{
	    		if(IsValid(InTileTypeItem->TileClass.Get())||
	    			InTileTypeItem->TileClass.Get() != CurrentSelectionTileTypeClass.Get())
	    		{
	    			InTileTypeItem->TileClass = CurrentSelectionTileTypeClass;
	    			CurrentLevelInfomation->MarkPackageDirty();
	    		}
	    	}
	        return FReply::Handled();
	    })
	    [
	    	SNew(SOverlay)
	    	+SOverlay::Slot()
	    	[
	    		CurrentColorWidget
	    	]
	    	+SOverlay::Slot()
	    	[
	    		//这后面是绘制path需要用到的UI
	    		SAssignNew(ShowPathWidget, SOverlay)
	    		.Visibility_Lambda([InTileTypeItem]()->EVisibility
	    		{
	    			if(!InTileTypeItem->ShowPath)
	    				return EVisibility::Collapsed;
	    			else
	    				return EVisibility::All;
	    		})
				+SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SOverlay)
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SColorBlock)
						.Size(EachTileSize-10)
						.Color(FLinearColor(0,0,0.6,0.3))
					]
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString::FromInt(InTileTypeItem->PathIndex)))
						.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
					]
				]
			]
	    ]
	];
	return TargetMap;
}

void SMapCreatorPanel::OnShowPath(TArray<int32>& InPath)
{
	// 首先绘制之前全部path清空
	for(auto InTile:CurrentMapEditorTiles)
	{
		InTile->ShowPath = false;
	}
	for(int32 i = 0; i < InPath.Num(); i++)
	{
		int32 TargetTileIdx = InPath[i];
		CurrentMapEditorTiles[TargetTileIdx]->ShowPath = true;
		CurrentMapEditorTiles[TargetTileIdx]->PathIndex = i;
	}
	MapEditorTileView->RequestListRefresh();
}
#undef LOCTEXT_NAMESPACE
