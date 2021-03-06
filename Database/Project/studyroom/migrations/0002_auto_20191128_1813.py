# Generated by Django 2.2.7 on 2019-11-28 09:13

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('studyroom', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='reservationroom',
            name='end_time',
            field=models.IntegerField(default=0),
        ),
        migrations.AddField(
            model_name='reservationroom',
            name='start_time',
            field=models.IntegerField(default=0),
        ),
        migrations.AlterField(
            model_name='reservationroom',
            name='room_id',
            field=models.IntegerField(default=0),
        ),
        migrations.AlterField(
            model_name='reservationroom',
            name='user_id',
            field=models.IntegerField(default=0),
        ),
        migrations.DeleteModel(
            name='Studyroom',
        ),
    ]
