# Generated by Django 2.2.7 on 2019-11-27 06:50

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Favoritelist',
            fields=[
                ('fav_id', models.IntegerField(default=True, primary_key=True, serialize=False)),
                ('user_id', models.IntegerField()),
                ('book_id', models.IntegerField()),
            ],
        ),
    ]
