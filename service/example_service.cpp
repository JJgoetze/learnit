
#include <dlfcn.h>

#include "framework/packet_dispatcher.h"
#include "framework/transcode_packet.h"

#include "framework/service_manager.h"

#include "service/example_service.h"
#include "service/example_pimpl.h"

GameExampleService::GameExampleService(rapidjson::Value& config)
	: is_running_(false)
	, pimpl_(nullptr)
	, dispatcher_(nullptr)
 {
	config_ = config;
}

GameExampleService::~GameExampleService() {
}

bool GameExampleService::Reload(rapidjson::Value& config) 
{
    pimpl_ = new GameExampleServicePimpl(config); 	
	return true;
}


bool GameExampleService::Init() {
	return Reload(config_);
}

bool GameExampleService::Start() {
	//注册Opcode
	bool ret = dispatcher_->RegisterOpcode(pimpl_->opcodes_, this);
	if (ret) {
		//维护长连接
		ret = dispatcher_->SubscribeConnClosedMessage(this);
		if (ret) {
			is_running_ = true;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool GameExampleService::Stop() {
	is_running_ = false;
	//解注册Opcode
	dispatcher_->DeregisterOpcode(pimpl_->opcodes_);
	return true;
}

bool GameExampleService::IsRunning() {
	return is_running_;
}

void GameExampleService::Update() {

}

const std::set<std::string> GameExampleService::GetDependentServices() const {
	std::set<std::string> dependencies;
	dependencies.insert("Dispatcher");
	return dependencies;
}

bool GameExampleService::IsDependentServicesResolved() const {
	if (dispatcher_ != nullptr){
		return true;
	} else {
		return false;
	}
}

void GameExampleService::Inject(std::string& name, Service* service) {
	if (name == "Dispatcher") 
	{	
		dispatcher_ = static_cast<Dispatcher*>(service);
	} 
}


bool GameExampleService::OnRecvPacket(TcpConnection* conn, PacketTranscode& request) {
	pimpl_->HandleRequest(conn, request);
	return true;
}

bool GameExampleService::OnConnClosed(TcpConnection* conn) {
	pimpl_->OnConnClosed(conn);
	return true;
}




