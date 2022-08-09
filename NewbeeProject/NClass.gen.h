#pragma once
#include "NewbeeProject.h"
#include "NObject.h"

class NClass
{
	friend class NObjectManager;

public:

	NClass();

	void Initialize();

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
	string className;
	unsigned int objIdx;

	// ���� �𸮾������� int32 bitflag�� ���������� ���ǻ� bool�� �ϴ� ����(���� �������ɼ� ����) 
	bool isUnreachable; // GC���� ������ ����
	bool isPendingKill; // world�󿡼� destroy�� �������� ����
	bool isRootSet; // RootSet����? RootSet�� ��� GC���� �������� ����

	set<string> propertySet;
	set<string> methodSet;
};
