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

	// ������ƮǮ�� ������Ʈ�� ����
	vector<NClass*> classArray;

	// ��ȸ�ϸ� GC�����ϴ� Array�� Allocate
	NClass* AllocateObj(NObject* obj);

	// ������ �޸𸮿��� ����
	void RemoveObj(const unsigned int idx);

public:
	NObjectManager();
	static NObjectManager* Get();

	void Tick(float deltaTime);

	// ������Ʈ ����
	bool SpawnObject(const string objName);
	// ������Ʈ ����(mark as pendingkill)
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