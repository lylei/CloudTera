// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVER_HEIMDALLR_SERVER_H
#define SERVER_HEIMDALLR_SERVER_H

#include <sofa/pbrpc/pbrpc.h>
#include <mutex.h>

#include "src/proto/server_rpc.pb.h"

#include "src/db/db.h"

namespace baidu {
namespace heimdallr {
namespace server {

class ServerImpl : public Server {
public:
  ServerImpl();
  virtual ~ServerImpl() {}

private:
  ////// Service //////
  virtual void AddService(google::protobuf::RpcController* controller,
                          const AddServiceRequest* request,
                          AddServiceResponse* response,
                          google::protobuf::Closure* done);
  virtual void SetServiceResource(google::protobuf::RpcController* controller,
                                  const SetServiceResourceRequest* request,
                                  SetServiceResourceResponse* response,
                                  google::protobuf::Closure* done);
  virtual void ListService(google::protobuf::RpcController* controller,
                          const ListServiceRequest* request,
                          ListServiceResponse* response,
                          google::protobuf::Closure* done);

  ////// Group //////
  virtual void AddGroup(google::protobuf::RpcController* controller,
                        const AddGroupRequest* request,
                        AddGroupResponse* response,
                        google::protobuf::Closure* done);
  virtual void SetGroupQuota(google::protobuf::RpcController* controller,
                             const SetGroupQuotaRequest* request,
                             SetGroupQuotaResponse* response,
                             google::protobuf::Closure* done);
  virtual void ListGroup(google::protobuf::RpcController* controller,
                         const ListGroupRequest* request,
                         ListGroupResponse* response,
                         google::protobuf::Closure* done);
  virtual void AddApp(google::protobuf::RpcController* controller,
                         const AddAppRequest* request,
                         AddAppResponse* response,
                         google::protobuf::Closure* done);
  virtual void DelApp(google::protobuf::RpcController* controller,
                         const DelAppRequest* request,
                         DelAppResponse* response,
                         google::protobuf::Closure* done);

  ////// User //////
  virtual void AddUser(google::protobuf::RpcController* controller,
                       const AddUserRequest* request,
                       AddUserResponse* response,
                       google::protobuf::Closure* done);
  virtual void AddUserToGroup(google::protobuf::RpcController* controller,
                       const AddUserToGroupRequest* request,
                       AddUserToGroupResponse* response,
                       google::protobuf::Closure* done);
  virtual void ListUser(google::protobuf::RpcController* controller,
                       const ListUserRequest* request,
                       ListUserResponse* response,
                       google::protobuf::Closure* done);
private:
  Mutex mutex_;
  db::DB* db_;
};

} // namespace server
} // namespace heimdallr
} // namespace baidu

#endif // SERVER_SQUIRREL_SERVER_H
