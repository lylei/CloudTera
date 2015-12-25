from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django import forms
import json
import eva_process as eva
import cli

# Create your views here.

def index(request):
    user = get_user(request)
    if 'act' in request.GET and 'email' in request.session:
        request.session.pop('email')
        return render(request, 'home.html', {'user': ['']})
    return render(request, 'home.html', {'user': [user]})


def arch(request):
    user = get_user(request)
    return render(request, 'arch.html', {'user': [user]})

@csrf_exempt
def bench(request):
    if request.method == 'POST':
        return HttpResponse(eva.write_eva_data(request))
    elif request.method == 'GET':
        user = get_user(request)
        rw_disk, r_disk, rw_flash, r_flash = eva.read_eva_data(request)
        data = {}
        data['name'] = ['cpu(%)', 'memory(G)', 'write(M/s)', 'read(qps)']
        if 'cluster' in request.GET:
            data['stat'] = eva.read_trace(encode_utf8(request.GET['cluster']))
        #return HttpResponse(str(r_flash))
        return render(request, 'bench.html', {'rw_disk': rw_disk, 'r_disk': r_disk,
                                              'rw_flash': rw_flash, 'r_flash': r_flash,
                                              'data': data,
                                              'user': [user]})


def api(request):
    user = get_user(request)
    return render(request, 'api.html', {'user': [user]})


def faq(request):
    user = get_user(request)
    return render(request, 'faq.html', {'user': [user]})


def dash(request):
    galaxy_info = cli.list_serv('galaxy')
    groups = galaxy_info['groups']
    # need to encode data
    # s.encode('utf-8')
    resources = galaxy_info['resources']
    names = []
    caps_dict = {}
    caps = []
    used = []
    for i in range(len(resources)):
        resource = resources[i]
        name = resource['resource_name'].encode('utf-8')
        names.append(name)
        caps_dict[name] = resource['capacity']
        caps.append(resource['capacity'])
        used.append(resource['capacity'] - resource['left'])

    provides = galaxy_info['providers']
    provid_dict = {}
    for n in names:
        provid_dict.update({n: []})
    for p in provides:
        provid_dict[p['resource_name']].append({'name': p['name'].encode('utf-8'),
                                                'quantity': p['quantity'],
                                                'cap': caps_dict[p['resource_name']]})

    # for user sign in
    user = get_user(request)

    return render(request, 'dash/dash.html', {'names': names, 'caps': caps, 'used': used,
                                         'provide_names': provid_dict.keys(), 'provides': provid_dict.values(),
                                         'user': [user]})

def overview(request):
    user, group_list, quota_dict, apps = parse_group_quota(request)
    return render(request, 'dash/overview.html', {'user': [user], 'groups':group_list, 'quota': quota_dict})

def tera(request):
    user, group_list, quota_dict, apps = parse_group_quota(request)
    data = {}
    data['name'] = ['cpu(%)', 'memory(G)', 'write(M/s)', 'read(qps)']
    if 'cluster' in request.GET:
        data['stat'] = eva.read_trace(encode_utf8(request.GET['cluster']))
    return render(request, 'dash/tera.html', {'user': [user], 'groups':group_list, 'quota': quota_dict, 'apps': apps, 'data': data})


def nexus(request):
    user, group_list, quota_dict, apps = parse_group_quota(request)
    return render(request, 'dash/nexus.html', {'user': [user], 'groups':group_list, 'quota': quota_dict, 'apps': apps})


def container(request):
    user, group_list, quota_dict, apps = parse_group_quota(request)
    return render(request, 'dash/container.html', {'user': [user]})

@csrf_exempt
def signin(request):
    if request.method == "POST":
        e = request.POST['email']
        request.session['email'] = e
        return render(request, 'home.html', {'user': [e.encode('utf-8')]})
    elif request.method == "GET":
        user = get_user(request)
        if user == '':
            return render(request, 'signin.html')
        else:
            return dash(request)


def git(request):
    user = get_user(request)
    return render(request, 'git.html', {'user': [user]})


def open(request):
    user = get_user(request)
    return render(request, 'open.html', {'user': [user]})


def contact(request):
    user = get_user(request)
    return render(request, 'contact.html', {'user': [user]})


def team(request):
    user = get_user(request)
    return render(request, 'team.html', {'user': [user]})


def join(request):
    user = get_user(request)
    return render(request, 'join.html', {'user': [user]})


def test(request):
    if request.method == 'GET':
        user = get_user(request)
        rw_disk, r_disk, rw_flash, r_flash = eva.read_eva_data(request)
        return HttpResponse(str(r_flash))


        return HttpResponse(str(used))
    elif request.method == 'POST':
        return HttpResponse('post')
    return HttpResponse("else!")


def simple(request):
    if request.method == 'GET':
        user = get_user(request)
        return render(request, 'simple.html', {'email': request.session['email'], 'user': [user]})
    else:
        return HttpResponse('none')


def get_user(request):
    user = ''
    if 'email' in request.session:
        user = request.session['email'].encode('utf-8')
    return user


def encode_utf8(input):
    if type(input) == unicode:
        return input.encode('utf-8')
    else:
        return input


def parse_group_quota(request):
    user = get_user(request)
    groups = cli.list_user('lei')
    group_list = []
    quota_dict = {}
    app_list = []
    for g in groups:
        group_list.append(g.encode('utf-8'))

    if 'group' in request.GET:
        group = cli.list_group(request.GET['group'])
        apps = group['apps']
        quota = group['quota']
        for app in apps:
            temp = {}
            for k, v in app.iteritems():
                temp[encode_utf8(k)] = encode_utf8(v)
            app_list.append(temp)

        for q in quota:
            quota_dict[encode_utf8(q['resource_name'])] = encode_utf8(q['quota'])
    return user, group_list, quota_dict, app_list
