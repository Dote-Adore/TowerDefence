#include "SLevelList.h"
#include "TowerDefence/Level/LevelInfomation.h"
#include "TowerDefence/GlobalConfig.h"

#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SLevelList::Construct(const SLevelList::FArguments& InArgs)
{
	OnSelectionChanged = InArgs._OnSelectionChanged;
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	auto AllLevels = Config->GetAllLevels();
	
	AllLevels.GenerateValueArray(ObjectArray);
	ChildSlot
	[
		SAssignNew(ListView, SListView<ULevelInfomation*>)
		.ListItemsSource(&ObjectArray)
		.OnGenerateRow(this, &SLevelList::OnGenerateRow)
		.SelectionMode(ESelectionMode::Single)
		.OnSelectionChanged(this, &SLevelList::OnSelectionChangedFunc)
		.ClearSelectionOnClick(false)
	];
}

void SLevelList::RefreshList()
{
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	auto AllLevels = Config->GetAllLevels();
	AllLevels.GenerateValueArray(ObjectArray);
	ListView->RequestListRefresh();	
}

void SLevelList::SetSelection(ULevelInfomation* TargetLevel)
{
	ListView->SetSelection(TargetLevel);
}

TSharedRef<ITableRow> SLevelList::OnGenerateRow(ULevelInfomation* InItem,
                                                const TSharedRef<STableViewBase>& TableViewBase)
{
	return SNew(STableRow<ULevelInfomation*>,TableViewBase)
	[
		SNew(STextBlock)
		.Text(FText::FromString(InItem->GetName()))
	];
}

void SLevelList::OnSelectionChangedFunc(ULevelInfomation* LevelInfomation, ESelectInfo::Type Type)
{
	OnSelectionChanged.ExecuteIfBound(LevelInfomation, Type);
}

#undef LOCTEXT_NAMESPACE
