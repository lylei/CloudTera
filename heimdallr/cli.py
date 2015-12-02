# Copyright (c) 2015, Baidu.com. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# encoding: utf-8

import argparse
import subprocess


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
  list_serv_parser.set_defaults(func=list_serv)

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
  list_group_parser.set_defaults(func=list_group)

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
  list_user_parser.set_defaults(func=list_user)

  args = parser.parse_args()
  args.func(args)


def add_serv(args):
  cmd = ' '.join([CLI, 'add_serv', args.name])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_serv(args)


def set_serv(args):
  cmd = ' '.join([CLI, 'set_serv', args.name, args.res, args.prov, args.quan])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_serv(args)


def list_serv(args):
  resources = 'resources'
  providers = 'providers'
  groups = 'group_name'
  history = 'history'
  cmd = ' '.join([CLI, 'list_serv', args.name])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  res_str = ''.join(ret.stdout.readlines())
  name_ends = res_str.find(args.name) + len(args.name) + 1
  print 'Service', args.name
  print '------------------------------'

  res_str = res_str[name_ends:]
  res_str = res_str.replace(resources, resources+'\n')
  res_str = res_str.replace(providers, providers+'\n')
  res_str = res_str.replace(history, '\n  '+history+'\n  ')
  res_str = res_str.replace(groups, '\n  used')
  res_str = res_str.replace('left', ' left')
  res_str = res_str.replace('used_by', '')
  res_str = res_str.replace('}', '\n')
  res_str = res_str.replace('{', '')

  comp = res_str.split('\n')
  new_res_str = []
  l = []
  r = []
  for line in comp:
    if 'resource_name' in line and 'capacity' in line:
      l.append(r)
      r = []
      tmp = line.split(' ')
      tmp = filter(None, tmp)
      r.append(tmp[1][1:-1])
      new_res_str.append(line)
    elif 'capacity' in line:
      tmp = line.split(' ')
      tmp = filter(None, tmp)
      r.append(tmp[1])
    elif 'history' in line or 'providers' in line:
      pass
    else:
      new_res_str.append(line)
  l = filter(None, l)
  for resource in l:
    print resource[0]
    for i in range(1, len(resource)):
      if resource[0] == 'cpu':
        n = int(resource[i])
        print '%6d' % n, '.' * (n / 100)
      if resource[0] == 'host':
        n = int(resource[i])
        print '%6d' % n, '.' * (n / 10)
  print ''
  print '\n'.join(new_res_str)


def add_group(args):
  cmd = ' '.join([CLI, 'add_group', args.name])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_group(args)


def set_group(args):
  cmd = ' '.join([CLI, 'set_group', args.name, args.serv, args.res, args.quota])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_group(args)


def list_group(args):
  cmd = ' '.join([CLI, 'list_group', args.name])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)

  res_str = ''.join(ret.stdout.readlines())
  name_ends = res_str.find(args.name) + len(args.name) + 1
  res_str = res_str.replace('}', '\n')
  res_str = res_str.replace('{', '')
  print 'Group', args.name
  print '------------------------------'

  res_str = res_str[name_ends:]
  print res_str

  if args.name == 'tera':
    try:
      fp = open('tera.tail', 'r')
      fp.seek(-150, 2)
      lines = ''
      for i in range(10):
        lines += fp.readline()
      comp = lines.split('\n')
      comp = comp[1:-1]
      for line in comp:
        l = line.split(' ')
        mem = l[1]
        cpu = l[2]
        mem = int(float(mem))
        cpu = int(float(cpu))
        print 'mem:%6dM' % (mem /1024/1024),
        mem_str = '.' * (mem / 1024 / 1024 / 5)
        print mem_str
        print 'cpu:%6d%%' % cpu,
        cpu_str = '.' * (cpu)
        print cpu_str
    except:
      pass



def add_user(args):
  cmd = ' '.join([CLI, 'add_user', args.name, args.passwd])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_user(args)


def set_user(args):
  cmd = ' '.join([CLI, 'set_user', args.name, args.group])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  list_user(args)


def list_user(args):
  cmd = ' '.join([CLI, 'list_user', args.name])
  ret = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                         shell=True)
  print ''.join(ret.stdout.readlines())


def main():
  parse_args()


if __name__ == '__main__':
  main()