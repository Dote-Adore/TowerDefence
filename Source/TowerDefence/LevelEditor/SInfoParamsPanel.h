#pragma once
#include "Brushes/SlateColorBrush.h"

struct FEnemyGenerationInfo;
class ULevelInfomation;
//  每一个关卡文件属性的编辑
struct FWaveWidgetItem
{
	FEnemyGenerationInfo& WaveItem;
	int32 Idx;
	FWaveWidgetItem(FEnemyGenerationInfo& InWaveIntem, int32 InIdx)
	:WaveItem(InWaveIntem),
	Idx((InIdx))
	{}
};

DECLARE_DELEGATE_OneParam(FOnShowPath, TArray<int32>&)
class SInfoParamsPanel:public SCompoundWidget
{
	typedef TSharedPtr<FWaveWidgetItem> FWaveItemEntry;
	SLATE_BEGIN_ARGS(SInfoParamsPanel){}
	SLATE_EVENT(FOnShowPath, OnShowPath)
	SLATE_END_ARGS()
public:
	void Construct(const SInfoParamsPanel::FArguments& InArgs, ULevelInfomation*LevelInfomation);

private:
	FOnShowPath OnShowPath;
	ULevelInfomation* CurrentLevelInfomation;
	TSharedPtr<SComboButton> BGSelectorSelctorButton;
	TArray<FWaveItemEntry> CurrentWaveItemArray;
	TSharedPtr<SVerticalBox> MainVerticalBoxPanel;
	TSharedPtr<SWidget> EachWaveSettingWidget;

	// Wave Params
	TSharedPtr<SListView<FWaveItemEntry>> WaveSelectorList;
	FWaveItemEntry CurrentSelectedWaveItem;
	FSlateColorBrush WaveListItemBorderBrush = FSlateColorBrush(FLinearColor(0,0,0,0));
	
	// ----------

	void OnBackgroundLevelSelected(const FAssetData& AssetData);
	TSharedRef<ITableRow> OnGenerateWaveListItemRow(FWaveItemEntry InWaveItem,
		const TSharedRef<STableViewBase>& TableViewBase);
	bool OnBackgroundIsAssetAcceptableForDrop(const UObject* InObject);
	TSharedRef<SWidget> OnGetBGSelectorMenuContent();
	void OnBGSelectorMenuOpenChanged(bool bOpen);

	// Wave Editor
	void OnWaveItemListSelectedChanged(FWaveItemEntry InItem, ESelectInfo::Type Type);
	void InitWaveArray();
	FReply OnCreateNewWaveBtnClicked();
	FReply OnDeleteSelectedWaveBtnClicked();
};
