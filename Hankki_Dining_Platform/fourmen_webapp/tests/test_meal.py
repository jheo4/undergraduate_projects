from pytest import fixture
from flask import url_for
from fourmen.controllers.file_server import minio_client
from fourmen.models import Meal, db_session, Reservation, User


@fixture
def dummy_guest(client):
    dummy_guest = {
        'username': 'qqqqq',
        'password': 'qqqqq',
        'confirmed_password': 'qqqqq',
        'name': 'qqqqq',
        'email': 'qqqqq@gmail.com'
    }

    resp = client.post(url_for('account.signup'),
                       data=dummy_guest).get_json()

    del dummy_guest['confirmed_password'], dummy_guest['password']
    for field, value in dummy_guest.items():
        assert resp['user'][field] == dummy_guest[field]
    yield dummy_guest
    resp = client.post(url_for('account.unregister'))
    assert resp.status_code == 302
    dummy_guest = User.query.with_deleted.filter_by(username='qqqqq').first()
    assert dummy_guest.deleted_at is not None
    if dummy_guest.profile_uuid:
        minio_client.remove_object('fourmen', dummy_guest.profile_uuid)
    db_session.delete(dummy_guest)
    db_session.flush()


@fixture
def meal(client, dummy_host):
    meal_pics = ['fourmen/static/img/dummy_1.jpg',
                 'fourmen/static/img/dummy_2.jpg']
    image_uuids = []
    for pic in meal_pics:
        resp = client.post(url_for('file_server.put_presigned_url')).get_json()
        assert 'presigned_url' in resp and 'object_uuid' in resp
        minio_client.fput_object('fourmen', resp['object_uuid'], pic)
        image_uuids.append(resp['object_uuid'])
    meal = {
        'title': 'Roasted Portobello Mushroom',
        'price': 27000,
        'date': '2999-06-30',
        'time': '19:00',
        'lat': '37.2839',
        'lng': '127.04',
        'city': 'Suwon',
        'state': 'Gyeonggi-do',
        'country': 'KR',
        'description': 'Roasted Portobello Mushroom with a blue crab \
                        & Roasted artichoke risotto and red pepper coulis',
        'image_uuids': image_uuids,
    }
    resp = client.post(url_for('meal.list_meal'),
                       data=meal)
    assert resp.status_code == 302
    m = Meal.query.order_by(Meal.id.desc()).first()
    assert m.name == meal['title']
    assert m.price == meal['price']
    resp = client.post(url_for('meal.reservation', guest_id=1, meal_id=m.id),
                       data={'payment_id': 1234444})
    assert resp.status_code == 403
    yield meal


def test_meal_deletion(client, meal, dummy_guest):
    m = Meal.query.order_by(Meal.id.desc()).first()
    assert m is not None

    guest = User.query.filter_by(username=dummy_guest['username']).first()
    resp = client.post(url_for('account.signin'),
                       data={'username': dummy_guest['username'],
                             'password': 'qqqqq'})
    assert resp.status_code == 302
    reservation = Reservation(
            meal_id=m.id,
            guest_id=guest.id,
            payment_id=1234444
    )
    db_session.add(reservation)
    db_session.flush()
    resp = client.post(url_for('meal.delete_meal', meal_id=m.id))
    assert resp.status_code == 302
