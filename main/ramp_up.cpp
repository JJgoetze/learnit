#include <stdio.h>
#include <dlfcn.h>

#include	<unistd.h>
#include	<signal.h>                                               
#include	<sys/stat.h>
#include	<sys/param.h>

#include <thread>

#include <gtest/gtest.h>
#include <chrono>

#include <gflags/gflags.h>

#include "framework/logging.h"
#include "framework/service_manager.h"
#include "framework/game_loop.h"
//#include "framework/game_server_service.h"
#include "framework/game_service.h"
#include "framework/packet_dispatcher.h"
#include "framework/CbFormat.h"
#include "framework/logpluslogger.h"
#include "service/example_service.h"


class ExampleGameLoop : public GameLoop {
public:
	explicit ExampleGameLoop(const std::string &config) : GameLoop(config) {}
	
  	virtual bool Init() {
	
		//GameServerService *game_server_service = new GameServerService();
		//service_manager_->Register("GameServerService", game_server_service);
		
  		service_manager_->Register("Dispatcher", new Dispatcher());
		
	    if (config_.IsObject() && config_.HasMember("tcp_port") && config_["tcp_port"].IsInt()) {
  			service_manager_->Register("server", new TcpServer(config_["tcp_port"].GetInt()));
  		} else {
  			LOG(ERROR) << "Please check the config has an object member 'Dispatcher'";
  			return false;
  		}

        //ramp up service
        service_manager_->Register("GameExampleService", new GameExampleService(config_));

		
   		bool res = service_manager_->Init();
		LOG(INFO) << "service_manager_ Init success";
   		return res;
   	}

  	virtual void Update() {

		CbUtility::updateCurLoopTime(CbUtility::get_start_time()); 
		int64_t  t1 = CbUtility::get_start_time(); 
  		Dispatcher *dispatcher = service_manager_->LocateService<Dispatcher>("Dispatcher");
  		dispatcher->Update();

  		for (auto it = gameServices_.begin(); it != gameServices_.end(); ++it) {
  			if ((*it)->IsRunning())	{
  			(*it)->Update();
  			}
  		}		
    }

  	virtual void Destory() {
  		GameLoop::Destory();
  	}
private:
  	std::set<GameService*> gameServices_;
};


int main(int argc, char* argv[]) 
{
    if  (argc > 1)
	{
	    //运行unitest用例
		::testing::InitGoogleTest(&argc, argv);
	    int ret = RUN_ALL_TESTS();
	    return ret;
	} else{
	    //运行ramp up miniproject的服务,ExampleService
		std::string logcplus_config = "../conf/log4cplus.properties";
		log4cplus::PropertyConfigurator::doConfigure(logcplus_config.c_str());

		LogPlusLogger  log4cplus_logger; 
		SetDefaultLogger(&log4cplus_logger); 

		int port = 7011;
		LOG(INFO) << "starting on port " << port << "...";
		LOG(INFO) << "server is running...";
		FILE* configFile = fopen("../conf/conf.txt", "r");
		if (configFile == nullptr) {
			LOG(ERROR) <<  "fopen error";
			return 1;
		}
		char config[102400] = {0};
		uint32_t realLength = fread(config, sizeof(config[0]), sizeof(config), configFile);

		LOG(INFO) <<  "realLength: " << realLength;

		if (realLength >= sizeof(config)) {
			LOG(ERROR) <<  "fread error , buffer size  " << sizeof(config) << " is not enough";
			fclose(configFile);
			return 2;
		}

		int32_t result = 0;
		if ((result = ferror(configFile)) != 0) {
			LOG(ERROR) << "fopen error code is " << result;
			fclose(configFile);
			return 3;
		}
		fclose(configFile);

		GameLoop* loop = new ExampleGameLoop(config);
		loop->Run();
		return 0;	
	}	
}
