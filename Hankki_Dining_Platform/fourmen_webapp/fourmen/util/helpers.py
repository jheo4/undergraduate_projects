import flask
from flask import jsonify, render_template, request
from collections import OrderedDict


def serialize(obj):
    result = OrderedDict()
    for key in obj.__mapper__.c.keys():
        if key.startswith('_'):
            continue
        if getattr(obj, key) is not None:
            result[key] = str(getattr(obj, key))
        else:
            result[key] = getattr(obj, key)

    return result


def render(template=None, redirect=None, **kwargs):
    mimetypes = request.accept_mimetypes
    if mimetypes.best == 'application/json':
        return jsonify({key: serialize(kwargs[key])
                        for key in kwargs})
    else:
        if redirect:
            return flask.redirect(redirect)
        elif template:
            return render_template(template, **kwargs)
