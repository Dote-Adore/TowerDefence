#pragma once
#include "TowerDefence/Entities/Base/Tile.h"
#include "Widgets/Views/STileView.h"


class ULevelInfomation;

class SMapCreatorPanel:public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMapCreatorPanel){}
	SLATE_END_ARGS()
public:

	void Construct(const SMapCreatorPanel::FArguments& InArgs);


	void SetCurrentLevelInfo(ULevelInfomation* LevelInformation);

private:
	ULevelInfomation* CurrentLevelInfomation;
	TSharedPtr<SBorder> SNoSelectedTips;
	TArray<const ABaseTile*> AllBaseTiles;
	TSharedPtr<SListView<const ABaseTile*>> TileTypeSelectList;
	TSharedPtr<SBox> MapEditorOuterBox;
	TSharedPtr<STileView<TSharedPtr<FString>>> MapEditorTileView;
	TArray<TSharedPtr<FString>> CurrentMapEditorTiles;
	
	int32 EachTileSize = 50;
	int32 OuterBoxWidth;
	int32 OuterBoxHeight;
	
	void GetDefaultBaseTiles();
	TSharedRef<ITableRow> OnGenerateTileTypesRow(const ABaseTile* InItem,
		const TSharedRef<STableViewBase>& TableViewBase);
	TSharedRef<ITableRow> OnGenerteTileMapItem(TSharedPtr<FString> InTile,
		const TSharedRef<STableViewBase>& TableViewBase);
};
