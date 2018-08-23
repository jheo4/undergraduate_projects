import bcrypt
from sqlalchemy import Column, String, Integer, Boolean, DateTime
from sqlalchemy.ext.hybrid import hybrid_property
from sqlalchemy.orm import relationship
from flask import url_for
from fourmen.models.database import base
from .meal import Meal
from .reservation import Reservation
from .review import Review
from fourmen.controllers.file_server import get_presigned_url


class User(base):
    __tablename__ = 'users'
    id = Column(Integer, primary_key=True)
    username = Column(String(15), unique=True, nullable=False)
    _password = Column('password', String(256), nullable=False)
    email = Column(String(50), unique=True, nullable=False)
    name = Column(String(50), nullable=False)
    is_admin = Column(Boolean, default=False)
    is_host = Column(Boolean, default=False)
    deleted_at = Column(DateTime, nullable=True)
    profile_uuid = Column(String(50), nullable=True)

    meals = relationship('Meal', back_populates='host')
    application = relationship('HostApplication', back_populates='user')
    reservations = relationship('Reservation', back_populates='guest')
    reviews = relationship('Review', back_populates='author')
    sent_messages = relationship('Message', back_populates='sender',
                                 foreign_keys='Message.sender_id')
    received_messages = relationship('Message', back_populates='receiver',
                                     foreign_keys='Message.receiver_id')

    @hybrid_property
    def password(self):
        if isinstance(self._password, str):
            return self._password.encode()
        else:
            return self._password

    @password.setter
    def password(self, value):
        self._password = bcrypt \
            .hashpw(value.encode(), bcrypt.gensalt()) \
            .decode('utf-8')

    def validate_password(self, password):
        return bcrypt.checkpw(password.encode(), self.password)

    @property
    def profile_image_url(self):
        if self.profile_uuid:
            profile_url = get_presigned_url(object_uuid=self.profile_uuid)
        else:
            profile_url = url_for('static', filename='img/elliot.jpg')
        return profile_url

    def reviews_about_you(self, author):
        reviews = []
        if author == 'host':
            reviews = Review.query.filter(Review.guest_id == self.id,
                                          Review.guest_id != Review.author_id
                                          )
        elif author == 'guest':
            reviews = Review.query \
                .join(Review.reservation) \
                .join(Reservation.meal) \
                .filter(Meal.host_id == self.id) \
                .filter(Review.author_id != self.id)

        return reviews

    def reviews_by_you(self, target):
        if target == 'host':
            return [review for review in self.reviews
                    if review.guest_id is self.id]
        elif target == 'guest':
            return [review for review in self.reviews
                    if review.guest_id is not self.id]

    @property
    def available_meals(self):
        return Meal.query.filter_by(host_id=self.id).filter(~Meal.is_expired)
