// Copyright (c) 2015, Baidu.com. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>
#include <unistd.h>

#include <boost/lexical_cast.hpp>

#include "src/proto/status_code.pb.h"
#include "src/sdk/heimdallr.h"
#include "src/sdk/heimdallr_impl.h"

namespace baidu {
namespace heimdallr {

int32_t AddServiceOp(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string service_name = argv[2];
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  StatusCode status;

  client->AddService(service_name, &status, NULL);
  if (status == kOK) {
    std::cout << "add service " << service_name << " ok" << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t SetServiceResourceOp(int argc, char* argv[]) {
  if (argc < 6) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string service_name = argv[2];
  std::string resource_name = argv[3];
  std::string provider_name = argv[4];
  int64_t quantity = boost::lexical_cast<uint64_t>(argv[5]);
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  std::string services;
  StatusCode status;

  client->SetServiceResource(service_name, resource_name, provider_name,
                             quantity, &status, NULL);
  if (status == kOK) {
    std::cout << "set service: " << service_name << "\n"
              << "resource: " << resource_name << "\n"
              << "provider: " << provider_name << "\n"
              << "quantity: " << quantity << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t ListServiceOp(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string service_name = argv[2];
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  std::string service_info;
  StatusCode status;

  client->ListService(service_name, &service_info, &status, NULL);
  if (status == kOK) {
    std::cout << service_info << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t AddGroupOp(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string group_name = argv[2];
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  StatusCode status;

  client->AddGroup(group_name, &status, NULL);
  if (status == kOK) {
    std::cout << "add group " << group_name << " ok" << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t SetGroupOp(int argc, char* argv[]) {
  if (argc < 6) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string group_name = argv[2];
  std::string service_name = argv[3];
  int64_t host = boost::lexical_cast<uint64_t>(argv[4]);
  int64_t cpu = boost::lexical_cast<uint64_t>(argv[5]);
  int64_t mem = boost::lexical_cast<uint64_t>(argv[6]);
  int64_t disk = boost::lexical_cast<uint64_t>(argv[7]);
  int64_t flash = boost::lexical_cast<uint64_t>(argv[8]);
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  std::string groups;
  StatusCode status;

  client->SetGroupQuota(group_name, service_name, host, cpu, mem, disk, flash, &status, NULL);
  if (status == kOK) {
    std::cout << "set group quota: " << group_name << "\n"
              << "service: " << service_name << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t ListGroupOp(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string group_name = argv[2];
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  std::string group_info;
  StatusCode status;

  client->ListGroup(group_name, &group_info, &status, NULL);
  if (status == kOK) {
    std::cout << group_info << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t AddAppOp(int argc, char* argv[]) {
  if (argc < 8) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  // TODO check quota
  std::string group_name = argv[2];
  std::string app_name = argv[3];
  std::string app_id = argv[4];
  uint64_t host = boost::lexical_cast<uint64_t>(argv[5]);
  uint64_t cpu = boost::lexical_cast<uint64_t>(argv[6]);
  uint64_t mem = boost::lexical_cast<uint64_t>(argv[7]);
  uint64_t disk = boost::lexical_cast<uint64_t>(argv[8]);
  uint64_t flash = boost::lexical_cast<uint64_t>(argv[9]);

  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  StatusCode status;

  client->AddApp(group_name, app_name, app_id, host, cpu, mem, disk, flash, &status, NULL);
  if (status == kOK) {
    std::cout << "add app " << app_name << " ok" << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t DelAppOp(int argc, char* argv[]) {
  if (argc < 5) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  // TODO check quota
  std::string group_name = argv[2];
  std::string app_name = argv[3];
  std::string app_id = argv[4];

  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  StatusCode status;

  client->DelApp(group_name, app_name, app_id, &status, NULL);
  if (status == kOK) {
    std::cout << "del app " << app_name << " ok" << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t AddUserOp(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string user_name = argv[2];
  std::string passwd = argv[3];
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  StatusCode status;

  client->AddUser(user_name, passwd, &status, NULL);
  if (status == kOK) {
    std::cout << "add user " << user_name << " ok" << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t ListUserOp(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string user_name = argv[2];
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  std::string user_info;
  StatusCode status;

  client->ListUser(user_name, &user_info, &status, NULL);
  if (status == kOK) {
    std::cout << user_info << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

int32_t SetUserOp(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string user_name = argv[2];
  std::string group_name = argv[3];
  sdk::Heimdallr* client = new sdk::HeimdallrImpl;
  StatusCode status;

  client->AddUserToGroup(user_name, group_name, &status, NULL);
  if (status == kOK) {
    std::cout << "add user " << user_name << " to "
              << group_name << " ok" << std::endl;
  } else {
    std::cerr << StatusCode_Name(status) << std::endl;
  }
  delete client;
  return 0;
}

} // namespace sdk
} // namespace heimdallr

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "missing arguments" << std::endl;
    return 1;
  }
  std::string op = argv[1];
  if (op == "add_serv") {
    return baidu::heimdallr::AddServiceOp(argc, argv);
  } else if (op == "set_serv") {
    return baidu::heimdallr::SetServiceResourceOp(argc, argv);
  } else if (op == "list_serv") {
    return baidu::heimdallr::ListServiceOp(argc, argv);
  } else if (op == "add_group") {
    return baidu::heimdallr::AddGroupOp(argc, argv);
  } else if (op == "set_group") {
    return baidu::heimdallr::SetGroupOp(argc, argv);
  } else if (op == "list_group") {
    return baidu::heimdallr::ListGroupOp(argc, argv);
  } else if (op == "add_app") {
    return baidu::heimdallr::AddAppOp(argc, argv);
  } else if (op == "del_app") {
    return baidu::heimdallr::DelAppOp(argc, argv);
  } else if (op == "add_user") {
    return baidu::heimdallr::AddUserOp(argc, argv);
  } else if (op == "set_user") {
    return baidu::heimdallr::SetUserOp(argc, argv);
  } else if (op == "list_user") {
    return baidu::heimdallr::ListUserOp(argc, argv);
  }

  return 0;
}
