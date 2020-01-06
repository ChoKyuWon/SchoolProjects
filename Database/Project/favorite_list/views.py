from django.shortcuts import render
from django.http import HttpResponse
from search.models import Book
from favorite_list.models import Favoritelist
from search.models import Book

def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")

def addlist(request, bid):
    userinfo = request.COOKIES.get('userinfo')
    if not userinfo:
        return HttpResponse("Login first!")
    ret = Favoritelist.objects.filter(book_id=bid)
    for r in ret:
        if str(r.user_id) == userinfo:
            return HttpResponse('You already add this book on favorite list.')
    try:
        newid = Favoritelist.objects.order_by('-fav_id')[0].res_id + 1
    except:
        newid = 0
    data = Favoritelist(fav_id=newid, user_id=userinfo, book_id=bid)
    data.save()
    return HttpResponse('Add book on favorite list!')

def mylist(request):
    s = ''
    userinfo = request.COOKIES.get('userinfo')
    if not userinfo:
        return HttpResponse("No list...")
    ret = Favoritelist.objects.filter(user_id=userinfo)
    for r in ret:
        bid = r.book_id
        books = Book.objects.filter(book_id=bid)
        for book in books:
            s += (str(r.fav_id) + ":" + book.book_name + '<br>')
    return HttpResponse(s)

def delete(request, fav_id):
    ret = Favoritelist.objects.get(fav_id=fav_id)
    ret.delete()
    return HttpResponse('Del COm.')
# Create your views here.
