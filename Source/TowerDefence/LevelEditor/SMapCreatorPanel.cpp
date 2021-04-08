#include "SMapCreatorPanel.h"



#include "Brushes/SlateColorBrush.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Entities/Base/Tile.h"
#include "Widgets/Colors/SColorBlock.h"

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
						.ListItemsSource(&AllBaseTiles)
						.Orientation(EOrientation::Orient_Horizontal)
						.OnGenerateRow(this, &SMapCreatorPanel::OnGenerateTileTypesRow)
						.SelectionMode(ESelectionMode::Single)
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
					SNew(SBox)
					.WidthOverride(EachTileSize*15)
					.HeightOverride(EachTileSize*4)
					.MaxDesiredWidth(EachTileSize*15)
					[
						SNew(SColorBlock)
						.Color(FLinearColor::Yellow)
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
	if(AllBaseTiles.Num()>0)
	{
		TileTypeSelectList->SetSelection(AllBaseTiles[0]);
	}
}

void SMapCreatorPanel::SetCurrentLevelInfo(ULevelInfomation* LevelInformation)
{
	CurrentLevelInfomation = LevelInformation;
	if(LevelInformation)
	{
	    SNoSelectedTips->SetVisibility(EVisibility::Collapsed);
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
		AllBaseTiles.Add(BaseTile);
	}
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
#undef LOCTEXT_NAMESPACE
