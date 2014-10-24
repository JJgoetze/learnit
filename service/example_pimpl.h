#ifndef SERVICE_RAMP_UP_PIMPL_H_
#define SERVICE_RAMP_UP_PIMPL_H_

#include <rapidjson/document.h>

#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>

#include "service/example_def.h"
#include "service/example_opcode_def.h"

class PacketTranscode;
class TcpConnection;
class Service;

class PlayerRoom{
public:
	PlayerRoom():capacity_(5){}
	PlayerRoom(int32_t cap) : capacity_(cap){}
	virtual ~PlayerRoom(){}

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

class ForbindendFile{
public:
	/*static ForbindendFile * GetInstance(){
		static ForbindendFile t;
		return &t;
	}*/
	
	static ForbindendFile * GetInstance(const std::string filename){
		static ForbindendFile t(filename);
		return &t;
	}

	virtual ~ForbindendFile(){ if (!data_) data_.close(); }

	bool IsForbidended(std::string account){
		long low_bd, up_bd, mid;
		data_.seekg(0, std::ios::beg);
		low_bd = data_.tellg();
		data_.seekg(0, std::ios::end);
		up_bd = data_.tellg() / unit_size_ - 1;

		while (low_bd <= up_bd){
			mid = (low_bd + up_bd) / 2;
			data_.seekg(mid * unit_size_);
			data_.get(buf, unit_size_);
			switch (ForbidendedMatch(account)){
			case small:
				up_bd = mid - 1;
				break;
			case large:
				low_bd = mid + 1;
				break;
			case equal:
				return true;
			}
		}

		return false;
	}

	bool IsInited(){
		return is_inited_;
	}

public:
	enum status { small, equal, large };

protected:
	enum status ForbidendedMatch(std::string account){
		for (size_t i = 0; i < account.size(); ++i){
			if (account[i] > buf[i])
				return large;
			else if (account[i] < buf[i])
				return small;
		}

		return equal;
	}

private:
	//ForbindendFile():filename_("forbidended"){ if (Init()) is_inited_ = true; else is_inited_ = false; }
	ForbindendFile(std::string filename) : filename_(filename){ if (Init()) is_inited_ = true; else is_inited_ = false; }
	
	bool Init(){
		std::ifstream conf(filename_ + ".conf", std::ios::in);
		if (!conf){
			return false;
		}

		std::string proprity;
		conf >> unit_size_;
		unit_size_ *= sizeof(char);
		conf.close();

		data_.open(filename_ + ".data", std::ios::in | std::ios::binary);
		if (!data_){
			return false;
		}

		return true;
	}

private:
	std::string filename_;
	size_t unit_size_;
	std::ifstream data_;
	bool is_inited_;
	char buf[256];
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
