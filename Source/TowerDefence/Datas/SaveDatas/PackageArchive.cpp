#include "PackageArchive.h"

void UPackageArchive::DeleteFood(int32 FoodID, int32 Num)
{
	int32* Num = Foods.Find(FoodID);
	if(Num!=nullptr)
	{
		(*Num) --;
	}
	if((*Num)<=0)
	{
		Foods.Remove(FoodID);
	}
}

void UPackageArchive::DeleteDevelopment(int32 DevelopID, int32 Num)
{
	int32* Num = Developments.Find(DevelopID);
	if(Num!=nullptr)
	{
		(*Num) --;
	}
	if((*Num)<=0)
	{
		Foods.Remove(DevelopID);
	}
}
