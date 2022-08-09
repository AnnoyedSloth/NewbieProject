#pragma once
#include "NewbeeProject.h"
#include "NSerializer.hpp"

// ��¥ ������ �ƴ����� �׳� ������� ��������
// ���𼭹������� ��¥ ó���ϰ����� ���⼱ �׳� �й������� ����
class NServerManager
{
	
public:

	// Client -> Server
	static void SendPacketToServer(Serializer* serialized);
	
	// Server -> Client
	static void BroadcastToClient(Serializer* serialized);

};

