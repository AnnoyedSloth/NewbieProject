#include "NObjectManager.h"

NObjectManager* NObjectManager::objMgrInstance = nullptr;
constexpr unsigned int ObjCount = 1000;

NObjectManager::NObjectManager()
{
	objectIndexMap.clear();

	for (unsigned int a = 0; a < ObjCount; ++a)
	{
		classArray.push_back(new NClass());
		allocIndexQueue.push(a);
	}
}

NObjectManager* NObjectManager::Get()
{
	if (objMgrInstance == nullptr)
	{
		objMgrInstance = new NObjectManager();
	}

	return objMgrInstance;
}

void NObjectManager::Tick(float deltaTime)
{

}

NObject* NObjectManager::GetObject(const unsigned int idx)
{
	NObject* obj = classArray[idx]->objPtr;
	if (obj == nullptr)
	{
		cout << "Object ptr null임" << endl;
	}
	
	return obj;
}

NObject* NObjectManager::GetObject(const string objName)
{
	auto foundObj = objectIndexMap.find(objName);

	if (foundObj == objectIndexMap.end())
	{
		return nullptr;
	}

	unsigned int idx = foundObj->second;

	return GetObject(idx);
}

bool NObjectManager::SpawnObject(const string objName)
{

	bool isSpawnnable = true;

	// 스폰하기 전에 스폰할 수 없는 조건들은 여기에 추가해서 예외처리(ex. obj 이름 중복)
	auto isExist = objectIndexMap.find(objName);
	if (isExist != objectIndexMap.end())
	{
		cout << "Obj spawn failed. Obj name duplicated\n";
		isSpawnnable = false;
	}

	if (isSpawnnable == false)
	{
		return false;
	}

	NObject* obj = new NObject();

	obj->objectName = objName;
	obj->isReplicate = true;
	obj->Initialize();

	NClass* allocatedClass = AllocateObj(obj);
	if (allocatedClass == nullptr)
	{
		cout << "Allocated class is nullptr\n";
		return false;
	}

	replicatedObjects.push_back(obj);

	for (unsigned int a = 0; a < replicatedObjects.size(); ++a)
	{
		for (unsigned int b = 0; b < replicatedObjects.size(); ++b)
		{
			if (a == b)
				continue;

			// a가 관찰하는 map에 b가 없다면?
			if (replicatedObjects[a]->observingObjects.find(replicatedObjects[b]->objectName) == replicatedObjects[a]->observingObjects.end())
			{
				replicatedObjects[a]->AddObservingObject(replicatedObjects[b]->objectName);
			}
		}
	}
	

	// 멤버변수 중 subobject를 더 이쁘게 등록할만한 방법이 이거 말고 뭐가있을까...
	if (obj->propertyInfoMap.empty() != true)
	{
		for (auto iter = obj->propertyInfoMap.begin(); iter != obj->propertyInfoMap.end(); ++iter)
		{
			if (iter->second.DataTypeStr == "NObject*")
			{
				NObject* childObj = iter->second.GetValue<NObject*>();
				if (childObj == nullptr)
				{
					continue;
				}
				childObj->objectName = obj->objectName + "_" + childObj->objectName;
				NClass* allocatedChildClass = AllocateObj(childObj);
				allocatedClass->childIndexArr.push_back(allocatedChildClass->objIdx);
			}
		}
	}

	return isSpawnnable;
}

bool NObjectManager::DespawnObject(const string objName)
{
	auto foundIdx = objectIndexMap.find(objName);
	if (foundIdx == objectIndexMap.end())
	{
		cout << "Failed to find obj index with obj name\n";
		return false;
	}

	const unsigned int idx = foundIdx->second;

	NClass* objClass = classArray[idx];

	// PendingKill 상태로 전환
	objClass->isPendingKill = true;

	// 서브오브젝트도 전부 PendingKill 상태로 전환
	for (unsigned int childIdx : objClass->childIndexArr)
	{
		classArray[childIdx]->isPendingKill = true;
	}

	return true;
}

NClass* NObjectManager::AllocateObj(NObject* obj)
{
	unsigned int idx = allocIndexQueue.front();

	if (idx >= ObjCount)
	{
		cout << "idx over than max object count" << endl;
		return nullptr;
	}

	if (classArray[idx]->IsValid() == true)
	{
		cout << "already allocated in the same idx" << endl;
		return nullptr;
	}

	allocIndexQueue.pop();

	if (classArray[idx]->objPtr != nullptr)
	{
		classArray[idx]->objPtr = nullptr;
	}

	classArray[idx]->objPtr = obj;
	classArray[idx]->isPendingKill = false;
	classArray[idx]->isUnreachable = false;
	classArray[idx]->objIdx = idx;

	objectIndexMap.insert({ obj->objectName, idx });

	return classArray[idx];
}

void NObjectManager::RemoveObj(const unsigned int idx)
{	
	if (classArray[idx]->objPtr == nullptr)
	{
		cout << "Cannot remove because object of idx num : " << idx << "is already null" << endl;
		return;
	}

	objectIndexMap.erase(classArray[idx]->objPtr->objectName);

	// 서브오브젝트 이름목록 삭제
	for (unsigned int childIdx : classArray[idx]->childIndexArr)
	{
		objectIndexMap.erase(classArray[childIdx]->objPtr->objectName);
	}

	classArray[idx]->isPendingKill = false;
	classArray[idx]->isUnreachable = false; 
	classArray[idx]->childIndexArr.clear();

	allocIndexQueue.push(idx);

	string tempName;

	if (classArray[idx]->objPtr != nullptr)
	{
		tempName = classArray[idx]->objPtr->objectName;

		delete classArray[idx]->objPtr;
		classArray[idx]->objPtr = nullptr;
	}

	cout << "object name : " << tempName << " is successfully removed.\n";
}

//Mark
void NObjectManager::MarkObjectsAsUnreachable()
{
	for (unsigned int i = 0; i < ObjCount; ++i)
	{
		if (classArray[i]->objPtr == nullptr)
			continue;

		if (classArray[i]->IsPendingKill() == true && classArray[i]->IsRootSet() == false)
		{
			classArray[i]->isUnreachable = true;
		}
	}
}

// Sweep
void  NObjectManager::CollectGarbage()
{
	for (unsigned int i = 0; i < ObjCount; ++i)
	{
		NClass* classInfo = classArray[i];
		if (classInfo->objPtr == nullptr
			|| classInfo->IsRootSet() == true) // RootSet일 경우 수집하지 않는다
			continue;

		if (classArray[i]->IsUnreachable() == true)
		{
			RemoveObj(i);
		}
	}
}

void NObjectManager::PrintValidObjectsName()
{
	for (const NClass* obj : classArray)
	{
		if (obj->objPtr != nullptr)
		{
			cout << obj->objPtr->objectName << endl;
		}
	}
}