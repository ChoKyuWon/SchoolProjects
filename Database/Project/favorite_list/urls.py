from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('mylist', views.mylist, name='mylist'),
    path('delete/<fav_id>', views.delete, name="dellist"),
    path('<bid>', views.addlist, name='addlist'),
]