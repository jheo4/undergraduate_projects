from flask import Flask, render_template, session, g
from flask_assets import Bundle, Environment
from werkzeug.utils import import_string
from werkzeug.contrib.fixers import ProxyFix
from fourmen.controllers import blueprints
from fourmen.util import filters

# Create app
app = Flask(__name__, template_folder='templates',
            static_url_path='/static')
app.wsgi_app = ProxyFix(app.wsgi_app)

# Create app
# App configuration from objects
app.config.from_object('fourmen.configurations.DevelopConfig')

# Register Blueprints
for bp in blueprints:
    mod = import_string('fourmen.controllers.%s:mod' % bp)
    app.register_blueprint(mod)

# Register Jinja filters
for filter_name in filters:
    filter_ = import_string('fourmen.util.filters:%s' % filter_name)
    app.add_template_filter(filter_, filter_name)

# JS/CSS Bundle
bundles = {
    'vendor_js': Bundle(
        'js/jquery-3.3.1.min.js',
        'js/jquery-ui.min.js',
        'js/jquery.timepicker.min.js',
        'js/semantic.min.js',
        'js/slick.min.js',
        output='gen/vendor.js'),
    'common_js': Bundle(
        'js/upload.js',
        output='gen/common.js'),
    'main_js': Bundle(
        'js/main.js',
        'js/search.js',
        output='gen/main.js'),
    'meal_js': Bundle(
        'js/meal.js',
        'js/main.js',
        output='gen/meal.js'),
    'css_all': Bundle(
        'css/layout.css',
        'css/jquery.timepicker.min.css',
        'css/slick-carousel.css',
        output='gen/all.css'),
}

assets = Environment(app)
assets.register(bundles)


# Error handling
@app.errorhandler(403)
def forbidden(e):
    return render_template('403.html'), 403


@app.before_request
def current_user():
    from fourmen.models import User
    if 'id' in session:
        g.current_user = User.query.get(session['id'])
