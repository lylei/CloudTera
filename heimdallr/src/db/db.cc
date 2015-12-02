// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>
#include <vector>

#include <leveldb/status.h>

#include "src/proto/server_rpc.pb.h"
#include "db.h"

extern std::string CONF_GROUP_DB;
extern std::string CONF_SERVICE_DB;
extern std::string CONF_USER_DB;

namespace baidu {
namespace heimdallr {
namespace db {

DB::DB() {
  init();
}

StatusCode DB::init() {
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, CONF_GROUP_DB, &group_db_);
  if (status.ok()) {
    std::cout << CONF_GROUP_DB << " init OK\n";
  } else {
    std::cout << status.ToString() << std::endl;
    return kDBInitError;
  }

  status = leveldb::DB::Open(options, CONF_SERVICE_DB, &service_db_);
  if (status.ok()) {
    std::cout << CONF_SERVICE_DB << " init OK\n";
  } else {
    std::cout << status.ToString() << std::endl;
    return kDBInitError;
  }

  status = leveldb::DB::Open(options, CONF_USER_DB, &user_db_);
  if (status.ok()) {
    std::cout << CONF_USER_DB << " init OK\n";
  } else {
    std::cout << status.ToString() << std::endl;
    return kDBInitError;
  }
  return kOK;
}

StatusCode DB::AddService(const std::string& service_name) {
  std::cout << "in db AddService\n";
  std::string res;
  leveldb::Status status = service_db_->Get(roption_, service_name, &res);
  if (!status.IsNotFound()) {
    return kServiceExists;
  }
  Service service;
  service.set_service_name(service_name);

  res.clear();
  service.SerializeToString(&res);
  std::cout << "debug string: " << service.ShortDebugString() << std::endl;

  status = service_db_->Put(woption_, service_name, res);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return kDBWriteError;
  }
  std::cout << res << std::endl;
  return kOK;
}

StatusCode DB::SetServiceResource(const std::string& service_name,
                                  const std::string& resource_name,
                                  const std::string& provider_name,
                                  uint64_t quantity) {
  std::cout << "in db SetServiceResource\n";
  Service service;
  StatusCode status =  GetService(service_name, &service);
  if (status != kOK) {
    return status;
  }
  std::cout << "get service: " << service.ShortDebugString() << std::endl;

  // update provider info
  uint32_t provider_num = service.providers_size();
  std::cout << "provider_num:" << provider_num << std::endl;
  bool found = false;
  for (int i = 0; i < provider_num; ++i) {
    Provider* provider = service.mutable_providers(i);
    if (provider->resource_name() == resource_name &&
        provider->name() == provider_name) {
      found = true;
      provider->set_quantity(provider->quantity() + quantity);
      break;
    } else {
      std::cout << "old res:" << provider->resource_name() << ":" << resource_name
                << "\n old name:" << provider->name() << ":" << provider_name
                << std::endl;
    }
  }
  if (!found) {
    Provider* provider = service.add_providers();
    provider->set_resource_name(resource_name);
    provider->set_name(provider_name);
    provider->set_quantity(quantity);
    std::cout << "new provider: " << provider->ShortDebugString() << std::endl;
  }

  // update resource info
  found = false;
  uint32_t resource_num = service.resources_size();
  for (int i = 0; i < resource_num; ++i) {
    Resource* resource = service.mutable_resources(i);
    if (resource->resource_name() == resource_name) {
      found = true;
      resource->set_capacity(resource->capacity() + quantity);
      History* history = resource->add_history();
      history->set_capacity(resource->capacity());
      history->set_ts(history->ts() + 1);
      std::cout << "set resource: " << resource->ShortDebugString() << std::endl;
      break;
    }
  }
  if (!found) {
    Resource* resource = service.add_resources();
    resource->set_resource_name(resource_name);
    resource->set_capacity(quantity);
    resource->set_left(quantity);

    History* history = resource->add_history();
    history->set_capacity(resource->capacity());
    history->set_ts(0);
  }
  std::string result_str;
  service.SerializeToString(&result_str);
  std::cout << "resutl str: " << service.ShortDebugString() << std::endl;
  leveldb::Status s = service_db_->Put(woption_, service_name, result_str);
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
    return kDBWriteError;
  }

  return kOK;
}

StatusCode DB::ListService(const std::string& service_name, std::string* services) {
  std::cout << "in db ListService\n";
  std::string service_str;
  leveldb::Status status = service_db_->Get(roption_, service_name, &service_str);
  *services = service_str;
  if (status.IsNotFound()) {
    return kServiceNotFound;
  }
  Service s;
  s.ParseFromString(service_str);
  *services = s.ShortDebugString();
  std::cout << *services << std::endl;
  return kOK;
}

StatusCode DB::AddGroup(const std::string& group_name) {
  std::cout << "in db AddSAddGroupervice\n";
  std::string res;
  leveldb::Status status = group_db_->Get(roption_, group_name, &res);
  if (!status.IsNotFound()) {
    return kGroupExists;
  }
  Group group;
  group.set_group_name(group_name);

  res.clear();
  group.SerializeToString(&res);
  std::cout << "debug string: " << group.ShortDebugString() << std::endl;

  status = group_db_->Put(woption_, group_name, res);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return kDBWriteError;
  }
  std::cout << res << std::endl;
  return kOK;
}

StatusCode DB::SetGroupQuota(const std::string& group_name,
                             const std::string& service_name,
                             const std::string& resource_name,
                             uint64_t quota) {
  std::cout << "in db SetGroupQuota\n";
  Service service;
  StatusCode s = GetService(service_name, &service);
  if (s != kOK) {
    return s;
  }
  Group group;
  s = GetGroup(group_name, &group);
  if (s != kOK) {
    return s;
  }

  // update resource
  bool found = false;
  uint32_t resource_num = service.resources_size();
  for (int i = 0; i < resource_num; ++i) {
    Resource* resource = service.mutable_resources(i);
    if (resource->resource_name() == resource_name) {
      found = true;
      if (resource->left() < quota) {
        return kResourceRunOut;
      } else {
        UsedBy* used_by = resource->add_used_by();
        used_by->set_group_name(group_name);
        used_by->set_quantity(quota);
        resource->set_left(resource->left() - quota);
        std::cout << resource->ShortDebugString() << std::endl;

        std::string result_str;
        service.SerializeToString(&result_str);
        std::cout << "resutl str: " << service.ShortDebugString() << std::endl;
        leveldb::Status s = service_db_->Put(woption_, service_name, result_str);
        if (!s.ok()) {
          std::cout << s.ToString() << std::endl;
          return kDBWriteError;
        }
      }
      break;
    }
  }
  if (!found) {
    return kResourceNotFound;
  }

  // update group info
  Quota* group_quota = group.add_quota();
  group_quota->set_service_name(service_name);
  group_quota->set_resource_name(resource_name);
  group_quota->set_quota(quota);

  std::string result_str;
  group.SerializeToString(&result_str);
  std::cout << "resutl str: " << group.ShortDebugString() << std::endl;
  leveldb::Status status = group_db_->Put(woption_, group_name, result_str);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return kDBWriteError;
  }

  return kOK;
}

StatusCode DB::ListGroup(const std::string& group_name, std::string* group_info) {
  std::cout << "in db ListGroup\n";
  std::string group_str;
  leveldb::Status status = group_db_->Get(roption_, group_name, &group_str);
  if (status.IsNotFound()) {
    return kServiceNotFound;
  }
  Group g;
  g.ParseFromString(group_str);
  *group_info = g.ShortDebugString();
  std::cout << *group_info << std::endl;
  return kOK;
}

StatusCode DB::AddUser(const std::string& user_name, const std::string& passwd) {
  std::cout << "in db AddUser\n";
  std::string res;
  leveldb::Status status = user_db_->Get(roption_, user_name, &res);
  if (!status.IsNotFound()) {
    return kUserExists;
  }
  User user;
  user.set_user_name(user_name);
  user.set_token(passwd);

  res.clear();
  user.SerializeToString(&res);
  std::cout << "debug string: " << user.ShortDebugString() << std::endl;

  status = user_db_->Put(woption_, user_name, res);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return kDBWriteError;
  }
  std::cout << res << std::endl;
  return kOK;
}

StatusCode DB::AddUserToGroup(const std::string& user_name,
                              const std::string& group_name) {
  Group group;
  StatusCode s = GetGroup(group_name, &group);
  if (s != kOK) {
    return s;
  }
  User user;
  s = GetUser(user_name, &user);
  if (s!= kOK) {
    return s;
  }

  group.add_users(user_name);
  std::string result_str;
  group.SerializeToString(&result_str);
  std::cout << "resutl str: " << group.ShortDebugString() << std::endl;
  leveldb::Status status = group_db_->Put(woption_, group_name, result_str);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return kDBWriteError;
  }

  user.add_groups(group_name);
  user.SerializeToString(&result_str);
  std::cout << "resutl str: " << user.ShortDebugString() << std::endl;
  status = user_db_->Put(woption_, user_name, result_str);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return kDBWriteError;
  }
  return kOK;
}

StatusCode DB::ListUser(const std::string& user_name, std::string* user_info) {
  std::cout << "in db ListUser:" << user_name << std::endl;
  std::string user_str;
  leveldb::Status status = user_db_->Get(roption_, user_name, &user_str);
  if (status.IsNotFound()) {
    return kUserNotFound;
  }
  User u;
  u.ParseFromString(user_str);
  *user_info += "user_name: " + u.user_name() + "\n";
  uint32_t group_num = u.groups_size();
  for (uint32_t i = 0; i < group_num; ++i) {
    user_info->append(u.groups(i));
    user_info->append(";");
  }
  std::cout << *user_info << std::endl;
  std::cout << u.ShortDebugString() << std::endl;
  return kOK;
}

StatusCode DB::GetService(const std::string& service_name, Service* service) {
  std::cout << "in db GetService:" << service_name << std::endl;
  std::string service_str;
  leveldb::Status status = service_db_->Get(roption_, service_name, &service_str);
  if (status.IsNotFound()) {
    return kServiceNotFound;
  } else if (!status.ok()) {
    return kDBReadError;
  }
  service->ParseFromString(service_str);
  return kOK;
}

StatusCode DB::GetGroup(const std::string& group_name, Group* group) {
  std::cout << "in db GetGroup:" << group_name << std::endl;
  std::string group_str;
  leveldb::Status status = group_db_->Get(roption_, group_name, &group_str);
  if (status.IsNotFound()) {
    return kServiceNotFound;
  } else if (!status.ok()) {
    return kDBReadError;
  }
  group->ParseFromString(group_str);
  return kOK;
}

StatusCode DB::GetUser(const std::string& user_name, User* user) {
  std::cout << "in db GetUser\n";
  std::string user_str;
  leveldb::Status status = user_db_->Get(roption_, user_name, &user_str);
  if (status.IsNotFound()) {
    return kUserNotFound;
  } else if (!status.ok()) {
    return kDBReadError;
  }
  user->ParseFromString(user_str);
  return kOK;
}

} // namespace server
} // namespace heimdallr
} // namespace baidu
