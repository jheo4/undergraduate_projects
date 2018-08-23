from sqlalchemy import Column, Integer, Text, ForeignKey, Date
from sqlalchemy.schema import ForeignKeyConstraint
from sqlalchemy.orm import relationship
from fourmen.models.database import base
from fourmen.models.reservation import Reservation


class Review(base):
    __tablename__ = 'reviews'

    id = Column(Integer, primary_key=True)
    meal_id = Column(Integer)
    guest_id = Column(Integer)
    author_id = Column(Integer, ForeignKey('users.id'))
    date = Column(Date)
    content = Column(Text)
    rating = Column(Integer)

    author = relationship('User', back_populates='reviews')
    reservation = relationship('Reservation', back_populates='reviews')

    __table_args__ = (ForeignKeyConstraint(
        [meal_id, guest_id],
        [Reservation.meal_id, Reservation.guest_id]),
        {})
