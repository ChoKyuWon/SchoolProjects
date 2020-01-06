from django.db import models
from login.models import User

class Studyroom(models.Model):
    room_id = models.IntegerField(default=0)

class Reservationroom(models.Model):
    res_id = models.IntegerField(default=0, primary_key=True)
    room_id = models.IntegerField(default=0)
    user_id = models.IntegerField(default=0)
    start_time = models.IntegerField(default=0)
    end_time = models.IntegerField(default=0)
# Create your models here.
