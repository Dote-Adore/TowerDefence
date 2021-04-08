#pragma once


class ULevelInfomation;
class SLevelList:public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelList){}
	SLATE_END_ARGS()

	void Construct(const SLevelList::FArguments& InArgs);
private:
	TArray<class ULevelInfomation*> ObjectArray;

	TSharedRef<class ITableRow> OnGenerateRow(ULevelInfomation* InItem,  const TSharedRef< class STableViewBase >& TableViewBase);
};
