#pragma once
#include "NewbeeProject.h"
#include "NSerializer.hpp"

// 진짜 서버는 아니지만 그냥 서버라고 가정하자
// 데디서버에서는 진짜 처리하겠지만 여기선 그냥 분배정도만 하자
class NServerManager
{
	
public:

	// Client -> Server
	static void SendPacketToServer(Serializer* serialized);
	
	// Server -> Client
	static void BroadcastToClient(Serializer* serialized);

};

