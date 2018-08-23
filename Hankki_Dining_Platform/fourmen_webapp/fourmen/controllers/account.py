from flask import Blueprint, request, session, \
    url_for, render_template, flash, redirect
from sqlalchemy import func

from fourmen.models.database import db_session
from fourmen.models.user import User
from fourmen.models.host_document import HostDocument
from fourmen.models.host_application import HostApplication
from fourmen.models.meal import Meal

from fourmen.util.helpers import render
from . import login_required


mod = Blueprint('account', __name__, url_prefix='/account')


@mod.route('/signin', methods=['GET', 'POST'])
def signin():
    if request.method == 'GET':
        return render_template('signin.html')
    elif request.method == 'POST':
        u = User.query.filter_by(username=request.form['username']) \
            .first()
        if u and u.validate_password(request.form['password']):
            session['id'] = u.id
            flash('Good to see you again, ' + u.name)
            if request.args.get('return_url'):
                return redirect(request.args['return_url'])
            return redirect(url_for('index.index'))
        else:
            flash('Failed to login. Please check your password or id.')
            return render(template='signin.html'), 400


@mod.route('/signout')
def signout():
    if 'id' in session:
        session.clear()
        flash('Bye! We hope we can meet each other soon!')
        return redirect(url_for('index.index'))


@mod.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.method == 'GET':
        return render_template('signup.html')

    elif request.method == 'POST':
        u = User(
            username=request.form['username'],
            password=request.form['password'],
            email=request.form['email'],
            name=request.form['name'],
        )
        # check the accordance btw passwords
        if request.form['password'] != request.form['confirmed_password']:
            flash('the confirmed password is different from the password.')
            return render_template('signup.html'), 400

        # check the overlapped ID & E-mail
        if User.query.with_deleted.filter_by(username=u.username).first():
            flash('Username is already in use.')
            return render_template('signup.html'), 400
        if User.query.filter_by(email=u.email).first():
            flash('email is already in use.')
            return render_template('signup.html'), 400
        db_session.add(u)
        db_session.flush()
        session['id'] = u.id
        flash('Hello, ' + u.name + '! It\'s lovely to meet you!')
        return render(redirect=url_for('index.index'), user=u)


@mod.route('/modification', methods=['GET', 'POST'])
@login_required
def modification():
    u = User.query.get_or_404(session['id'])
    if request.method == 'GET':
        return render_template('account_modification.html', user=u)
    elif request.method == "POST":
        if u and u.validate_password(request.form['password']):
            if request.form['new_password']:
                u.password = request.form['new_password']
            if request.form['name']:
                u.name = request.form['name']
            if request.form['email']:
                u.phone_number = request.form['email']
            if request.form['profile_uuid']:
                u.profile_uuid = request.form['profile_uuid']
            db_session.add(u)
            db_session.flush()
            return redirect(url_for('index.index'))
        flash('The password you entered was incorrect!')
        return render_template('account_modification.html')


@mod.route('/unregister', methods=['POST'])
@login_required
def unregister():
    u = User.query.get(session['id'])
    if u.meals:
        flash('You have proceeding meals')
        return render_template('account_modification.html', user=u)

    u.deleted_at = func.now()
    db_session.flush()
    session.clear()
    return redirect(url_for('index.index'))


@mod.route('/meals', methods=['GET'])
@login_required
def list_meals():
    meals = Meal.query.filter_by(host_id=session['id'])
    upcomings = meals.filter(~Meal.is_expired).all()
    expireds = meals.filter(Meal.is_expired).all()
    return render_template('account_meals.html', upcomings=upcomings,
                           expireds=expireds)


@mod.route('/<int:user_id>', methods=['GET'])
def view_profile(user_id):
    user = User.query.get(user_id)
    return render(template='account_show.html',
                  user=user)


@mod.route('/reviews', methods=['GET'])
@login_required
def list_reviews():
    # reviews about you
    user = User.query.get(session['id'])
    return render_template('account_reviews.html',
                           reviews_from_host=user.reviews_about_you('host'),
                           reviews_from_guest=user.reviews_about_you('guest'),
                           reviews_to_host=user.reviews_by_you('host'),
                           reviews_to_guest=user.reviews_by_you('guest'))


@mod.route('/host_documents', methods=['GET', 'POST'])
@login_required
def upload_host_documents():
    if request.method == 'GET':
        return render_template('account_be_host.html')
    elif request.method == 'POST':
        application = HostApplication(
            user_id=session['id'], bank_name=request.form['banks'],
            description=request.form['description'],
            account_number=request.form['account_numbers'])
        db_session.add(application)
        for uuid in request.form.getlist('image_uuids'):
            document = HostDocument(application=application, uuid=uuid)
            db_session.add(document)
        db_session.flush()
        flash('your documents are submmited.')
        return 'OK'
