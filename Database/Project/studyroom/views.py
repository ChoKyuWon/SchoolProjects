from django.shortcuts import render
from django.http import HttpResponse
from studyroom.models import Reservationroom
from studyroom.models import Studyroom
def index(request):
    context = {}
    l = []
    rooms = Studyroom.objects.all()
    for room in rooms:
        l.append(room.room_id)
    context['list'] = l
    
    return render(request, "room_index.html", context)

def viewroom(request, rid):
    start_time_list = []
    end_time_list = []
    timedic = {}

    rooms = Reservationroom.objects.filter(room_id=rid).order_by('start_time')
    for room in rooms:
        start_time_list.append(room.start_time)
        end_time_list.append(room.end_time)
    timedic['list'] = zip(start_time_list,end_time_list)
    res = render(request, 'viewroom.html', timedic)
    res.set_cookie('room_id', value=rid)
    return res

def reservation(request):
    rid = request.COOKIES.get('room_id')
    userinfo = request.COOKIES.get('userinfo')
    if not userinfo:
        res = HttpResponse("Login First!")
        res.delete_cookie('room_id')
        return res
    try:
        start = int(request.POST['start'])
        end = int(request.POST['end'])
    except:
        res = HttpResponse('Not empty field allowed!')
        res.delete_cookie('room_id')
        return res
    if start < 0 or start > 24:
        res = HttpResponse('only 0~24 value allowed!')
        res.delete_cookie('room_id')
        return res
    if end < 0 or end > 24:
        res = HttpResponse('only 0~24 value allowed!')
        res.delete_cookie('room_id')
        return res
    r = Reservationroom.objects.order_by('-res_id')[0].res_id + 1
    rooms = Reservationroom.objects.filter(room_id=rid)
    time_range = range(start, end)
    set_time_range = set(time_range)
    for room in rooms:
        check_time = range(room.start_time, room.end_time)
        if set_time_range.intersection(check_time):
            res = HttpResponse('Can not reservate this time. Try another time.')
            res.delete_cookie('room_id')
            return res
    data = Reservationroom(res_id=r, room_id=rid, user_id=userinfo, start_time=start, end_time=end)
    data.save()
    res = HttpResponse('Reservation Success!')
    res.delete_cookie('room_id')
    return res
    
# Create your views here.
