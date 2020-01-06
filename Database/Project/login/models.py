from django.db import models

class User(models.Model):
    uid = models.IntegerField(default=0, primary_key=True)
    user_id = models.IntegerField(default=0)
    user_passwordhash = models.CharField(max_length=100)
    user_isadmin = models.BinaryField(default=b'0')
    user_classroom_id = models.IntegerField(default=0)
    def __str__(self):
        return str(self.user_id)

# Create your models here.
