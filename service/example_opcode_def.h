#ifndef SERVICE_RAMP_UP_OPCODE_DEF_H_
#define SERVICE_RAMP_UP_OPCODE_DEF_H_

#include <inttypes.h>

enum RampupOpCode
{
    //进入房间
	CMSG_ENTER_ROOM			   	  = 201,			
	//进入房间成功
	SMSG_ENTER_ROOM_RESP	  	 = 202,		

   //离开房间
	CMSG_LEAVE_ROOM			     = 203,			
	//离开房间成功
	SMSG_LEAVE_ROOM_RESP	     = 204,	
	
	//查询金币前20名的玩家
	CMSG_QUERY_TOP20		     =  205,
	//查询金币前20名的玩家信息
	SMSG_QUERY_TOP20_RESP	         =  206,
		
	//查询账号是否被封
	CMSG_QUERY_ACCOUNT_BE_FORBIDDENED =  207, 	
	//查询账号是否被封返回
	SMSG_QUERY_ACCOUNT_BE_FORBIDDENED_RESP = 208, 
	
};


#endif  // SERVICE_RAMP_UP_OPCODE_DEF_H_
