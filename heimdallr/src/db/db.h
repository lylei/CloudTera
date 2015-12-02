// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DB_DB_H
#define DB_DB_H

#include <string>

#include <sofa/pbrpc/pbrpc.h>
#include <leveldb/db.h>

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

class DB {
public:
  DB();
  StatusCode init();
  ////// Service //////
  StatusCode AddService(const std::string& service_name);
  StatusCode SetServiceResource(const std::string& service_name,
                                const std::string& resource_name,
                                const std::string& provider_name,
                                uint64_t quantity);
  StatusCode ListService(const std::string& service_name, std::string* services);

  ////// Group //////
  StatusCode AddGroup(const std::string& group_name);
  StatusCode SetGroupQuota(const std::string& group_name,
                           const std::string& service_name,
                           const std::string& resource_name,
                           uint64_t quota);
  StatusCode ListGroup(const std::string& group_name, std::string* group_info);

  ////// User //////
  StatusCode AddUser(const std::string& user_name, const std::string& passwd);
  StatusCode AddUserToGroup(const std::string& user_name,
                            const std::string& group_name);
  StatusCode ListUser(const std::string& user_name, std::string* user_info);

private:
  StatusCode GetService(const std::string& service_name, Service* service);
  StatusCode GetGroup(const std::string& group_name, Group* group);
  StatusCode GetUser(const std::string& user_name, User* user);

private:
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
