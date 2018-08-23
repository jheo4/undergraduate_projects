from operator import attrgetter
from itertools import groupby
from flask import Blueprint, session, render_template
from fourmen.controllers import login_required
from fourmen.models import Meal, Reservation

mod = Blueprint('dining', __name__, url_prefix='/dinings')


@login_required
@mod.route('/', methods=['GET'])
def list_dining():
    rsvs = Reservation.query.filter_by(guest_id=session['id']).all()
    meals = Meal.query.filter(Meal.id.in_([rsv.meal_id for rsv in rsvs]))
    rsvs = {a: [r.meal_id for r in rsvs]
            for a, rsvs in groupby(rsvs, attrgetter('approval'))}
    meals = {meal.id: meal for meal in meals}
    reserved = [meals[r] for r in rsvs.get(1, [])]
    pending = [meals[r] for r in rsvs.get(0, [])]
    return render_template('dining_main.html', reserved=reserved,
                           pending=pending)
