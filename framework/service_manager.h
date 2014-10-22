//
//  service_manager.h
//  gateway
//
//  Created by Fubai Zhong on 10/4/13.
//  Copyright (c) 2013 Baina. All rights reserved.
//

#ifndef FRAMEWORK_SERVER_SERVICE_MANAGER_H_
#define FRAMEWORK_SERVER_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>

#include "framework/singleton.h"
#include "framework/service.h"
#include "framework/dependencyResolver.h"

class ServiceManager : public Singleton <ServiceManager>, public Service
{
protected:
    ServiceManager();
    ~ServiceManager();
    
public:
    virtual bool Init();
    virtual bool Start();
    virtual bool Stop();
    virtual bool IsRunning();
    
public:
    bool Register(const char* name, Service* service);
    bool Register(const std::string& name, Service* service);
    
    Service* Deregister(const char* name);
    Service* Deregister(const std::string& name);
    
    //add by shshang, tcp server needs the order to invoke OnConnClosed
    std::list<std::string> GetSort();

    void Destroy() {
        for (auto it = services_.begin(); it != services_.end(); ++it) {
            delete it->second;
        }
        services_.clear();
    };

    template<typename T>
    T* LocateService(const char* name) {
        return LocateService<T>(std::string(name));
    }
    
    template<typename T>
    T* LocateService(const std::string& name) {
        auto it = services_.find(name);
        if (it != services_.end()) {
            return dynamic_cast<T*>(it->second);
        }
        return nullptr;
    };
    
private:
    std::map<std::string, Service*> services_;
    bool initialized_;
    bool running_;
    DependencyResolver dependencyResolver_;
    
private:
    friend class Singleton<ServiceManager>;
};

#endif  // FRAMEWORK_SERVER_SERVICE_MANAGER_H_
