from flask import Blueprint, render_template
from sqlalchemy import func

from fourmen.models import db_session, Meal, Review


mod = Blueprint('index', __name__)


@mod.route('/', methods=['GET'])
def index():
    # TODO: ES query(geo distance)
    #       -> 주변 반경 50km 내에 별점 4점이상인 식사 리스트 출력
    trending_meal_ids = db_session \
        .query(Review.meal_id) \
        .filter(Review.author_id == Review.guest_id) \
        .group_by(Review.meal_id) \
        .order_by(func.avg(Review.rating).desc()) \
        .limit(30) \
        .all()

    meals = Meal.query.filter(Meal.id.in_(trending_meal_ids)).all()
    meal_idx = {meal.id: meal for meal in meals}
    popular_meals = [meal_idx[id] for id, in trending_meal_ids]

    return render_template("main.html", meals=popular_meals)
