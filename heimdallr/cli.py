# Copyright (c) 2015, Baidu.com. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# encoding: utf-8

import argparse
import subprocess
import json


CLI = './cli'


def parse_args():
  parser = argparse.ArgumentParser()
  subparser = parser.add_subparsers()

  add_serv_parser = subparser.add_parser('add_serv')
  add_serv_parser.add_argument('--name', type=str)
  add_serv_parser.set_defaults(func=add_serv)

  set_serv_parser = subparser.add_parser('set_serv')
  set_serv_parser.add_argument('--name', type=str)
  set_serv_parser.add_argument('--res', type=str)
  set_serv_parser.add_argument('--prov', type=str)
  set_serv_parser.add_argument('--quan', type=str)
  set_serv_parser.set_defaults(func=set_serv)

  list_serv_parser = subparser.add_parser('list_serv')
  list_serv_parser.add_argument('--name', type=str)
  list_serv_parser.set_defaults(func=list_serv_warp)

  add_group_parser = subparser.add_parser('add_group')
  add_group_parser.add_argument('--name', type=str)
  add_group_parser.set_defaults(func=add_group)

  set_group_parser = subparser.add_parser('set_group')
  set_group_parser.add_argument('--name', type=str)
  set_group_parser.add_argument('--serv', type=str)
  set_group_parser.add_argument('--res', type=str)
  set_group_parser.add_argument('--quota', type=str)
  set_group_parser.set_defaults(func=set_group)

  list_group_parser = subparser.add_parser('list_group')
  list_group_parser.add_argument('--name', type=str)
  list_group_parser.set_defaults(func=list_group_warp)

  add_app_parser = subparser.add_parser('add_app')
  add_app_parser.add_argument('--group_name', type=str)
  add_app_parser.add_argument('--app_name', type=str)
  add_app_parser.add_argument('--cpu', type=int, default=0)
  add_app_parser.add_argument('--mem',  type=int, default=0)
  add_app_parser.add_argument('--disk', type=int, default=0)
  add_app_parser.add_argument('--flash', type=int, default=0)
  add_app_parser.set_defaults(func=add_app_wrap)

  add_user_parser = subparser.add_parser('add_user')
  add_user_parser.add_argument('--name', type=str)
  add_user_parser.add_argument('--passwd', type=str)
  add_user_parser.set_defaults(func=add_user)

  set_user_parser = subparser.add_parser('set_user')
  set_user_parser.add_argument('--name', type=str)
  set_user_parser.add_argument('--group', type=str)
  set_user_parser.set_defaults(func=set_user)

  list_user_parser = subparser.add_parser('list_user')
  list_user_parser.add_argument('--name', type=str)
  list_user_parser.set_defaults(func=list_user_wrap)

  args = parser.parse_args()
  args.func(args)


def add_serv(args):
  cmd = ' '.join([CLI, 'add_serv', args.name])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_serv_warp(args)


def set_serv(args):
  cmd = ' '.join([CLI, 'set_serv', args.name, args.res, args.prov, args.quan])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_serv_warp(args)


def list_serv_warp(args):
  list_serv(args.name)


def list_serv(name):
  cmd = '''curl -d '{"service_name":"''' + name + '''"}' 0.0.0.0:8223/baidu.heimdallr.server.Server.ListService'''
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  ret = ''.join(ret.stdout.readlines())
  d = json.loads(ret)
  print d['services']
  return d['services']


def add_group(args):
  cmd = ' '.join([CLI, 'add_group', args.name])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_group_warp(args)


def set_group(args):
  cmd = ' '.join([CLI, 'set_group', args.name, args.serv, args.res, args.quota])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_group_warp(args)


def list_group_warp(args):
  list_group(args.name)

def list_group(name):
  cmd = '''curl -d '{"group_name":"''' + name + '''"}' 0.0.0.0:8223/baidu.heimdallr.server.Server.ListGroup'''
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  ret = ''.join(ret.stdout.readlines())
  d = json.loads(ret)
  print d['group']
  return d['group']


def add_app_wrap(args):
  add_app(args.group_name, args.app_name, args.cpu, args.mem, args.disk, args.flash)


def add_app(group, app, cpu, mem, disk, flash):
  cmd = ' '.join([CLI, 'add_app', str(group), str(app), str(cpu), str(mem), str(disk), str(flash)])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_group(group)


def add_user(args):
  cmd = ' '.join([CLI, 'add_user', args.name, args.passwd])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_user(args)


def set_user(args):
  cmd = ' '.join([CLI, 'set_user', args.name, args.group])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_user(args.name)


def list_user_wrap(args):
  list_user(args.name)


def list_user(name):
  cmd = '''curl -d '{"user_name":"''' + name + '''"}' 0.0.0.0:8223/baidu.heimdallr.server.Server.ListUser'''
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  ret = ''.join(ret.stdout.readlines())
  d = json.loads(ret)
  print d['user']['groups']
  return d['user']['groups']


def main():
  parse_args()


if __name__ == '__main__':
  main()