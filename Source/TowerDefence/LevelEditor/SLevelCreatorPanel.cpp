#include "SLevelCreatorPanel.h"

void SLevelCreatorPanel::Construct(const SLevelCreatorPanel::FArguments& InArgs)
{
	ChildSlot
	[
		SNew(STextBlock)
		.Text(FText::FromString("Hello World"))
	];
}
