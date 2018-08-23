from elasticsearch import Elasticsearch
from sqlalchemy import Column, String, Integer, Text, Float,\
                        ForeignKey, DateTime, event, func
from sqlalchemy.orm import relationship
from sqlalchemy.ext.hybrid import hybrid_property
from fourmen.models.database import base, db_session
from .review import Review

es = Elasticsearch(
    ['dining-es']
)


class Meal(base):
    __tablename__ = 'meals'

    id = Column(Integer, primary_key=True)
    name = Column(String(256), nullable=False)
    host_id = Column(Integer, ForeignKey('users.id'))
    price = Column(Integer)
    datetime = Column(DateTime)
    lat = Column(Float(Precision=64))
    lng = Column(Float(Precision=64))
    city = Column(String(50))
    state = Column(String(50))
    country = Column(String(50))
    description = Column(Text, nullable=False)
    reservations = relationship('Reservation',
                                back_populates='meal',
                                cascade='delete',
                                lazy='dynamic')
    photos = relationship('MealPhoto', back_populates='meal',
                          cascade='delete', lazy='dynamic')
    host = relationship('User', back_populates='meals')
    messages = relationship('Message', back_populates='meal')

    @property
    def serialize(self):
        return {
            'id': self.id,
            'name': self.name,
            'host_id': self.host_id,
            'price': self.price,
            'time': self.time,
            'date': self.date,
            'host': {
                'name': self.host.name
            }
        }

    @property
    def average_rating(self):
        reviews = Review.query.filter(Review.meal_id == self.id,
                                      Review.author_id == Review.guest_id)
        if reviews:
            return db_session.query(func.avg(Review.rating)).\
                filter(Review.meal_id == self.id,
                       Review.author_id == Review.guest_id).first()[0]
        else:
            return 0

    @hybrid_property
    def is_expired(self):
        return self.datetime < func.now()


@event.listens_for(Meal, "after_insert")
def create_es(mapper, conn, meal):
    es.create(
        index="dining",
        doc_type="meal",
        id=meal.id,
        body={
            "id": meal.id,
            "pin": {
                "location": {
                    "lat": meal.lat,
                    "lon": meal.lng
                }
            },
            "name": meal.name,
            "description": meal.description,
        }
    )


@event.listens_for(Meal, "after_update")
def update_es(mapper, conn, meal):
    es.update(
        index="dining",
        doc_type="meal",
        id=meal.id,
        body={
            "doc": {
                "pin": {
                    "location": {
                        "lat": meal.lat,
                        "lon": meal.lng
                    }
                },
                "name": meal.name,
                "description": meal.description,
            }
        }
    )


@event.listens_for(Meal, "after_delete")
def delete_es(mapper, conn, meal):
    es.delete(
        index="dining",
        doc_type="meal",
        id=meal.id,
    )
