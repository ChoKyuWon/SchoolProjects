from django.shortcuts import render
from django.http import HttpResponse
from search.models import Book

def index(request):
    isadmin = request.COOKIES.get('isadmin')
    if isadmin == 'True':
        return render(request, "addbooks.html")
    return HttpResponse("Login as admin first!")

def addbook(request):
    isadmin = request.COOKIES.get('isadmin')
    if not (isadmin == 'True'):
        return HttpResponse("invalid access!")
    form = request.POST
    bid = -1
    for book in Book.objects.all():
        if book.book_id > bid:
            bid = book.book_id
    bid += 1
    try:
        _book_name = form['bookname']
        _author = form['author']
        _location = form['location']
        _isbn = int(form['isbn'])
    except:
        return HttpResponse("invalid data!")
    data = Book(book_id=bid, book_name=_book_name, author_name=_author, book_location=_location, ISBN=_isbn)
    data.save()
    return HttpResponse('Add book!')

# Create your views here.
