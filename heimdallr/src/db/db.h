// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DB_DB_H
#define DB_DB_H

#include <string>

#include <sofa/pbrpc/pbrpc.h>
#include <leveldb/db.h>
#include <logging.h>

#include "src/proto/status_code.pb.h"

namespace baidu {
namespace heimdallr {
namespace db {

typedef server::Service Service;
typedef server::Resource Resource;
typedef server::Provider Provider;
typedef server::Group Group;
typedef server::UsedBy UsedBy;
typedef server::Quota Quota;
typedef server::User User;
typedef server::History History;
typedef server::App App;

class DB {
public:
  DB();
  StatusCode init();
  ////// Service //////
  StatusCode AddService(const std::string& service_name);
  StatusCode SetServiceResource(const std::string& service_name,
                                const std::string& resource_name,
                                const std::string& provider_name,
                                int64_t quantity);
  StatusCode ListService(const std::string& service_name, Service* services,
                         std::string* service_info);

  ////// Group //////
  StatusCode AddGroup(const std::string& group_name);
  StatusCode SetGroupQuota(const std::string& group_name,
                           const std::string& service_name,
                           int64_t host, int64_t cpu, int64_t mem,
                           int64_t disk, int64_t flash);
  StatusCode ListGroup(const std::string& group_name, Group* group, std::string* group_info);
  StatusCode AddApp(const std::string& group_name, const std::string& app_name,
                    const std::string& app_id, int64_t host, int64_t cpu, int64_t mem,
                    int64_t disk, int64_t flash);
  StatusCode DelApp(const std::string& group_name, const std::string& app_name,
                    const std::string& app_id);

  ////// User //////
  StatusCode AddUser(const std::string& user_name, const std::string& passwd);
  StatusCode AddUserToGroup(const std::string& user_name,
                            const std::string& group_name);
  StatusCode ListUser(const std::string& user_name, User* user, std::string* user_info);

private:
  StatusCode GetService(const std::string& service_name, Service* service);
  StatusCode GetResource(Service* service, const std::string& resource_name, Resource** resource);
  StatusCode GetGroup(const std::string& group_name, Group* group);
  //StatusCode GetQuota(Group* group, const std::string resource_name, Quota** quota);
  StatusCode GetUser(const std::string& user_name, User* user);
  StatusCode GetUserInGroup(const Group* group, const std::string& user_name);
  StatusCode GetApp(const Group* group, const std::string& app_name, const std::string& app_id);

  StatusCode ParseResource(const Resource&, std::string* res);

private:
  common::LogStream* logger_;
  leveldb::DB* group_db_;
  leveldb::DB* service_db_;
  leveldb::DB* user_db_;
  const leveldb::ReadOptions roption_;
  const leveldb::WriteOptions woption_;
};

} // namespace server
} // namespace heimdallr
} // namespace baidu

#endif // DB_DB_H
