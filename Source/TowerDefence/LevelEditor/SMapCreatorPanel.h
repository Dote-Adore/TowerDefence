#pragma once
#include "TowerDefence/Entities/Base/Tile.h"
#include "Widgets/Views/STileView.h"

struct FMapTileEntry
{
	// 这里直接传引用，直接改源数据
	TSubclassOf<ABaseTile>& TileClass;
	int32 PathIndex;
	int32 TileIdx;
	bool ShowPath = false;
	FMapTileEntry(TSubclassOf<ABaseTile>& InTileClass, int32 InTileIdx)
		:TileClass(InTileClass), TileIdx(InTileIdx)
	{};
};

class ULevelInfomation;

class SMapCreatorPanel:public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMapCreatorPanel){}
	SLATE_END_ARGS()
typedef TSharedPtr<FMapTileEntry> FMapEditorItemType;
public:

	void Construct(const SMapCreatorPanel::FArguments& InArgs);
	void SetCurrentLevelInfo(ULevelInfomation* LevelInformation);
	//Wave 路径绘制
	void OnShowPath(TArray<int32>& InPath);



private:
	ULevelInfomation* CurrentLevelInfomation;
	TSharedPtr<SBorder> SNoSelectedTips;
	TArray<const ABaseTile*> AllBaseTileTypes;
	TSharedPtr<SListView<const ABaseTile*>> TileTypeSelectList;
	TSharedPtr<SBox> MapEditorOuterBox;
	TSharedPtr<STileView<FMapEditorItemType>> MapEditorTileView;
	TArray<FMapEditorItemType> CurrentMapEditorTiles;
	
	int32 EachTileSize = 50;
	int32 OuterBoxWidth;
	int32 OuterBoxHeight;
	TSubclassOf<ABaseTile> CurrentSelectionTileTypeClass;
	bool IsMouseBtnDown = false;
	
	void GetDefaultBaseTiles();
	void OnTileTypeSelectionChanged(const ABaseTile* InTime, ESelectInfo::Type Type);
	TSharedRef<ITableRow> OnGenerateTileTypesRow(const ABaseTile* InItem,
		const TSharedRef<STableViewBase>& TableViewBase);
	TSharedRef<ITableRow> OnGenerteTileMapItem(FMapEditorItemType InTileTypeItem,
		const TSharedRef<STableViewBase>& TableViewBase);
	
};
