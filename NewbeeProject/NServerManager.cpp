#include "NServerManager.h"
#include "NObjectManager.h"
#include "NRPCManager.h"

void NServerManager::SendPacketToServer(Serializer* serialized)
{
	BroadcastToClient(serialized);
}

void NServerManager::BroadcastToClient(Serializer* serialized)
{
	string dummyName;
	*serialized >> dummyName;

	string funcName;
	*serialized >> funcName;

	for (NObject* obj : NObjectManager::Get()->GetReplicatedObjectArr())
	{
		string objName = obj->Get_objectName();

		NRPCManager::Get()->CallFunc(objName, funcName, serialized->current(), serialized->size() - funcName.size() - objName.size()); // �������� Ŭ���̾�Ʈ�� �����ϰ� Ŭ���̾�Ʈ���� �����ϴ� �Լ�
	}
}
