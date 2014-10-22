#ifndef SERVICE_RAMP_UP_PIMPL_H_
#define SERVICE_RAMP_UP_PIMPL_H_

#include <rapidjson/document.h>

#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "service/example_def.h"
#include "service/example_opcode_def.h"

class PacketTranscode;
class TcpConnection;
class Service;

class PlayerRoom{
public:
	PlayerRoom():capacity_(5){}
	PlayerRoom(int32_t cap) : capacity_(cap){}
	
	//判断房间是否已经满员
	bool IsFull(){
		return player_ids_.size() >= capacity_;
	}

	//判断房间是否为空
	bool IsEmpty(){
		return player_ids_.empty();
	}

	//将指定玩家分配到该房间
	bool AssignPlayerId(int32_t player_id){
		if (IsFull())
			return false;
		
		player_ids_.insert(player_id);
		return true;
	}

	//获取房间的空位数
	int32_t GetRestCount(){
		return capacity_ - player_ids_.size();
	}

	//从房间中移除某个玩家
	void RemovePlayerId(int32_t player_id){
		player_ids_.erase(player_id);
	}

	//查找某个玩家是否存在
	bool IsExistPlayerId(int32_t player_id){
		return player_ids_.count(player_id);
	}

private:
	//房间内的所有玩家集合
	std::set<int32_t> player_ids_;
	//房间的容量
	int32_t capacity_;
};


class GameExampleServicePimpl {
public:
	explicit GameExampleServicePimpl(rapidjson::Value& config);
	virtual ~GameExampleServicePimpl();

	virtual bool Reload(rapidjson::Value& config);

public:
	virtual bool Init();
	virtual void Destroy();
	virtual void Update();
	
public:
	virtual void InjectDependency(const std::string& name, Service* service);
	
public:
	virtual int32_t HandleRequest(TcpConnection* conn, PacketTranscode& request);
	virtual int32_t OnConnClosed(TcpConnection* conn);
		
    //维护opcode和handler的对应关系
	typedef int32_t(GameExampleServicePimpl::*ReqHandler)(TcpConnection* conn, PacketTranscode& request);
	std::map<uint32_t, ReqHandler> req_handler_;

protected:
    //进入房间 
    virtual int32_t  HandleEnterRoom(TcpConnection* conn, PacketTranscode& request);
	//离开房间
	virtual int32_t  HandleLeaveRoom(TcpConnection* conn, PacketTranscode& request); 
	//查询金币前20的玩家信息
	virtual int32_t  HandleQueryTop20ByMoney(TcpConnection* conn, PacketTranscode& request); 	
	//查询某个账号是否被封
	virtual int32_t  HandleQueryAccountBeForbidened(TcpConnection* conn, PacketTranscode& request); 

private:
    //根据session查找玩家
    virtual Player*  FindPlayerBySessionId(std::string session_id); 
	virtual PlayerRoom* GetBestRoom();

protected:
   

    //账号ID
    int32_t          next_account_id_; 
    //在线玩家
    std::map<std::string,Player*>  session_player_map_; 
	//所有房间
	std::vector<PlayerRoom*> player_rooms;

protected:
	rapidjson::Value config_;
    std::set<uint32_t> opcodes_;
private:
	friend class GameExampleService;
};


#endif  // SERVICE_RAMP_UP_PIMPL_H_
