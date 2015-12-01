import settings

"""cloudtera URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.8/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Add an import:  from blog import urls as blog_urls
    2. Add a URL to urlpatterns:  url(r'^blog/', include(blog_urls))
"""
from django.conf.urls import include, url
from django.contrib import admin

urlpatterns = [
    url(r'^admin/', include(admin.site.urls)),
    url(r'^$', 'tera.views.index', name='home'),
    url(r'^arch/$', 'tera.views.arch', name='arch'),
    url(r'^dash/$', 'tera.views.dash', name='dash'),
    url(r'^signin/$', 'tera.views.signin', name='signin'),
    url(r'^contact/$', 'tera.views.contact', name='contact'),
    url(r'^team/$', 'tera.views.team', name='team'),

    url( r'^static/(?P<path>.*)$', 'django.views.statc.serve',{ 'document_root': settings.STATIC_ROOT }),
]
