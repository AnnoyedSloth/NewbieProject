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

		NRPCManager::Get()->CallFunc(objName, funcName, serialized->current(), serialized->size() - funcName.size() - objName.size()); // 서버에서 클라이언트로 전송하고 클라이언트에서 실행하는 함수
	}
}
