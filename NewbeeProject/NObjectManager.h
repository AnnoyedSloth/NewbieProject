#pragma once
#include "NewbeeProject.h"
#include "NClass.h"

class NObjectManager
{
private:
	static NObjectManager* objMgrInstance;

	queue<unsigned int> allocIndexQueue;

	unordered_map<string, unsigned int> objectIndexMap;
	vector<NObject*> replicatedObjects;

	// 오브젝트풀로 오브젝트들 관리
	vector<NClass*> classArray;

	// 순회하며 GC관리하는 Array로 Allocate
	NClass* AllocateObj(NObject* obj);

	// 실제로 메모리에서 삭제
	void RemoveObj(const unsigned int idx);

public:
	NObjectManager();
	static NObjectManager* Get();

	void Tick(float deltaTime);

	// 오브젝트 스폰
	bool SpawnObject(const string objName);
	// 오브젝트 디스폰(mark as pendingkill)
	bool DespawnObject(const string objName);

	NObject* GetObject(const unsigned int idx);
	NObject* GetObject(const string objName);

	//Mark 
	void MarkObjectsAsUnreachable();

	// Sweep
	void  CollectGarbage();

	void PrintValidObjectsName();

public:
	vector<NObject*>& const GetReplicatedObjectArr() { return replicatedObjects; }
};