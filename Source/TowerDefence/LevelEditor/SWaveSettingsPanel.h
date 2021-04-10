#pragma once
#include "Brushes/SlateColorBrush.h"
#include "Widgets/Views/STileView.h"

class SEditableTextBox;
struct FEnemyGenerationInfo;
class ULevelInfomation;
struct FGenerateIDWidgetItem
{
	int32& ItemRef;
	FGenerateIDWidgetItem(int32& InItem):
	ItemRef(InItem){}
};
class SWaveSettingsPanel:public SCompoundWidget
{
	typedef TSharedPtr<FGenerateIDWidgetItem> FGeneratedIDEntry;
	SLATE_BEGIN_ARGS(SWaveSettingsPanel){}
	SLATE_END_ARGS()
public:
	void Construct(const SWaveSettingsPanel::FArguments& InArgs,
		FEnemyGenerationInfo* EnemyGenerationInfo, ULevelInfomation* ParentLevelInfomation);
private:
	FEnemyGenerationInfo* CurrentWaveInfo;
	ULevelInfomation* CurrentLevelInfomation;
	TSharedPtr<SEditableTextBox> GrapTimeInputWidget;

	// Generated IDs
	TSharedPtr<STileView<FGeneratedIDEntry>> GeneratedIDListTileWidget;
	TArray<FGeneratedIDEntry> GeneratedIDArray;
	FSlateColorBrush GenerateIDItemBorderBrush = FSlateColorBrush(FLinearColor(0,0,0,0));
	// --------------
	
	void OnGrapTimeInputChanged(const FText& InText);


	// GeneratedIDs
	TSharedRef<ITableRow> OnGenerateIDTile(FGeneratedIDEntry InEntry, const TSharedRef<STableViewBase>& TableViewBase);
	void InitGenerateIDArray();
	FReply OnAddNewGeneratedIDBtnClicked();
	FReply OnDeleteSelectedIDBtnClicked();
	// --------------
};
