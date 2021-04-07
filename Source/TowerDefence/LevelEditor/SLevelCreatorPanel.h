#pragma once

class SLevelCreatorPanel:public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelCreatorPanel){}
	SLATE_END_ARGS()

	void Construct(const SLevelCreatorPanel::FArguments& InArgs);
	
};
