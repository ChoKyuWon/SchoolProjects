from django.db import models
from login.models import User
from search.models import Book

class Favoritelist(models.Model):
    fav_id = models.IntegerField(default=0, primary_key=True)
    user_id = models.IntegerField()
    book_id = models.IntegerField()
# Create your models here.
