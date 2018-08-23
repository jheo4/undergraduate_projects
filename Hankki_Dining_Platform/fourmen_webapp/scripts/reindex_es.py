from fourmen.models import Meal
from fourmen.models.meal import create_es

for meal in Meal.query:
    create_es(None, None, meal)
