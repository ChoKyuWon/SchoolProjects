from django.shortcuts import render
from django.http import HttpResponse, HttpResponseNotFound
from search.models import Book

def index(request):
    return render(request, 'search.html')

def search(request, category, search):
    res = ""
    names = []
    authors = []
    locations = []
    isbns =[]
    statuses =[]
    bids = []
    if category == "total":
        ret = Book.objects.filter(book_name__contains=search) | Book.objects.filter(author_name__contains=search)
        if search.isdigit() == True:
            ret = ret | Book.objects.filter(ISBN=search)

        for r in ret:
            names.append(r.book_name)
            authors.append(r.author_name)
            locations.append(r.book_location)
            isbns.append(str(r.ISBN))
            if r.book_status == True:
                statuses.append('Available')
            else:
                statuses.append('Unavailable')
            bids.append(str(r.book_id))

        dic = {}
        dic['name'] = names
        dic['author'] = authors
        dic['location'] = locations
        dic['isbn'] = isbns
        dic['status'] = statuses
        dic['bid'] = bids
        #res += r.book_name + '    |' + r.author_name + '    |' + r.book_location + '    |' + str(r.ISBN) + '    |Available<br>'
        return render(request, 'search_result.html', dic)

    elif category == "name":
        ret = Book.objects.filter(book_name__contains=search)
    elif category == "author_name":
        ret = Book.objects.filter(author_name__contains=search)
    elif category == "ISBN":
        ret = Book.objects.filter(ISBN=search)
    else:
        return HttpResponseNotFound(category)
    
    for r in ret:
        names.append(r.book_name)
        authors.append(r.author_name)
        locations.append(r.book_location)
        isbns.append(str(r.ISBN))
        if r.book_status == True:
            statuses.append('Available')
        else:
            statuses.append('Unavailable')
        bids.append(str(r.book_id))

    dic = {}
    dic['name'] = names
    dic['author'] = authors
    dic['location'] = locations
    dic['isbn'] = isbns
    dic['status'] = statuses
    dic['bid'] = bids
    return render(request, 'search_result.html', dic)
# Create your views here.
