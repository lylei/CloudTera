// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>
#include <vector>

#include <leveldb/status.h>
#include <timer.h>

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
  logger_ = new common::LogStream(INFO);
  common::SetLogFile("./dblog");

  LOGS(INFO) << "log init ok!";
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, CONF_GROUP_DB, &group_db_);
  if (status.ok()) {
    LOGS(INFO) << CONF_GROUP_DB << " init OK\n";
  } else {
    LOGS(INFO) << status.ToString();
    return kDBInitError;
  }

  status = leveldb::DB::Open(options, CONF_SERVICE_DB, &service_db_);
  if (status.ok()) {
    LOGS(INFO) << CONF_SERVICE_DB << " init OK\n";
  } else {
    LOGS(INFO) << status.ToString();
    return kDBInitError;
  }

  status = leveldb::DB::Open(options, CONF_USER_DB, &user_db_);
  if (status.ok()) {
    LOGS(INFO) << CONF_USER_DB << " init OK\n";
  } else {
    LOGS(INFO) << status.ToString();
    return kDBInitError;
  }
  return kOK;
}

StatusCode DB::AddService(const std::string& service_name) {
  LOGS(INFO) << "in db AddService\n";
  std::string res;
  leveldb::Status status = service_db_->Get(roption_, service_name, &res);
  if (!status.IsNotFound()) {
    return kServiceExists;
  }
  Service service;
  service.set_service_name(service_name);

  res.clear();
  service.SerializeToString(&res);
  LOGS(INFO) << "debug string: " << service.ShortDebugString();

  status = service_db_->Put(woption_, service_name, res);
  if (!status.ok()) {
    LOGS(INFO) << status.ToString();
    return kDBWriteError;
  }
  LOGS(INFO) << res;
  return kOK;
}

StatusCode DB::SetServiceResource(const std::string& service_name,
                                  const std::string& resource_name,
                                  const std::string& provider_name,
                                  int64_t quantity) {
  LOGS(INFO) << "in db SetServiceResource\n";
  Service service;
  StatusCode status =  GetService(service_name, &service);
  if (status != kOK) {
    return status;
  }
  LOGS(INFO) << "get service: " << service.ShortDebugString();

  // update provider info
  Provider* provider = service.add_providers();
  provider->set_resource_name(resource_name);
  provider->set_name(provider_name);
  provider->set_quantity(quantity);
  provider->set_ts(common::timer::now_time());
  LOGS(INFO) << "new provider: " << provider->ShortDebugString();

  // update resource info
  Resource* resource;
  StatusCode rs = GetResource(&service, resource_name, &resource);
  if (rs == kOK) {
    if (resource->left() + quantity < 0) {
      return kResourceRunOut;
    }
    resource->set_capacity(resource->capacity() + quantity);
    resource->set_left(resource->left() + quantity);
    History* history = resource->add_history();
    history->set_capacity(resource->capacity());
    history->set_ts(common::timer::now_time());
    LOGS(INFO) << "set resource: " << resource->ShortDebugString();
  } else if (rs == kResourceNotFound) {
    resource = service.add_resources();
    resource->set_resource_name(resource_name);
    resource->set_capacity(quantity);
    resource->set_left(quantity);

    History* history = resource->add_history();
    history->set_capacity(resource->capacity());
    history->set_ts(common::timer::now_time());
  } else {
    LOGS(WARNING) << "Error in SetServiceResource::GetResource: " << StatusCode_Name(rs);
    return rs;
  }

  std::string result_str;
  service.SerializeToString(&result_str);
  LOGS(INFO) << "resutl str: " << service.ShortDebugString();
  leveldb::Status s = service_db_->Put(woption_, service_name, result_str);
  if (!s.ok()) {
    LOGS(INFO) << s.ToString();
    return kDBWriteError;
  }

  return kOK;
}

StatusCode DB::ListService(const std::string& service_name, Service* services,
                           std::string* service_info) {
  LOGS(INFO) << "in db ListService\n";
  std::string service_str;
  leveldb::Status status = service_db_->Get(roption_, service_name, &service_str);
  //*services = service_str;
  if (status.IsNotFound()) {
    return kServiceNotFound;
  }
  Service s;
  s.ParseFromString(service_str);
  *services = s;
  *service_info = s.ShortDebugString();
  //LOGS(INFO) << *services;
  return kOK;
}

StatusCode DB::AddGroup(const std::string& group_name) {
  LOGS(INFO) << "in db AddSAddGroupervice\n";
  std::string res;
  leveldb::Status status = group_db_->Get(roption_, group_name, &res);
  if (!status.IsNotFound()) {
    return kGroupExists;
  }
  Group group;
  group.set_group_name(group_name);

  res.clear();
  group.SerializeToString(&res);
  LOGS(INFO) << "debug string: " << group.ShortDebugString();

  status = group_db_->Put(woption_, group_name, res);
  if (!status.ok()) {
    LOGS(INFO) << status.ToString();
    return kDBWriteError;
  }
  LOGS(INFO) << res;
  return kOK;
}

StatusCode DB::SetGroupQuota(const std::string& group_name,
                             const std::string& service_name,
                             int64_t host, int64_t cpu, int64_t mem,
                             int64_t disk, int64_t flash) {
  LOGS(INFO) << "in db SetGroupQuota\n";
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
  Resource* resource;
  std::string resource_names[5] = {"host", "cpu", "mem", "disk", "flash"};
  int64_t quotas[5] = {host, cpu, mem, disk, flash};
  for (int i = 0; i < 5; ++i) {
    const std::string& resource_name = resource_names[i];
    int64_t quota = quotas[i];
    if (quota == 0) {
      continue;
    }
    StatusCode rs = GetResource(&service, resource_name, &resource);
    if (rs == kOK) {
      if (resource->left() < quota) {
        LOGS(WARNING) << "run out: " << resource->left() << ":" << quota;
        return kResourceRunOut;
      } else {
        UsedBy* used_by = resource->add_used_by();
        used_by->set_group_name(group_name);
        used_by->set_quantity(quota);
        used_by->set_ts(common::timer::now_time());
        resource->set_left(resource->left() - quota);
        LOGS(INFO) << resource->ShortDebugString();
      }
    } else if (rs == kResourceNotFound) {
      return kResourceNotFound;
    } else {
      LOGS(WARNING) << "Error in SetServiceResource::GetResource: " << StatusCode_Name(rs);
      return rs;
    }
  }

  // update group info
  if (group.host_left() + host >= 0 && group.cpu_left() + cpu >= 0 &&
      group.mem_left() + mem >= 0 && group.disk_left() + disk >= 0 &&
      group.flash_left() + flash >= 0) {
    group.set_host_quota(group.host_quota() + host);
    group.set_host_left(group.host_left() + host);
    group.set_cpu_quota(group.cpu_quota() + cpu);
    group.set_cpu_left(group.cpu_left() + cpu);
    group.set_mem_quota(group.mem_quota() + mem);
    group.set_mem_left(group.mem_left() + mem);
    group.set_disk_quota(group.disk_quota() + disk);
    group.set_disk_left(group.disk_left() + disk);
    group.set_flash_quota(group.flash_quota() + flash);
    group.set_flash_left(group.flash_left() + flash);
  } else {
    LOGS(WARNING) << "Quota can't be negative";
    return kQuotaInvalid;
  }

  std::string result_str;
  service.SerializeToString(&result_str);
  LOGS(INFO) << "resutl str: " << service.ShortDebugString();
  leveldb::Status ls = service_db_->Put(woption_, service_name, result_str);
  if (!ls.ok()) {
    LOGS(INFO) << ls.ToString();
    return kDBWriteError;
  }

  group.SerializeToString(&result_str);
  LOGS(INFO) << "resutl str: " << group.ShortDebugString();
  leveldb::Status status = group_db_->Put(woption_, group_name, result_str);
  if (!status.ok()) {
    LOGS(WARNING) << status.ToString();
    return kDBWriteError;
  }

  return kOK;
}

StatusCode DB::ListGroup(const std::string& group_name, Group* group, std::string* group_info) {
  LOGS(INFO) << "in db ListGroup\n";
  std::string group_str;
  leveldb::Status status = group_db_->Get(roption_, group_name, &group_str);
  if (status.IsNotFound()) {
    return kServiceNotFound;
  }
  Group g;
  g.ParseFromString(group_str);
  *group = g;
  *group_info = g.ShortDebugString();
  LOGS(INFO) << *group_info;
  return kOK;
}

StatusCode DB::AddApp(const std::string& group_name, const std::string& app_name,
                      const std::string& app_id, int64_t host, int64_t cpu, int64_t mem,
                      int64_t disk, int64_t flash) {
  Group group;
  StatusCode s = GetGroup(group_name, &group);
  if (s != kOK) {
    return s;
  }
  s = GetApp(&group, app_name, app_id);
  if (s == kAppExists) {
    return s;
  }
  if (group.host_left() >= host && group.cpu_left() >= cpu &&
      group.mem_left() >= mem && group.disk_left() >= disk &&
      group.flash_left() >= flash) {
    App* app = group.add_apps();
    app->set_name(app_name);
    app->set_id(app_id);
    app->set_host(host);
    app->set_cpu(cpu);
    app->set_mem(mem);
    app->set_disk(disk);
    app->set_flash(flash);
    group.set_host_left(group.host_left() - host);
    group.set_cpu_left(group.cpu_left() - cpu);
    group.set_mem_left(group.mem_left() - mem);
    group.set_disk_left(group.disk_left() - disk);
    group.set_flash_left(group.flash_left() - flash);
  } else {
    LOGS(WARNING) << "resource run out";
    return kResourceRunOut;
  }

  std::string result_str;
  group.SerializeToString(&result_str);
  LOGS(INFO) << "resutl str: " << group.ShortDebugString();
  leveldb::Status status = group_db_->Put(woption_, group_name, result_str);
  if (!status.ok()) {
    LOGS(INFO) << status.ToString();
    return kDBWriteError;
  }
  return kOK;
}

StatusCode DB::DelApp(const std::string& group_name, const std::string& app_name,
                      const std::string& app_id) {
  Group group;
  StatusCode s = GetGroup(group_name, &group);
  if (s != kOK) {
    return s;
  }
  s = GetApp(&group, app_name, app_id);
  if (s != kAppExists) {
    return s;
  }

  std::vector<App> new_apps;
  for(int i = 0; i < group.apps_size(); ++i) {
    App cur_app = group.apps(i);
    if (cur_app.name() == app_name && cur_app.id() == app_id) {
      group.set_host_left(group.host_left() + cur_app.host());
      group.set_cpu_left(group.cpu_left() + cur_app.cpu());
      group.set_mem_left(group.mem_left() + cur_app.mem());
      group.set_disk_left(group.disk_left() + cur_app.disk());
      group.set_flash_left(group.flash_left() + cur_app.flash());
    } else {
      App tmp;
      tmp.CopyFrom(cur_app);
      new_apps.push_back(tmp);
    }
  }
  group.clear_apps();
  for (int i = 0; i < new_apps.size(); ++i) {
    App cur = new_apps[i];
    App* app = group.add_apps();
    app->set_name(cur.name());
    app->set_id(cur.id());
    app->set_host(cur.host());
    app->set_cpu(cur.cpu());
    app->set_mem(cur.mem());
    app->set_disk(cur.disk());
    app->set_flash(cur.flash());
  }
  std::string result_str;
  group.SerializeToString(&result_str);
  LOGS(INFO) << "resutl str: " << group.ShortDebugString();
  leveldb::Status status = group_db_->Put(woption_, group_name, result_str);
  if (!status.ok()) {
    LOGS(INFO) << status.ToString();
    return kDBWriteError;
  }

  return kOK;
}

StatusCode DB::AddUser(const std::string& user_name, const std::string& passwd) {
  LOGS(INFO) << "in db AddUser\n";
  StatusCode us = GetUser(user_name, NULL);
  if (us != kUserNotFound) {
    return kUserExists;
  }

  User user;
  user.set_user_name(user_name);
  user.set_token(passwd);

  std::string res;
  user.SerializeToString(&res);
  LOGS(INFO) << "debug string: " << user.ShortDebugString();

  leveldb::Status status = user_db_->Put(woption_, user_name, res);
  if (!status.ok()) {
    LOGS(INFO) << status.ToString();
    return kDBWriteError;
  }
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

  s = GetUserInGroup(&group, user_name);
  if (s == kOK) {
    return kUserExists;
  } else if (s == kUserNotFound) {
    // ok
  } else {
    LOGS(WARNING) << StatusCode_Name(s);
    return s;
  }

  group.add_users(user_name);
  std::string result_str;
  group.SerializeToString(&result_str);
  LOGS(INFO) << "resutl str: " << group.ShortDebugString();
  leveldb::Status status = group_db_->Put(woption_, group_name, result_str);
  if (!status.ok()) {
    LOGS(INFO) << status.ToString();
    return kDBWriteError;
  }

  user.add_groups(group_name);
  user.SerializeToString(&result_str);
  LOGS(INFO) << "resutl str: " << user.ShortDebugString();
  status = user_db_->Put(woption_, user_name, result_str);
  if (!status.ok()) {
    LOGS(INFO) << status.ToString();
    return kDBWriteError;
  }
  return kOK;
}

StatusCode DB::ListUser(const std::string& user_name, User* user, std::string* user_info) {
  LOGS(INFO) << "in db ListUser:" << user_name;
  std::string user_str;
  leveldb::Status status = user_db_->Get(roption_, user_name, &user_str);
  if (status.IsNotFound()) {
    return kUserNotFound;
  }
  User u;
  u.ParseFromString(user_str);
  *user = u;
  *user_info += "user_name: " + u.user_name() + "\n";
  uint32_t group_num = u.groups_size();
  for (uint32_t i = 0; i < group_num; ++i) {
    user_info->append(u.groups(i));
    user_info->append(";");
  }
  LOGS(INFO) << *user_info;
  LOGS(INFO) << u.ShortDebugString();
  return kOK;
}

StatusCode DB::GetService(const std::string& service_name, Service* service) {
  LOGS(INFO) << "in db GetService:" << service_name;
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

StatusCode DB::GetResource(Service* service, const std::string& resource_name, Resource** resource) {
  LOGS(INFO) << "in db GetService:" << resource_name;
  uint32_t resource_num = service->resources_size();
  for (int i = 0; i < resource_num; ++i) {
    Resource* cur_resource = service->mutable_resources(i);
    if (cur_resource->resource_name() == resource_name) {
      *resource = cur_resource;
      LOGS(INFO) << "resource found: " << resource_name;
      return kOK;
    }
  }
  LOGS(INFO) << "resource not found: " << resource_name;
  return kResourceNotFound;
}

StatusCode DB::GetGroup(const std::string& group_name, Group* group) {
  LOGS(INFO) << "in db GetGroup:" << group_name;
  std::string group_str;
  leveldb::Status status = group_db_->Get(roption_, group_name, &group_str);
  if (status.IsNotFound()) {
    LOGS(WARNING) << "group not found";
    return kGroupNotFound;
  } else if (!status.ok()) {
    LOGS(WARNING) << "db read error";
    return kDBReadError;
  }
  group->ParseFromString(group_str);
  return kOK;
}

StatusCode DB::GetUser(const std::string& user_name, User* user) {
  LOGS(INFO) << "in db GetUser\n";
  std::string user_str;
  leveldb::Status status = user_db_->Get(roption_, user_name, &user_str);
  if (status.IsNotFound()) {
    return kUserNotFound;
  } else if (!status.ok()) {
    return kDBReadError;
  }
  if (user) {
    user->ParseFromString(user_str);
  }
  return kOK;
}

StatusCode DB::GetUserInGroup(const Group* group, const std::string& user_name) {
  LOGS(INFO) << "in db GetUserInGroup\n";
  uint32_t user_size = group->users_size();
  for (int i = 0; i < user_size; ++i) {
    if (group->users(i) == user_name) {
      return kOK;
    }
  }
  return kUserNotFound;
}

StatusCode DB::GetApp(const Group* group, const std::string& app_name, const std::string& app_id) {
  LOGS(INFO) << "in db GetUserInGroup\n";
  uint32_t app_size = group->apps_size();
  for (int i = 0; i < app_size; ++i) {
    if (group->apps(i).name() == app_name && group->apps(i).id() == app_id) {
      return kAppExists;
    }
  }
  return kAppNotFound;
}

} // namespace server
} // namespace heimdallr

using common::DEBUG;
using common::INFO;
using common::WARNING;
using common::FATAL;

} // namespace baidu
