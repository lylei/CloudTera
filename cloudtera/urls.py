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

    # Docs
    url(r'^arch/$', 'tera.views.arch', name='arch'),
    url(r'^bench/$', 'tera.views.bench', name='bench'),
    url(r'^api/$', 'tera.views.api', name='api'),
    url(r'^faq/$', 'tera.views.faq', name='faq'),

    # Dashboard
    url(r'^dash/$', 'tera.views.dash', name='dash'),
    url(r'^signin/$', 'tera.views.signin', name='signin'),
    url(r'^container/$', 'tera.views.container', name='container'),
    url(r'^tera/$', 'tera.views.tera', name='tera'),
    url(r'^nexus/$', 'tera.views.nexus', name='nexus'),
    url(r'^overview/$', 'tera.views.overview', name='overview'),

    # Community
    url(r'^git/$', 'tera.views.git', name='git'),
    url(r'^open/$', 'tera.views.open', name='ope'),

    # About
    url(r'^team/$', 'tera.views.team', name='team'),
    url(r'^join/$', 'tera.views.join', name='join'),

    # Contact
    url(r'^contact/$', 'tera.views.contact', name='contact'),

    url(r'^test/$', 'tera.views.test', name='test'),
    url(r'^simple/$', 'tera.views.simple', name='simple'),

    url( r'^static/(?P<path>.*)$', 'django.views.statc.serve',{ 'document_root': settings.STATIC_ROOT }),
]
