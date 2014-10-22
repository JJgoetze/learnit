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

class PlayerMoneyCmp{
public:
	bool inline operator () (const Player *p1, const Player *p2) const{
		return p1->money > p2->money;
	}
};

GameExampleServicePimpl::GameExampleServicePimpl(rapidjson::Value& config) 
{
	//load config
    next_account_id_ = 0; 
	config_ = config;
}

GameExampleServicePimpl::~GameExampleServicePimpl() {
	PlayerRoom* del;

	for (std::vector<PlayerRoom*>::iterator it = player_rooms.begin();
		it != player_rooms.end();){
		del = *it;
		++it;
		delete del;
	}
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

PlayerRoom*  GameExampleServicePimpl::GetBestRoom(){
	int index = -1;

	int i = 0;
	for (i; i < player_rooms.size(); ++i){
		if ( !player_rooms[i]->IsFull()){
			index = i;
			break;
		}
	}

	for (i; i < player_rooms.size(); ++i){
		if (player_rooms[i]->GetRestCount() < player_rooms[index]->GetRestCount())
			index = i;
	}

	if (index > 0 && index < player_rooms.size())
		return player_rooms[index];
	else
		return NULL;
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

		PlayerRoom* player_room = GetBestRoom();
		if (player_room != NULL)
			player_room->AssignPlayerId(player->account_id);
		else{
			player_room = new PlayerRoom;
			player_rooms.push_back(player_room);
			player_room->AssignPlayerId(player->account_id);
		}

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
		for (std::vector<PlayerRoom*>::iterator it = player_rooms.begin();
			it != player_rooms.end(); ){
			if ((*it)->IsExistPlayerId(p->account_id)){
				(*it)->RemovePlayerId(p->account_id);

				if ((*it)->IsEmpty()){
					delete *it;
					player_rooms.erase(it);
				}
				break;
			}
			else {
				++it;
			}
		}

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
	std::vector<Player*> players;

    std::map<std::string, Player*>::iterator iter =  session_player_map_.begin(); 
    for (iter; iter != session_player_map_.end(); ++iter){
		players.push_back(iter->second);
    }

	sort(players.begin(), players.end(), PlayerMoneyCmp());

	std::vector<Player*>::iterator it = players.begin();
	for (it; it != players.end(); ++it){
		top_index ++;
		if (top_index < 20){
			query_resp.players.push_back(**it);
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
