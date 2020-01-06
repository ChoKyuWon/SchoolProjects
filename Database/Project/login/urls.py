from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('login/<userID>/<userPW>', views.login, name="login"),
    path('logout', views.logout, name='logout'),
    path('register', views.register_index, name='register_index'),
    path('register/new', views.register,name='register')
]