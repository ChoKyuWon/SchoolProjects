from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('reservation', views.reservation, name='roomreservation'),
    path('<rid>', views.viewroom, name='viewroom'),
]