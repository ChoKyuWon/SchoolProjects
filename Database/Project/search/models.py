from django.db import models

class Book(models.Model):
    book_id = models.IntegerField(default=0, primary_key=True)
    book_name = models.CharField(max_length=200)
    author_name = models.CharField(max_length=20)
    book_status = models.BooleanField(default=True)
    book_location = models.CharField(max_length=20)
    ISBN = models.IntegerField(default=0)
    def __str__(self):
        return self.book_name

# Create your models here.
