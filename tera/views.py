from django.shortcuts import render
from django.http import HttpResponse

# Create your views here.

def index(request):
    return render(request, 'home.html')


def arch(request):
    return render(request, 'arch.html')


def dash(request):
    return render(request, 'dash.html')


def signin(request):
    return render(request, 'signin.html')


def contact(request):
    return render(request, 'contact.html')


def team(request):
    return render(request, 'team.html')
