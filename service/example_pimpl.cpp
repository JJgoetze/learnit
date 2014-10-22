#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "framework/transcode_packet.h"
#include "framework/service_manager.h"
#include "framework/logging.h"
#include "framework/service_manager.h"
#include "framework/CbFormat.h"
#include "framework/tcp_connection.h"

#include "service/example_opcode_def.h"
#include "service/example_pimpl.h"




GameExampleServicePimpl::GameExampleServicePimpl(rapidjson::Value& config) 
{
	//load config
    next_account_id_ = 0; 
	config_ = config;
}

GameExampleServicePimpl::~GameExampleServicePimpl() {
}

bool GameExampleServicePimpl::Init() {
    opcodes_.insert(RampupOpCode::CMSG_ENTER_ROOM);
	opcodes_.insert(RampupOpCode::CMSG_LEAVE_ROOM);
	opcodes_.insert(RampupOpCode::CMSG_QUERY_TOP20);
	opcodes_.insert(RampupOpCode::CMSG_QUERY_ACCOUNT_BE_FORBIDDENED);
		
	req_handler_[RampupOpCode::CMSG_ENTER_ROOM] = &GameExampleServicePimpl::HandleEnterRoom;
	req_handler_[RampupOpCode::CMSG_LEAVE_ROOM] = &GameExampleServicePimpl::HandleLeaveRoom; 
	req_handler_[RampupOpCode::CMSG_QUERY_TOP20] = &GameExampleServicePimpl::HandleQueryTop20ByMoney;
	req_handler_[RampupOpCode::CMSG_QUERY_ACCOUNT_BE_FORBIDDENED] = &GameExampleServicePimpl::HandleQueryAccountBeForbidened;
	
	return Reload(config_);
}

int32_t GameExampleServicePimpl::HandleRequest(TcpConnection* conn, PacketTranscode& request){
    int32_t op_code = (int32_t)(request.GetOpcode());
	auto it = req_handler_.find(op_code);
	if (it != req_handler_.end()) 
	{
		LOG(INFO) << " GameExampleServicePimpl::HandleReqeust ,OpCode " << op_code;
		return (this->*(it->second))(conn, request);
	}
	else 
	{
		return -1;
	}	
}

//根据session查找玩家
Player*  GameExampleServicePimpl::FindPlayerBySessionId(std::string session_id){
    //在线玩家
    std::map<std::string, Player*>::iterator iter = session_player_map_.find(session_id); 
    if (iter != session_player_map_.end()){
        return iter->second; 
    } else {
        return nullptr; 
    }
}


//进入房间 
int32_t  GameExampleServicePimpl::HandleEnterRoom(TcpConnection* conn, PacketTranscode& request){
    Player* p = FindPlayerBySessionId(conn->GetSessionId());
    if (p != nullptr){

        //进入房间失败
        PacketTranscode packet;
        packet.SetOpcode(RampupOpCode::SMSG_ENTER_ROOM_RESP);

        EnterRoomResp  enter_room_resp;
        enter_room_resp.error_code = -1; 

        enter_room_resp.ToPacket(packet);
        conn->Write(packet);
        return  -1; 
    } else {
        next_account_id_++;

        Player* player = new Player();
        player->account_id = next_account_id_;
        player->money = rand() % 100000;

        session_player_map_[conn->GetSessionId()] = player;

        //发送给客户端进入房间成功
       PacketTranscode packet;
       packet.SetOpcode(RampupOpCode::SMSG_ENTER_ROOM_RESP);

       EnterRoomResp  enter_room_resp; 
       enter_room_resp.player = *player; 

       enter_room_resp.ToPacket(packet);
       conn->Write(packet); 
       return  0; 
    }
}
//离开房间
int32_t  GameExampleServicePimpl::HandleLeaveRoom(TcpConnection* conn, PacketTranscode& request){
    Player* p = FindPlayerBySessionId(conn->GetSessionId());
    if (p == nullptr){
        PacketTranscode packet;
        packet.SetOpcode(RampupOpCode::SMSG_LEAVE_ROOM_RESP);

        LeaveRoomResp  leave_room_resp;
        leave_room_resp.error_code = -1; 
        leave_room_resp.ToPacket(packet);
        conn->Write(packet);

        return  -1; 
    } else {

        PacketTranscode packet;
        packet.SetOpcode(RampupOpCode::SMSG_LEAVE_ROOM_RESP);

        LeaveRoomResp  leave_room_resp;
        leave_room_resp.player = *p;

        leave_room_resp.ToPacket(packet);
        conn->Write(packet);

        session_player_map_.erase(conn->GetSessionId()); 

        delete p; 

        return  0; 
    }
}

//查询金币前20的玩家信息
int32_t  GameExampleServicePimpl::HandleQueryTop20ByMoney(TcpConnection* conn, PacketTranscode& request){
    int top_index = 0;
     
    QueryTop20ByMoneyResp   query_resp; 
    std::map<std::string, Player*>::iterator iter =  session_player_map_.begin(); 
    for (iter; iter != session_player_map_.end(); ++iter){
	    top_index ++;
        if (top_index < 20){
            query_resp.players.push_back(*iter->second);
        } else {
            break; 
        }
    }
    PacketTranscode packet;
    packet.SetOpcode(RampupOpCode::SMSG_QUERY_TOP20_RESP);
    query_resp.ToPacket(packet); 
    conn->Write(packet);

    return  0;
}	
//查询某个账号是否被封
int32_t  GameExampleServicePimpl::HandleQueryAccountBeForbidened(TcpConnection* conn, PacketTranscode& request){
    AccountForbidenReq    req; 
    req.FromPacket(request); 

    //req.account_name

    PacketTranscode packet;
    packet.SetOpcode(RampupOpCode::SMSG_QUERY_ACCOUNT_BE_FORBIDDENED_RESP);
   
    AccountForbidenResp resp; 
    resp.ToPacket(packet); 

    conn->Write(packet);

    return 0;
}
	
void GameExampleServicePimpl::Destroy() {
}

bool GameExampleServicePimpl::Reload(rapidjson::Value& config) {
	return true;
}
	
void GameExampleServicePimpl::Update() {
}

void GameExampleServicePimpl::InjectDependency(const std::string& name, Service* service) {	
}

int32_t GameExampleServicePimpl::OnConnClosed(TcpConnection* conn) {
    Player* p = FindPlayerBySessionId(conn->GetSessionId());
    if (p == nullptr){
    }
    else {
        session_player_map_.erase(conn->GetSessionId());
        delete p;
    }
    return  0; 
}










