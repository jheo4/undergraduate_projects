from functools import wraps
from flask import session, redirect, url_for, request, abort


blueprints = frozenset([
    'account',
    'index',
    'meal',
    'file_server',
    'review',
    'dining',
    'admin',
    'message'
])


def login_required(func):
    @wraps(func)
    def wraped(*args, **kwargs):
        if 'id' in session:
            return func(*args, **kwargs)
        else:
            return redirect(url_for('account.signin',
                                    return_url=request.full_path))
    return wraped


def check_ownership(target):
    def decorator(func):
        @wraps(func)
        def wraped(*args, **kwargs):
            if session['id'] == kwargs.get(target):
                return func(*args, **kwargs)
            else:
                abort(403)
        return wraped
    return decorator
