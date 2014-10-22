#ifndef SERVICE_RAMP_UP_SERVICE_H_
#define SERVICE_RAMP_UP_SERVICE_H_

#include <rapidjson/document.h>
#include <string>
#include <set>

#include "framework/game_service.h"

class TcpConnection;
class PacketTranscode;
class Dispatcher;
class GameExampleServicePimpl; 


class GameExampleService : public GameService {
public:
	explicit GameExampleService(rapidjson::Value& config);
	~GameExampleService();

	virtual bool Reload(rapidjson::Value& config);

public:
	virtual bool Init();
	virtual bool Start();
	virtual bool Stop();
	virtual bool IsRunning();

	virtual void Update();

	//服务依赖
	virtual const std::set<std::string> GetDependentServices() const;
	virtual bool IsDependentServicesResolved() const;
	virtual void Inject(std::string& name, Service* service);
	
public:
	virtual bool OnRecvPacket(TcpConnection* conn, PacketTranscode& request);
	virtual bool OnConnClosed(TcpConnection* conn);
	
private:
	rapidjson::Value config_;


	bool is_running_;

	GameExampleServicePimpl* pimpl_; 
	
	Dispatcher* dispatcher_;
};

#endif  // SERVICE_RAMP_UP_SERVICE_H_
