// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>

#include "heimdallr_impl.h"

extern std::string CONF_server_addr;
extern std::string CONF_server_port;

namespace baidu {
namespace heimdallr {
namespace sdk {

HeimdallrImpl::HeimdallrImpl() {
  init();
}

StatusCode HeimdallrImpl::init() {
  SOFA_PBRPC_SET_LOG_LEVEL(INFO);

  sofa::pbrpc::RpcClientOptions options;
  rpc_client_ = new sofa::pbrpc::RpcClient(options);
  rpc_channel_ = new sofa::pbrpc::RpcChannel(rpc_client_, CONF_server_addr +
                                             ":" + CONF_server_port);
  stub_ = new Server_Stub(rpc_channel_);
  return kOK;
}

void HeimdallrImpl::AddService(const std::string& service_name,
                               StatusCode* status,
                               UserAddServiceCallback* callback) {
  AddServiceRequest* request = new AddServiceRequest();
  request->set_service_name(service_name);

  AddServiceResponse* response = new AddServiceResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::AddServiceCallback, cntl,
                            request, response, status, callback, &cond);

  stub_->AddService(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::SetServiceResource(const std::string& service_name,
                                       const std::string& resource_name,
                                       const std::string& provider_name,
                                       int64_t quantity,
                                       StatusCode* status,
                                       UserSetServiceResourceCallback* callback) {
  SetServiceResourceRequest* request = new SetServiceResourceRequest();
  request->set_service_name(service_name);
  request->set_resource_name(resource_name);
  request->set_provider_name(provider_name);
  request->set_quantity(quantity);

  SetServiceResourceResponse* response = new SetServiceResourceResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::SetServiceResourceCallback,
                            cntl, request, response, status, callback, &cond);

  stub_->SetServiceResource(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::ListService(const std::string& service_name,
                                std::string* service_info,
                                StatusCode* status,
                                UserListServiceCallback* callback) {
  ListServiceRequest* request = new ListServiceRequest();
  request->set_service_name(service_name);

  ListServiceResponse* response = new ListServiceResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::ListServiceCallback, cntl,
                            request, response, service_info, status,
                            callback, &cond);

  stub_->ListService(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}


void HeimdallrImpl::AddGroup(const std::string& group_name, StatusCode* status,
                             UserAddGroupCallback* callback) {
  AddGroupRequest* request = new AddGroupRequest();
  request->set_group_name(group_name);

  AddGroupResponse* response = new AddGroupResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::AddGroupCallback, cntl,
                            request, response, status, callback, &cond);

  stub_->AddGroup(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::SetGroupQuota(const std::string& group_name,
                                  const std::string& service_name,
                                  int64_t host, int64_t cpu, int64_t mem, int64_t disk,
                                  int64_t flash, StatusCode* status,
                                  UserSetGroupQuotaCallback* callback) {
  SetGroupQuotaRequest* request = new SetGroupQuotaRequest();
  request->set_group_name(group_name);
  request->set_service_name(service_name);
  request->set_host(host);
  request->set_cpu(cpu);
  request->set_mem(mem);
  request->set_disk(disk);
  request->set_flash(flash);

  SetGroupQuotaResponse* response = new SetGroupQuotaResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::SetGroupQuotaCallback,
                            cntl, request, response, status, callback, &cond);

  stub_->SetGroupQuota(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::ListGroup(const std::string& group_name,
                              std::string* group_info,
                              StatusCode* status,
                              UserListGroupCallback* callback) {
  ListGroupRequest* request = new ListGroupRequest();
  request->set_group_name(group_name);

  ListGroupResponse* response = new ListGroupResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::ListGroupCallback, cntl,
                            request, response, group_info, status,
                            callback, &cond);

  stub_->ListGroup(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::AddApp(const std::string& group_name, const std::string& app_name,
                           const std::string& app_id, int64_t host, int64_t cpu, int64_t mem,
                           int64_t disk, int64_t flash, StatusCode* status,
                           UserAddAppCallback* callback) {
  AddAppRequest* request = new AddAppRequest();
  request->set_group_name(group_name);
  request->set_app_name(app_name);
  request->set_app_id(app_id);
  request->set_host(host);
  request->set_cpu(cpu);
  request->set_mem(mem);
  request->set_disk(disk);
  request->set_flash(flash);

  AddAppResponse* response = new AddAppResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::AddAppCallback, cntl,
                            request, response, status, callback, &cond);

  stub_->AddApp(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::DelApp(const std::string& group_name, const std::string& app_name,
                      const std::string& app_id, StatusCode* status,
                      UserDelAppCallback* callback) {
  DelAppRequest* request = new DelAppRequest();
  request->set_group_name(group_name);
  request->set_app_name(app_name);
  request->set_app_id(app_id);

  DelAppResponse* response = new DelAppResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::DelAppCallback, cntl,
                            request, response, status, callback, &cond);

  stub_->DelApp(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}


void HeimdallrImpl::AddUser(const std::string& user_name,
                            const std::string& passwd,
                            StatusCode* status,
                            UserAddUserCallback* callback) {
  AddUserRequest* request = new AddUserRequest();
  request->set_user_name(user_name);
  request->set_passwd(passwd);

  AddUserResponse* response = new AddUserResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::AddUserCallback, cntl,
                            request, response, status, callback, &cond);

  stub_->AddUser(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::AddUserToGroup(const std::string& user_name,
                                   const std::string& group_name,
                                   StatusCode* status,
                                   UserAddUserToGroupCallback* callback) {
  AddUserToGroupRequest* request = new AddUserToGroupRequest();
  request->set_user_name(user_name);
  request->set_group_name(group_name);

  AddUserToGroupResponse* response = new AddUserToGroupResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::AddUserToGroupCallback,
                            cntl, request, response, status, callback, &cond);

  stub_->AddUserToGroup(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::ListUser(const std::string& user_name,
                             std::string* user_info,
                             StatusCode* status,
                             UserListUserCallback* callback) {
  ListUserRequest* request = new ListUserRequest();
  request->set_user_name(user_name);

  ListUserResponse* response = new ListUserResponse();
  sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
  cntl->SetTimeout(3000);

  Mutex mutex;
  CondVar cond(&mutex);
  google::protobuf::Closure* done =
    sofa::pbrpc::NewClosure(this, &HeimdallrImpl::ListUserCallback, cntl,
                            request, response, user_info, status,
                            callback, &cond);

  stub_->ListUser(cntl, request, response, done);
  if (!callback) {
    cond.Wait();
  }
}

void HeimdallrImpl::IdentifyUser(const std::string& user_name,
                            const std::string& token,
                            std::string* group_name, StatusCode* status,
                            UserIdentifyUserCallback* callback) {

}

void HeimdallrImpl::AddServiceCallback(sofa::pbrpc::RpcController* cntl,
                                       AddServiceRequest* request,
                                       AddServiceResponse* response,
                                       StatusCode* status,
                                       UserAddServiceCallback* callback,
                                       CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->service_name(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::SetServiceResourceCallback(sofa::pbrpc::RpcController* cntl,
                                               SetServiceResourceRequest* request,
                                               SetServiceResourceResponse* response,
                                               StatusCode* status,
                                               UserSetServiceResourceCallback* callback,
                                               CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->service_name(), request->resource_name(),
                request->provider_name(), request->quantity(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::ListServiceCallback(sofa::pbrpc::RpcController* cntl,
                                        ListServiceRequest* request,
                                        ListServiceResponse* response,
                                        std::string* service_info,
                                        StatusCode* status,
                                        UserListServiceCallback* callback,
                                        CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();
  *service_info = response->service_info();

  if (callback) {
    (*callback)(request->service_name(), service_info, status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;

}

void HeimdallrImpl::AddGroupCallback(sofa::pbrpc::RpcController* cntl,
                                     AddGroupRequest* request,
                                     AddGroupResponse* response,
                                     StatusCode* status,
                                     UserAddGroupCallback* callback,
                                     CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->group_name(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}
void HeimdallrImpl::SetGroupQuotaCallback(sofa::pbrpc::RpcController* cntl,
                                          SetGroupQuotaRequest* request,
                                          SetGroupQuotaResponse* response,
                                          StatusCode* status,
                                          UserSetGroupQuotaCallback* callback,
                                          CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->group_name(), request->service_name(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}
void HeimdallrImpl::ListGroupCallback(sofa::pbrpc::RpcController* cntl,
                                      ListGroupRequest* request,
                                      ListGroupResponse* response,
                                      std::string* group_info,
                                      StatusCode* status,
                                      UserListGroupCallback* callback,
                                      CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();
  *group_info = response->group_info();

  if (callback) {
    (*callback)(request->group_name(), group_info, status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::AddAppCallback(sofa::pbrpc::RpcController* cntl,
                                 AddAppRequest* request,
                                 AddAppResponse* response,
                                 StatusCode* status,
                                 UserAddAppCallback* callback, CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->group_name(), request->app_name(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::DelAppCallback(sofa::pbrpc::RpcController* cntl,
                                   DelAppRequest* request,
                                   DelAppResponse* response,
                                   StatusCode* status,
                                   UserDelAppCallback* callback, CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->group_name(), request->app_name(), request->app_id(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::AddUserCallback(sofa::pbrpc::RpcController* cntl,
                                    AddUserRequest* request,
                                    AddUserResponse* response,
                                    StatusCode* status,
                                    UserAddUserCallback* callback,
                                    CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->user_name(), request->passwd(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::AddUserToGroupCallback(sofa::pbrpc::RpcController* cntl,
                                           AddUserToGroupRequest* request,
                                           AddUserToGroupResponse* response,
                                           StatusCode* status,
                                           UserAddUserToGroupCallback* callback,
                                           CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();

  if (callback) {
    (*callback)(request->user_name(), request->group_name(), status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::ListUserCallback(sofa::pbrpc::RpcController* cntl,
                                     ListUserRequest* request,
                                     ListUserResponse* response,
                                     std::string* user_info,
                                     StatusCode* status,
                                     UserListUserCallback* callback,
                                     CondVar* cond) {
  if (cntl->Failed()) {
    SLOG(ERROR, "rpc failed: %s", cntl->ErrorText().c_str());
  }
  *status = response->status();
  *user_info = response->user_info();

  if (callback) {
    (*callback)(request->user_name(), user_info, status);
    delete callback;
  } else {
    cond->Signal();
  }

  delete cntl;
  delete request;
  delete response;
}

void HeimdallrImpl::IdentifyUserCallback(const std::string& user_name,
                                    const std::string& token,
                                    std::string* group_name, StatusCode* status,
                                    UserIdentifyUserCallback* callback) {

}


} // namespace sdk
} // namespace heimdallr
} // namespace baidu
