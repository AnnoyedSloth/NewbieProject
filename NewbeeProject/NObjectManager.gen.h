#pragma once
#include "NewbeeProject.h"
#include "NClass.h"

class NObjectManager
{
private:
	static NObjectManager* objMgrInstance;

	queue<unsigned int> allocIndexQueue;
	vector<NClass*> ClassArray;

public:
	NObjectManager();

	static NObjectManager* Get();

	//void Tick(float deltaTime);

	// ���⿡�� ���÷��� �����͸� �����;��ұ�?
	void AllocateObj(NObject* obj);
	void RemoveObj(const unsigned int idx);

	//Mark
	void MarkObjectsAsUnreachable();

	// Sweep
	void  CollectGarbage();
};
