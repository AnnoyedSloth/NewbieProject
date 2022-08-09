#pragma once
#include "NewbeeProject.h"
#include "NPropertyInfo.h"
#include "NRPCManager.h"
#include "NServerManager.h"
#include "NObject.generated.h"

NCLASS()
class NObject
{
public:

	GENERATED_BODY();

	std::map<std::string, std::function<void(Serializer*, const char*, int)>> functionInfoMap;

	NFUNCTION()
	void Replicate()
	{
		//Serializer serialized = NRPCManager::Get()->Serialize<int>(objectName, "Move", 50.f, 15.f);
		//NServerManager::SendPacketToServer(&serialized);

		Serializer replicate = NRPCManager::Get()->Serialize<void>(objectName, "Update", locationX, locationY, objectName, camera, weapon);
		NServerManager::SendPacketToServer(&replicate);
	}

	// 용이한 디버깅을 위해 아직은 일부러 노출
	// 언제든지 매크로로 뺄수 있슴,,
	NFUNCTION()
	void Update(float locationX_, float locationY_, const string objectName_, NObject* camera_, NObject* weapon_)
	{
		if (objectName == objectName_)
		{
			return;
		}
		
		if (observingObjects.find(objectName_) == observingObjects.end())
		{
			cout << "Observing object not exist\n";
			return;
		}

		cout << "저는 " << objectName << "입니다. " << "감시하고 있는 오브젝트는 " << objectName_ << " 입니다.\n";
		cout << "갱신 전\t";
		cout << "X : " << observingObjects[objectName_]->Get_locationX() << ", Y : " << observingObjects[objectName_]->Get_locationY() << endl;

		observingObjects[objectName_]->Set_locationX(locationX_);
		observingObjects[objectName_]->Set_locationY(locationY_);
		observingObjects[objectName_]->Set_objectName(objectName_);
		observingObjects[objectName_]->Set_camera(camera_);
		observingObjects[objectName_]->Set_weapon(weapon_);

		cout << "갱신 후\t";
		cout << "X : " << observingObjects[objectName_]->Get_locationX() << ", Y : " << observingObjects[objectName_]->Get_locationY() << endl;
	}

	// 리플리케이션 테스트용 관찰 오브젝트
	map<string, NObject*> observingObjects;

	void AddObservingObject(const string objName)
	{
		NObject* newObject = new NObject();
		newObject->objectName = objName;
		newObject->Initialize();
		observingObjects[objName] = newObject;
	}


public:
	friend class NObjectManager;

	NObject();
	NObject(NObject* parent, string name);

private:

	NObject* parent;

	NPROPERTY()
		float locationX;

	NPROPERTY()
		float locationY;

	NPROPERTY()
		string objectName;
	
	NPROPERTY()
		NObject* camera;

	NPROPERTY()
		NObject* weapon;

	NFUNCTION()
		void Move(float x, float y);
};