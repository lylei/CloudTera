// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <sofa/pbrpc/pbrpc.h>
#include <iostream>

#include "src/server/server.h"

namespace baidu {
namespace heimdallr {
namespace server {

ServerImpl::ServerImpl() : db_(new db::DB()) { }

////// Service //////
void ServerImpl::AddService(google::protobuf::RpcController* controller,
                            const AddServiceRequest* request,
                            AddServiceResponse* response,
                            google::protobuf::Closure* done) {
  SLOG(INFO, "receive AddService request: %s", request->service_name().c_str());

  StatusCode status = db_->AddService(request->service_name());

  response->set_status(status);
  done->Run();
}

void ServerImpl::SetServiceResource(google::protobuf::RpcController* controller,
                                    const SetServiceResourceRequest* request,
                                    SetServiceResourceResponse* response,
                                    google::protobuf::Closure* done) {
  SLOG(INFO, "receive SetServiceResource request: %s %s %s %llu",
       request->service_name().c_str(), request->resource_name().c_str(),
       request->provider_name().c_str(), request->quantity());

  StatusCode status = db_->SetServiceResource(request->service_name().c_str(),
                                              request->resource_name().c_str(),
                                              request->provider_name().c_str(),
                                              request->quantity());

  response->set_status(status);
  done->Run();
}

void ServerImpl::ListService(google::protobuf::RpcController* controller,
                     const ListServiceRequest* request,
                     ListServiceResponse* response,
                     google::protobuf::Closure* done) {
  SLOG(INFO, "receive ListService request: %s", request->service_name().c_str());

  StatusCode status = db_->ListService(request->service_name(),
                                       response->mutable_services(),
                                       response->mutable_service_info());

  response->set_status(status);
  done->Run();
}

////// Group //////
void ServerImpl::AddGroup(google::protobuf::RpcController* controller,
                          const AddGroupRequest* request,
                          AddGroupResponse* response,
                          google::protobuf::Closure* done) {
  SLOG(INFO, "receive AddGroup request: %s", request->group_name().c_str());

  StatusCode status = db_->AddGroup(request->group_name());

  response->set_status(status);
  done->Run();
}

void ServerImpl::SetGroupQuota(google::protobuf::RpcController* controller,
                               const SetGroupQuotaRequest* request,
                               SetGroupQuotaResponse* response,
                               google::protobuf::Closure* done) {
  SLOG(INFO, "receive SetGroupQuota request: %s %s %s %llu",
       request->group_name().c_str(), request->service_name().c_str(),
       request->resource_name().c_str(), request->quota());

  StatusCode status = db_->SetGroupQuota(request->group_name(),
                                         request->service_name(),
                                         request->resource_name(),
                                         request->quota());

  response->set_status(status);
  done->Run();
}

void ServerImpl::ListGroup(google::protobuf::RpcController* controller,
                           const ListGroupRequest* request,
                           ListGroupResponse* response,
                           google::protobuf::Closure* done) {
  SLOG(INFO, "receive ListGroup request: %s", request->group_name().c_str());

  StatusCode status = db_->ListGroup(request->group_name(),
                                     response->mutable_group(),
                                     response->mutable_group_info());

  response->set_status(status);
  done->Run();
}

void ServerImpl::AddApp(google::protobuf::RpcController* controller,
                         const AddAppRequest* request,
                         AddAppResponse* response,
                         google::protobuf::Closure* done) {
  SLOG(INFO, "receive AddApp request: %s", request->group_name().c_str());

  StatusCode status = db_->AddApp(request->group_name(),
                                     request->app_name(),
                                     request->cpu(),
                                     request->mem(),
                                     request->disk(),
                                     request->flash());

  response->set_status(status);
  done->Run();
}

////// User //////
void ServerImpl::AddUser(google::protobuf::RpcController* controller,
                         const AddUserRequest* request,
                         AddUserResponse* response,
                         google::protobuf::Closure* done) {
  SLOG(INFO, "receive AddUser request: %s %s", request->user_name().c_str(),
       request->passwd().c_str());

  StatusCode status = db_->AddUser(request->user_name(), request->passwd());

  response->set_status(status);
  done->Run();
}

void ServerImpl::AddUserToGroup(google::protobuf::RpcController* controller,
                                const AddUserToGroupRequest* request,
                                AddUserToGroupResponse* response,
                                google::protobuf::Closure* done) {
  SLOG(INFO, "receive AddUserToGroup request: %s %s",
       request->user_name().c_str(), request->group_name().c_str());

  StatusCode status = db_->AddUserToGroup(request->user_name(),
                                          request->group_name());

  response->set_status(status);
  done->Run();
}

void ServerImpl::ListUser(google::protobuf::RpcController* controller,
                          const ListUserRequest* request,
                          ListUserResponse* response,
                          google::protobuf::Closure* done) {
  SLOG(INFO, "receive ListUser request: %s", request->user_name().c_str());

  StatusCode status = db_->ListUser(request->user_name(),
                                    response->mutable_user(),
                                    response->mutable_user_info());

  response->set_status(status);
  done->Run();
}

} // namespace server
} // namespace heimdallr
} // namespace baidu
