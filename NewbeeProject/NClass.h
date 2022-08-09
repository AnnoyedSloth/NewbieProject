#pragma once
#include "NewbeeProject.h"
#include "NObject.h"

class NClass
{
	friend class NObjectManager;

public:

	NClass();

	bool IsValid()
	{
		return objPtr != nullptr;
	}

	void SetRootset(const bool flag)
	{
		isRootSet = flag;
	}

	bool IsPendingKill() const { return isPendingKill; }
	bool IsUnreachable() const { return isUnreachable; }
	bool IsRootSet() const { return isRootSet; }

private:

	NObject* objPtr;
	unsigned int objIdx;

	// 원래 언리얼엔진에선 int32 bitflag로 관리하지만 편의상 bool로 일단 구현(추후 수정가능성 있음) 
	bool isUnreachable; // GC에서 수집될 조건
	bool isPendingKill; // world상에서 destroy된 상태인지 점검
	bool isRootSet; // RootSet인지? RootSet일 경우 GC에서 수집되지 않음

	vector<unsigned int> childIndexArr;
};