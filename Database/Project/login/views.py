from django.shortcuts import render
from django.http import HttpResponse
from login.models import User

def index(request):
    return render(request, 'login.html')

def login(request, userID, userPW):
    ret = User.objects.filter(user_id=userID, user_passwordhash=userPW)
    if not ret:
        return HttpResponse('You are not User! or wrong password!')
    else:
        response = HttpResponse('Welcome, '+str(ret[0].user_id))
        response.set_cookie('userinfo', value=ret[0].uid)
        if ret[0].user_isadmin:
            response.set_cookie('isadmin', value='True')
        return response


def register(request):
    isadmin = False
    uid = -1
    if request.POST['userPW1'] == '':
        return HttpResponse('Not allowed null password')
    if request.POST['userPW1'] != request.POST['userPW2']:
        return HttpResponse('Not match password!')
    userID = request.POST['userID']
    if User.objects.filter(user_id=userID):
        return HttpResponse('already exist id!')
    userPW = request.POST['userPW1']
    try:
        if request.POST['admin']:
            isadmin = True
    except:
        pass
    qset = User.objects.all()
    for q in qset:
        if q.uid > uid:
            uid = q.uid
    uid += 1
    if isadmin:
        data = User(uid=uid, user_id=userID, user_passwordhash=userPW,user_isadmin=b'1')
    else:
        data = User(uid=uid, user_id=userID, user_passwordhash=userPW)
    data.save()
    return HttpResponse('Register Success!')

def register_index(request):
    return render(request, 'register.html')

def logout(request):
    response = HttpResponse('logout!')
    response.delete_cookie('userinfo')
    response.delete_cookie('isadmin')
    return response

# Create your views here.