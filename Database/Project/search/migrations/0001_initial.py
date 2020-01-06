# Generated by Django 2.2.7 on 2019-11-27 06:50

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Book',
            fields=[
                ('book_id', models.IntegerField(default=0, primary_key=True, serialize=False)),
                ('book_name', models.CharField(max_length=200)),
                ('author_name', models.CharField(max_length=20)),
                ('book_status', models.BooleanField(default=True)),
                ('book_location', models.CharField(max_length=20)),
                ('ISBN', models.IntegerField(default=0)),
            ],
        ),
    ]
