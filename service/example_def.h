#ifndef SERVICE_RAMP_UP_DEF_H_
#define SERVICE_RAMP_UP_DEF_H_

#include <inttypes.h>
#include <vector>
#include <string>

class PacketTranscode;

struct Player
{
    int32_t  account_id;    //账号id
    int64_t  money;         //金币

    virtual void	ToPacket(PacketTranscode& packet);
    virtual  void   FromPacket(PacketTranscode& packet);

};


struct EnterRoomResp  
{
    EnterRoomResp(){
        error_code = 0; 
    }
    int32_t error_code; 
    Player  player; 

    virtual void	ToPacket(PacketTranscode& packet);
    virtual  void   FromPacket(PacketTranscode& packet);
};


struct LeaveRoomResp
{
    LeaveRoomResp(){
        error_code = 0;
    }
    int32_t error_code;
    Player  player;

    virtual void	ToPacket(PacketTranscode& packet);
    virtual  void   FromPacket(PacketTranscode& packet);
};



struct QueryTop20ByMoneyResp
{
    std::vector<Player>  players;

    virtual void	ToPacket(PacketTranscode& packet);
    virtual  void   FromPacket(PacketTranscode& packet);
};

// 账号查封请求

struct AccountForbidenReq
{
    std::string     account_name; 

    virtual void	ToPacket(PacketTranscode& packet);
    virtual  void   FromPacket(PacketTranscode& packet);
};


struct AccountForbidenResp
{
    AccountForbidenResp(){
        status = 0; 
    }
    //0:没被封 1:被封
    int32_t         status;

    virtual void	ToPacket(PacketTranscode& packet);
    virtual  void   FromPacket(PacketTranscode& packet);
};


#endif  // SERVICE_RAMP_UP_DEF_H_
