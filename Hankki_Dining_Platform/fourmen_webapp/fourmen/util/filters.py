from fourmen.models.review import Review
from fourmen.models.reservation import Reservation


def check_review(meal_id, guest_id, author_id):
    review = Review.query.filter_by(meal_id=meal_id,
                                    guest_id=guest_id,
                                    author_id=author_id).first()
    if review:
        return review
    return 0


def check_reservation(meal_id, guest_id):
    rsv = Reservation.query.filter_by(meal_id=meal_id,
                                      guest_id=guest_id).first()

    if rsv:
        return rsv
    return 0
