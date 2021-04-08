#pragma once
#include "TowerDefence/Entities/Base/Tile.h"


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

	int32 EachTileSize = 50.f;
	
	
	void GetDefaultBaseTiles();
	TSharedRef<ITableRow> OnGenerateTileTypesRow(const ABaseTile* InItem,
		const TSharedRef<STableViewBase>& TableViewBase);
};
