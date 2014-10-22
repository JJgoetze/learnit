#include "service/example_def.h"
#include "framework/transcode_packet.h"


void Player::FromPacket(PacketTranscode& packet)
{
    packet >> account_id >> money; 
}

void Player::ToPacket(PacketTranscode& packet)
{
    packet << account_id << money;

}

void EnterRoomResp::FromPacket(PacketTranscode& packet)
{
    packet >> error_code; 
    player.FromPacket(packet); 
}

void EnterRoomResp::ToPacket(PacketTranscode& packet)
{
    packet << error_code; 
    player.ToPacket(packet); 
}


void LeaveRoomResp::FromPacket(PacketTranscode& packet)
{
    packet >> error_code;
    player.FromPacket(packet);
}

void LeaveRoomResp::ToPacket(PacketTranscode& packet)
{
    packet << error_code;
    player.ToPacket(packet);
}


void QueryTop20ByMoneyResp::ToPacket(PacketTranscode& packet)
{
    int32_t nSize = players.size(); 
    packet << nSize; 
    for (int32_t i = 0; i < nSize; i++){
        players[i].ToPacket(packet); 
    }
}

void QueryTop20ByMoneyResp::FromPacket(PacketTranscode& packet)
{
    int32_t nSize = 0;
    packet >> nSize;
    for (int32_t i = 0; i < nSize; i++){
        Player p; 
        p.FromPacket(packet); 
        players.push_back(p); 
    }
}


void AccountForbidenReq::ToPacket(PacketTranscode& packet)
{
    packet << account_name; 
}

void AccountForbidenReq::FromPacket(PacketTranscode& packet)
{
    packet >> account_name; 
}


void AccountForbidenResp::ToPacket(PacketTranscode& packet)
{
    packet << status;
}

void AccountForbidenResp::FromPacket(PacketTranscode& packet)
{
    packet >> status;
}






