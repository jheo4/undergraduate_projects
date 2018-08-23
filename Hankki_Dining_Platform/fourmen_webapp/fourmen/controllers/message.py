from flask import Blueprint, request, render_template, session
from fourmen.models import db_session, Message
from sqlalchemy import func, and_, or_

from . import login_required

mod = Blueprint('message', __name__, url_prefix='/messages')


@mod.route('/', methods=['GET', 'POST'])
@login_required
def message_list():
    if request.method == 'GET':
        rcv_msg = db_session.query(Message.sender_id, func.max(Message.id)). \
            filter_by(receiver_id=session['id']).group_by(Message.sender_id)
        send_msg = db_session \
            .query(Message.receiver_id, func.max(Message.id)). \
            filter_by(sender_id=session['id']).group_by(Message.receiver_id)
        message = rcv_msg.union(send_msg).all()
        msg = []
        messages_id = []
        for m in message:
            messages_id.append(m[1])
        msg.append(Message.query.filter(Message.id.in_(messages_id)).all())
        recent_msg = {}
        for m in msg[0]:
            counter_id = 0
            if m.sender_id == session['id']:
                counter_id = m.receiver_id
            elif m.sender_id != session['id']:
                counter_id = m.sender_id
            if counter_id not in recent_msg:
                recent_msg[counter_id] = m.id
            elif counter_id in recent_msg and counter_id < m.id:
                recent_msg[counter_id] = m.id
        recent_msgs = Message.query \
            .filter(Message.id.in_(recent_msg.values())). \
            order_by(Message.id.desc()).all()
        return render_template('message_box.html', message=recent_msgs)
    elif request.method == 'POST':
        if session['id'] == int(request.form['receiver_id']):
            return 'no'
        else:
            msg = Message(sender_id=session['id'],
                          receiver_id=request.form['receiver_id'],
                          meal_id=request.form['meal_id'],
                          content=request.form['content'])
            db_session.add(msg)
            db_session.flush()
        return 'ok'


@mod.route('/<int:meal_id>/chat/<int:counter_id>', methods=['GET', 'POST'])
@login_required
def message_room(meal_id, counter_id):
    if request.method == 'GET':
        msg = Message.query.filter_by(meal_id=meal_id) \
            .filter(or_(
                and_(Message.receiver_id == session['id'],
                     Message.sender_id == counter_id),
                and_(Message.receiver_id == counter_id,
                     Message.sender_id == session['id'])
            )).order_by(Message.id.desc()).all()
        return render_template('message_room.html', message=msg,
                               counter_id=counter_id)
