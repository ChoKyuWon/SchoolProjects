# Generated by Django 2.2.7 on 2019-11-27 06:50

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        ('login', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Studyroom',
            fields=[
                ('room_id', models.IntegerField(default=0, primary_key=True, serialize=False)),
                ('room_number', models.CharField(max_length=20)),
                ('room_status', models.BinaryField()),
            ],
        ),
        migrations.CreateModel(
            name='Reservationroom',
            fields=[
                ('res_id', models.IntegerField(default=0, primary_key=True, serialize=False)),
                ('room_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='studyroom.Studyroom')),
                ('user_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='login.User')),
            ],
        ),
    ]