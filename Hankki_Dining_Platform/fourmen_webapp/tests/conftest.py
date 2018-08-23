import json
from functools import partial

import pytest
from flask import Response, url_for

from fourmen import app
from fourmen.models import db_session, User
from fourmen.models.database import init_db


def get_json(response, expected_status_code=200):
    assert response.status_code == expected_status_code
    assert response.content_type == 'application/json'
    return json.loads(response.get_data())


@pytest.fixture(scope="session")
def client():
    app.config.from_object('fourmen.configurations.DevelopConfig')
    client = app.test_client()
    if not hasattr(Response, 'get_json'):
        Response.get_json = get_json
    headers = {'Accept': 'application/json'}
    client.post = partial(client.post, headers=headers)
    client.get = partial(client.get, headers=headers)
    with app.app_context():
        init_db(drop=True)
        admin = User(
            username='admin',
            password='admin',
            email='admin@example.com',
            name='Admin',
            is_admin=True,
            is_host=True
        )
        db_session.add(admin)
        db_session.flush()
        yield client


@pytest.fixture(scope="session")
def dummy_host(client):
    user = {
        'username': 'dummy',
        'password': 'dummy',
        'confirmed_password': 'dummy',
        'name': 'Dummy User',
        'email': 'dummy@gmail.com'
    }
    resp = client.post(url_for('account.signup'),
                       data=user).get_json()
    del user['confirmed_password'], user['password']
    for field, value in user.items():
        assert resp['user'][field] == user[field]
    resp = client.post(url_for('account.signin'),
                       data={'username': user['username'],
                             'password': 'dummy'}
                       )
    assert resp.status_code == 302
    yield
