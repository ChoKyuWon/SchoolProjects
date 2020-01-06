from django.shortcuts import render
from django.http import HttpResponse
from search.models import Book
from reservation.models import Reservationlist
from search.models import Book

def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")

def reserve(request, bid):
    userinfo = request.COOKIES.get('userinfo')
    if not userinfo:
        return HttpResponse("Login first!")
    try:
        newid = Reservationlist.objects.order_by('-res_id')[0].res_id + 1
    except:
        newid = 0
    ret = Reservationlist.objects.filter(book_id=bid)
    if not ret:
        data = Reservationlist(res_id=newid, user_id=userinfo, book_id=bid)
        data.save()
        books = Book.objects.filter(book_id=bid)
        for b in books:
            b.book_status = False
            b.save()
        return HttpResponse("no reservation list on "+bid+". reserve success.")
    p = -1
    for r in ret:
        if str(r.user_id) == userinfo:
            print("return!!")
            return HttpResponse('You already reserve this book.')
        if r.priority > p:
            p = r.priority
    p += 1
    data = Reservationlist(res_id=newid, user_id=userinfo, book_id=bid, priority=p)
    data.save()
    books = Book.objects.filter(book_id=bid)
    for b in books:
        b.book_status = False
        b.save()
        print('status update!')
    return HttpResponse('Reserve priority:'+str(p))
# Create your views here.
