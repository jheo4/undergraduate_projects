from pytest import fixture
from flask import url_for
from fourmen.controllers.file_server import minio_client
from fourmen.models import db_session, User, HostApplication


@fixture
def user(client):
    user = {
        'username': 'allan',
        'password': 'allan',
        'confirmed_password': 'allan',
        'name': 'Allan Turing',
        'email': 'allan.t@gmail.com'
    }

    resp = client.post(url_for('account.signup'),
                       data=user).get_json()

    del user['confirmed_password'], user['password']
    for field, value in user.items():
        assert resp['user'][field] == user[field]
    yield user
    resp = client.post(url_for('account.unregister'))
    assert resp.status_code == 302
    user = User.query.with_deleted.filter_by(username='allan').first()
    assert user.deleted_at is not None
    if user.profile_uuid:
        minio_client.remove_object('fourmen', user.profile_uuid)
    db_session.delete(user)
    db_session.flush()


def test_user_modification(client, user):
    resp = client.post(url_for('file_server.put_presigned_url')).get_json()
    assert 'presigned_url' in resp and 'object_uuid' in resp
    filename = 'fourmen/static/img/elliot.jpg'
    minio_client.fput_object('fourmen', resp['object_uuid'], filename)
    user['password'] = 'allan'
    user['new_password'] = 'new_password'
    user['profile_uuid'] = resp['object_uuid']
    resp = client.post(url_for('account.modification'),
                       data=user)
    assert resp.status_code == 302
    resp = client.get(url_for('account.signout'))
    assert resp.status_code == 302
    resp = client.post(url_for('account.signin'),
                       data={'username': 'allan', 'password': 'new_password'})
    assert resp.status_code == 302


def test_host_application(client):
    user = {
        'username': 'test',
        'password': 'test',
        'confirmed_password': 'test',
        'name': 'test',
        'email': 'test@gmail.com'
    }
    resp = client.post(url_for('account.signup'),
                       data=user).get_json()

    resp = client.post(url_for('account.signin'),
                       data={'username': 'test', 'password': 'test'})
    assert resp.status_code == 302

    document_pictures = ['fourmen/static/img/dummy_1.jpg',
                         'fourmen/static/img/dummy_2.jpg']
    image_uuids = []
    for pic in document_pictures:
        resp = client.post(url_for('file_server.put_presigned_url')).get_json()
        assert 'presigned_url' in resp and 'object_uuid' in resp
        minio_client.fput_object('fourmen', resp['object_uuid'], pic)
        image_uuids.append(resp['object_uuid'])
    application = {
        'description': 'iwannabeahost',
        'banks': 'SC제일',
        'account_numbers': '614-20-248051',
        'image_uuids': image_uuids,
    }

    client.post(url_for('account.upload_host_documents'), data=application)
    app = HostApplication.query \
        .filter_by(check='f', bank_name=application['banks'],
                   account_number=application['account_numbers']) \
        .first()
    assert app.description == application['description']

    # deny
    application['deny'] = 'deny'
    client.post(url_for('admin.confirm_applicants', application_id=app.id),
                data=application)
    db_session.refresh(app)
    assert not app.user.is_host

    # confirm
    application['confirm'] = 'confirm'
    client.post(url_for('admin.confirm_applicants', application_id=app.id),
                data=application)
    db_session.refresh(app)
    assert app.user.is_host
