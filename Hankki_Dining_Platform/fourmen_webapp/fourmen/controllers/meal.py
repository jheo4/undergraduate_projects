from werkzeug.exceptions import BadRequest
from flask import Blueprint, request, session, \
    url_for, render_template, redirect, abort, flash
from elasticsearch import Elasticsearch
from iamport import Iamport
from fourmen import app
from fourmen.models import db_session, Meal, Reservation, Review, MealPhoto
from fourmen.controllers import login_required, check_ownership
from datetime import datetime

es = Elasticsearch(
    ['dining-es']
)
mod = Blueprint('meal', __name__, url_prefix='/meals')


@mod.route('/search', methods=['GET'])
def keyword_search():
    if request.method == 'GET':
        keyword = request.args['keyword']
        distance = request.args['distance']
        lat = request.args['lat']
        lon = request.args['lng']
        query = {"query": {
            "bool": {
                "must": [
                    {
                        "multi_match": {
                            "query": keyword,
                            "fields": ["name", "description"]
                        }
                    }
                ],
                "filter": {
                    "geo_distance": {
                        "distance": distance,
                        "pin.location": {"lat": lat, "lon": lon}
                    }
                }
            }
        }
        }
        res = es.search(
            index="dining", doc_type="meal",
            body=query, filter_path=['hits.hits._source']
        )
        meals = []
        if 'hits' in res:
            for val in res['hits']['hits']:
                meal = Meal.query.get(val['_source']['id'])
                meals.append(meal)
        return render_template("meal_cards.html", meals=meals)


@mod.route('/', methods=['GET', 'POST'])
def list_meal():
    if request.method == 'GET':
        lat = request.args['lat']
        lon = request.args['lng']

        query = {"query": {
            "bool": {
                "must": {"match_all": {}},
                "filter": {
                    "geo_distance": {
                        "distance": "20km",
                        "pin.location": {
                            "lat": lat,
                            "lon": lon
                        }
                    }
                }
            }
        }}
        res = es.search(
            index="dining", doc_type="meal",
            body=query, filter_path=['hits.hits._id']
        )
        if (res):
            meals_id = [dic['_id'] for dic in res['hits']['hits']]
            meals = Meal.query.filter(Meal.id.in_(meals_id)).all()
        else:
            meals = []
        return render_template('meal_main.html', meals=meals, lat=lat, lon=lon)
    elif request.method == 'POST':
        meal = Meal(
            name=request.form['title'],
            host_id=session['id'],
            price=request.form['price'],
            description=request.form['description'],
            datetime=datetime.strptime(
                request.form['date'] + ' ' + request.form['time'],
                "%Y-%m-%d %H:%M"
            ),
            city=request.form['city'],
            state=request.form['state'],
            country=request.form['country'],
            lat=request.form['lat'],
            lng=request.form['lng']
        )
        db_session.add(meal)
        for uuid in request.form.getlist('image_uuids'):
            photo = MealPhoto(meal=meal, uuid=uuid)
            db_session.add(photo)
        db_session.flush()
        return redirect(url_for('meal.detail_meal', meal_id=meal.id))


@mod.route('/new', methods=['GET'])
@login_required
def meal_form():
    return render_template('meal_form.html')


@mod.route('/<int:meal_id>/delete', methods=['POST'])
@login_required
def delete_meal(meal_id):
    meal = Meal.query.get_or_404(meal_id)
    flag = meal.reservations.filter_by(approval=1).first()
    if flag:
        flash('예약승인이 된 회원이 존재합니다.')
    else:
        db_session.delete(meal)
        db_session.flush()
    return redirect(url_for('account.list_meals'))


@mod.route('/<int:meal_id>', methods=['GET'])
def detail_meal(meal_id):
    meal = Meal.query.get_or_404(meal_id)
    reviews = Review.query.filter(Review.meal_id == meal_id,
                                  Review.author_id == Review.guest_id)
    photos = meal.photos
    if 'id' in session:
        rsv = Reservation.query.filter_by(meal_id=meal.id,
                                          guest_id=session['id']).first()
        reservations = meal.reservations
        return render_template('meal_detail.html', meal=meal,
                               reservation=rsv, reservations=reservations,
                               reviews=reviews, photos=photos)
    else:
        return render_template('meal_detail.html', meal=meal, reviews=reviews,
                               photos=photos)


@mod.route('/<int:meal_id>/edit', methods=['GET', 'POST'])
@login_required
def update_meal(meal_id):
    if request.method == 'GET':
        meal = Meal.query.get_or_404(meal_id)
        return render_template('meal_form.html', meal=meal)
    elif request.method == 'POST':
        meal = Meal.query.get_or_404(meal_id)
        if meal:
            if request.form['title']:
                meal.name = request.form['title']
            if request.form['price']:
                meal.price = request.form['price']
            if request.form['description']:
                meal.description = request.form['description']
            if request.form['date'] and request.form['time']:
                meal.datetime = datetime.strptime(
                    request.form['date'] + ' ' + request.form['time'],
                    "%Y-%m-%d %H:%M")
            db_session.add(meal)
            if request.form.getlist('image_uuids'):
                MealPhoto.query.filter_by(meal_id=meal.id).delete()
                for uuid in request.form.getlist('image_uuids'):
                    photo = MealPhoto(meal=meal, uuid=uuid)
                    db_session.add(photo)
            db_session.flush()
            return redirect(url_for('meal.detail_meal', meal_id=meal_id))
        else:
            abort(403)


@mod.route('/<int:meal_id>/reservations/<int:guest_id>', methods=['POST'])
@login_required
@check_ownership(target='guest_id')
def reservation(meal_id, guest_id):
    meal = Meal.query.get_or_404(meal_id)
    iamport = Iamport(imp_key=app.config['IAMPORT_API_KEY'],
                      imp_secret=app.config['IAMPORT_API_SECRET'])
    if not iamport.is_paid(meal.price, imp_uid=request.form['payment_id']):
        iamport.cancel('Bad request', imp_uid=request.form['payment_id'])
        flash('잘못된 접근입니다.')
        raise BadRequest()
    else:
        reservations = Reservation(
            meal_id=meal_id,
            guest_id=guest_id,
            payment_id=request.form['payment_id']
        )
        db_session.add(reservations)
        db_session.flush()
        return redirect(url_for('meal.detail_meal', meal_id=meal_id))


@mod.route('/<int:meal_id>/reservations/<int:guest_id>/cancel',
           methods=['POST'])
@login_required
@check_ownership(target='guest_id')
def cancel_reservation(meal_id, guest_id):
    reservation = Reservation.query.filter_by(
        meal_id=meal_id,
        guest_id=guest_id
    ).first()
    if reservation.payment_id:
        # cancel payment transaction
        iamport = Iamport(imp_key=app.config['IAMPORT_API_KEY'],
                          imp_secret=app.config['IAMPORT_API_SECRET'])
        iamport.cancel('예약 취소', imp_uid=reservation.payment_id)

    db_session.delete(reservation)
    db_session.flush()
    return redirect(url_for('meal.detail_meal', meal_id=meal_id))


@mod.route('/<int:meal_id>/reservations/<int:guest_id>/approve',
           methods=['POST'])
@login_required
def approve_reservation(meal_id, guest_id):
    meal = Meal.query.get_or_404(meal_id)

    reservation = meal.reservations.filter_by(guest_id=guest_id).first()
    reservation.approval = 1
    db_session.flush()
    return redirect(url_for('meal.detail_meal', meal_id=meal_id))
