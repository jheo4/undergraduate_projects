from sqlalchemy import Column, ForeignKey, Integer, String
from sqlalchemy.orm import relationship
from fourmen.models.database import base


class Reservation(base):
    __tablename__ = 'reservations'
    meal_id = Column(Integer, ForeignKey('meals.id'), primary_key=True)
    guest_id = Column(Integer, ForeignKey('users.id'), primary_key=True)
    approval = Column(Integer, default=0)
    payment_id = Column(String(32))
    request_msg = Column(String(100))

    meal = relationship('Meal', back_populates='reservations')
    guest = relationship('User', back_populates='reservations')
    reviews = relationship('Review', back_populates='reservation')
