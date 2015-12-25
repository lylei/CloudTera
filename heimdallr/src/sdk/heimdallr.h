// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SDK_HEIMDALLR_H
#define SDK_HEIMDALLR_H

#include <string>

#include <boost/function.hpp>

#include "src/proto/server_rpc.pb.h"
#include "src/proto/status_code.pb.h"

namespace baidu {
namespace heimdallr {
namespace sdk {

////// Service //////
typedef boost::function<void (const std::string&, StatusCode*)>
        UserAddServiceCallback;
typedef boost::function<void (const std::string&, const std::string&,
                              const std::string&, int64_t, StatusCode*)>
        UserSetServiceResourceCallback;
typedef boost::function<void (const std::string&, std::string*, StatusCode*)>
        UserListServiceCallback;

////// Group //////
typedef boost::function<void (const std::string&, StatusCode*)>
        UserAddGroupCallback;
typedef boost::function<void (const std::string&, const std::string&, StatusCode*)>
        UserSetGroupQuotaCallback;
typedef boost::function<void (const std::string&, std::string*, StatusCode*)>
        UserListGroupCallback;
typedef boost::function<void (const std::string&, const std::string&, StatusCode*)>
        UserAddAppCallback;
typedef boost::function<void (const std::string&, const std::string&, const std::string&, StatusCode*)>
        UserDelAppCallback;

////// User //////
typedef boost::function<void (const std::string&, const std::string&, StatusCode*)>
        UserAddUserCallback;
typedef boost::function<void (const std::string&, const std::string&, StatusCode*)>
        UserAddUserToGroupCallback;
typedef boost::function<void (const std::string&, std::string*, StatusCode*)>
        UserListUserCallback;
typedef boost::function<void (const std::string&, const std::string&,
                              std::string*, StatusCode*)>
        UserIdentifyUserCallback;

//typedef ::google::protobuf::RepeatedPtrField<baidu::heimdallr::server::Service> ServiceList;


class Heimdallr {
public:
  virtual ~Heimdallr() {}
  virtual StatusCode init() = 0;

  ////// Service //////
  virtual void AddService(const std::string& service_name, StatusCode* status,
                          UserAddServiceCallback* callback) = 0;
  virtual void SetServiceResource(const std::string& service_name,
                                  const std::string& resource_name,
                                  const std::string& provider_name,
                                  int64_t quantity, StatusCode* status,
                                  UserSetServiceResourceCallback* callback) = 0;
  virtual void ListService(const std::string& service_name,
                           std::string* service_info,
                           StatusCode* status,
                           UserListServiceCallback* callback) = 0;

  ////// Group //////
  virtual void AddGroup(const std::string& group_name, StatusCode* status,
                        UserAddGroupCallback* callback) = 0;
  virtual void SetGroupQuota(const std::string& group_name,
                             const std::string& service_name, int64_t host, int64_t cpu,
                             int64_t mem, int64_t disk, int64_t flash,
                             StatusCode* status, UserSetGroupQuotaCallback* callback) = 0;
  virtual void ListGroup(const std::string& group_name, std::string* group_info,
                         StatusCode* status, UserListGroupCallback* callback) = 0;
  virtual void AddApp(const std::string& group_name, const std::string& app_name,
                      const std::string& app_id, int64_t host, int64_t cpu, int64_t mem,
                      int64_t disk, int64_t flash, StatusCode* status,
                      UserAddAppCallback* callback) = 0;
  virtual void DelApp(const std::string& group_name, const std::string& app_name,
                      const std::string& app_id, StatusCode* status,
                      UserDelAppCallback* callback) = 0;

  ////// User //////
  virtual void AddUser(const std::string& user_name, const std::string& passwd,
                       StatusCode* status, UserAddUserCallback* callback) = 0;
  virtual void AddUserToGroup(const std::string& user_name,
                              const std::string& group_name,
                              StatusCode* status,
                              UserAddUserToGroupCallback* callback) = 0;
  virtual void ListUser(const std::string& user_name,
                        std::string* user_info, StatusCode* status,
                        UserListUserCallback* callback) = 0;
  virtual void IdentifyUser(const std::string& user_name,
                            const std::string& token,
                            std::string* group_name, StatusCode* status,
                            UserIdentifyUserCallback* callback) = 0;
};

} // namespace sdk
} // namespace heimdallr
} // namespace baidu

#endif // SDK_HEIMDALLR_H
