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

	// 여기에서 리플렉션 데이터를 가져와야할까?
	void AllocateObj(NObject* obj);
	void RemoveObj(const unsigned int idx);

	//Mark
	void MarkObjectsAsUnreachable();

	// Sweep
	void  CollectGarbage();
};
