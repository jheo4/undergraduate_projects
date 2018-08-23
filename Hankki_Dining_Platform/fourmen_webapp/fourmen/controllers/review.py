from flask import Blueprint, request, session, \
    url_for, render_template, redirect
from fourmen.controllers import login_required
from fourmen.models import db_session, Meal, Review, User
from datetime import date

mod = Blueprint('review', __name__, url_prefix='/reviews')


@mod.route('/', methods=['GET', 'POST'])
@login_required
def create_review():
    if request.method == 'GET':
        meal = Meal.query.get(request.args['meal_id'])
        user = User.query.get(session['id'])
        guest = User.query.get(request.args['guest_id'])
        author_type = request.args['author_type']
        return render_template('review_form.html', author_type=author_type,
                               meal=meal, user=user, guest=guest)
    elif request.method == 'POST':
        review = Review(
            meal_id=request.form['meal_id'],
            guest_id=request.form['guest_id'],
            author_id=session['id'],
            date=date.today(),
            content=request.form['content'],
            rating=request.form['rating']
        )
        db_session.add(review)
        db_session.flush()
        return redirect(url_for('dining.list_dining'))
