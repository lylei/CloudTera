// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SDK_HEIMDALLR_IMPL_H
#define SDK_HEIMDALLR_IMPL_H

#include <sofa/pbrpc/pbrpc.h>
#include <mutex.h>

#include "heimdallr.h"

namespace baidu {
namespace heimdallr {
namespace sdk {

////// Service //////
typedef server::AddServiceRequest AddServiceRequest;
typedef server::AddServiceResponse AddServiceResponse;
typedef server::SetServiceResourceRequest SetServiceResourceRequest;
typedef server::SetServiceResourceResponse SetServiceResourceResponse;
typedef server::ListServiceRequest ListServiceRequest;
typedef server::ListServiceResponse ListServiceResponse;

////// Group //////
typedef server::AddGroupRequest AddGroupRequest;
typedef server::AddGroupResponse AddGroupResponse;
typedef server::SetGroupQuotaRequest SetGroupQuotaRequest;
typedef server::SetGroupQuotaResponse SetGroupQuotaResponse;
typedef server::ListGroupRequest ListGroupRequest;
typedef server::ListGroupResponse ListGroupResponse;
typedef server::AddAppRequest AddAppRequest;
typedef server::AddAppResponse AddAppResponse;

////// User //////
typedef server::AddUserRequest AddUserRequest;
typedef server::AddUserResponse AddUserResponse;
typedef server::AddUserToGroupRequest AddUserToGroupRequest;
typedef server::AddUserToGroupResponse AddUserToGroupResponse;
typedef server::ListUserRequest ListUserRequest;
typedef server::ListUserResponse ListUserResponse;

typedef server::Server_Stub Server_Stub;

class HeimdallrImpl : public Heimdallr {
public:
  HeimdallrImpl();
  virtual StatusCode init();
  ////// Service //////
  virtual void AddService(const std::string& service_name, StatusCode* status,
                          UserAddServiceCallback* callback);
  virtual void SetServiceResource(const std::string& service_name,
                                  const std::string& resource_name,
                                  const std::string& provider_name,
                                  uint64_t quantity, StatusCode* status,
                                  UserSetServiceResourceCallback* callback);
  virtual void ListService(const std::string& service_name,
                           std::string* service_info,
                           StatusCode* status,
                           UserListServiceCallback* callback);

  ////// Group //////
  virtual void AddGroup(const std::string& group_name, StatusCode* status,
                        UserAddGroupCallback* callback);
  virtual void SetGroupQuota(const std::string& group_name,
                             const std::string& service_name,
                             const std::string& resource_name,
                             uint64_t quota, StatusCode* status,
                             UserSetGroupQuotaCallback* callback);
  virtual void ListGroup(const std::string& group_name, std::string* group_info,
                         StatusCode* status, UserListGroupCallback* callback);
  virtual void AddApp(const std::string& group_name, const std::string& app_name, uint64_t cpu, uint64_t mem,
                      uint64_t disk, uint64_t flash, StatusCode* status, UserAddAppCallback* callback);

  ////// User //////
  virtual void AddUser(const std::string& user_name, const std::string& passwd,
                       StatusCode* status, UserAddUserCallback* callback);
  virtual void AddUserToGroup(const std::string& user_name,
                              const std::string& group_name,
                              StatusCode* status,
                              UserAddUserToGroupCallback* callback);
  virtual void ListUser(const std::string& user_name, std::string* user_info,
                        StatusCode* status, UserListUserCallback* callback);

  virtual void IdentifyUser(const std::string& user_name,
                            const std::string& token,
                            std::string* group_name, StatusCode* status,
                            UserIdentifyUserCallback* callback);

private:
  ////// Service //////
  virtual void AddServiceCallback(sofa::pbrpc::RpcController* cntl,
                                  AddServiceRequest* request,
                                  AddServiceResponse* response,
                                  StatusCode* status,
                                  UserAddServiceCallback* callback,
                                  CondVar* cond);
  virtual void SetServiceResourceCallback(sofa::pbrpc::RpcController* cntl,
                                          SetServiceResourceRequest* request,
                                          SetServiceResourceResponse* response,
                                          StatusCode* status,
                                          UserSetServiceResourceCallback* callback, CondVar* cond);
  virtual void ListServiceCallback(sofa::pbrpc::RpcController* cntl,
                                   ListServiceRequest* request,
                                   ListServiceResponse* response,
                                   std::string* service_info, StatusCode* status,
                                   UserListServiceCallback* callback, CondVar* cond);

  ////// Group //////
  virtual void AddGroupCallback(sofa::pbrpc::RpcController* cntl,
                                AddGroupRequest* request,
                                AddGroupResponse* response,
                                StatusCode* status,
                                UserAddGroupCallback* callback,
                                CondVar* cond);
  virtual void SetGroupQuotaCallback(sofa::pbrpc::RpcController* cntl,
                                     SetGroupQuotaRequest* request,
                                     SetGroupQuotaResponse* response,
                                     StatusCode* status,
                                     UserSetGroupQuotaCallback* callback,
                                     CondVar* cond);
  virtual void ListGroupCallback(sofa::pbrpc::RpcController* cntl,
                                 ListGroupRequest* request,
                                 ListGroupResponse* response,
                                 std::string* group_info, StatusCode* status,
                                 UserListGroupCallback* callback, CondVar* cond);

  virtual void AddAppCallback(sofa::pbrpc::RpcController* cntl,
                                 AddAppRequest* request,
                                 AddAppResponse* response,
                                 StatusCode* status,
                                 UserAddAppCallback* callback, CondVar* cond);

  ////// User //////
  virtual void AddUserCallback(sofa::pbrpc::RpcController* cntl,
                               AddUserRequest* request,
                               AddUserResponse* response,
                               StatusCode* status,
                               UserAddUserCallback* callback, CondVar* cond);
  virtual void AddUserToGroupCallback(sofa::pbrpc::RpcController* cntl,
                                      AddUserToGroupRequest* request,
                                      AddUserToGroupResponse* response,
                                      StatusCode* status,
                                      UserAddUserToGroupCallback* callback,
                                      CondVar* cond);
  virtual void ListUserCallback(sofa::pbrpc::RpcController* cntl,
                                ListUserRequest* request,
                                ListUserResponse* response,
                                std::string* user_info,
                                StatusCode* status,
                                UserListUserCallback* callback, CondVar* cond);

  virtual void IdentifyUserCallback(const std::string& user_name,
                                    const std::string& token,
                                    std::string* group_name, StatusCode* status,
                                    UserIdentifyUserCallback* callback);

private:
  sofa::pbrpc::RpcClient* rpc_client_;
  sofa::pbrpc::RpcChannel* rpc_channel_;
  Server_Stub* stub_;
};

} // namespace sdk
} // namespace heimdallr
} // namespace baidu

#endif // SDK_HEIMDALLR_IMPL_H
