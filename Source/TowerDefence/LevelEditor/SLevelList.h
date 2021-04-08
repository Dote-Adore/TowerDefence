#pragma once


class ULevelInfomation;
class SLevelList:public SCompoundWidget
{
public:

	using FOnSelectionChanged = typename TSlateDelegates<ULevelInfomation*>::FOnSelectionChanged;
	
	SLATE_BEGIN_ARGS(SLevelList){}
	
	SLATE_EVENT(FOnSelectionChanged, OnSelectionChanged)

	SLATE_END_ARGS()

	void Construct(const SLevelList::FArguments& InArgs);
private:
	TArray<class ULevelInfomation*> ObjectArray;
	FOnSelectionChanged OnSelectionChanged;
	
	
	TSharedRef<class ITableRow> OnGenerateRow(ULevelInfomation* InItem,
		const TSharedRef< class STableViewBase >& TableViewBase);
	void OnSelectionChangedFunc(ULevelInfomation* LevelInfomation, ESelectInfo::Type Type);
};
