from django.db import models
from login.models import User
from search.models import Book

class Reservationlist(models.Model):
    res_id = models.IntegerField(default=0, primary_key=True)
    user_id = models.IntegerField()
    book_id = models.IntegerField()
    priority = models.IntegerField(default=0)
    
# Create your models here.