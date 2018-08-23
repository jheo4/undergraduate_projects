from sqlalchemy import Column, String, Integer, ForeignKey
from sqlalchemy.orm import relationship
from fourmen.models.database import base
from fourmen.controllers.file_server import get_presigned_url


class MealPhoto(base):
    __tablename__ = 'meal_photos'

    id = Column(Integer, primary_key=True)
    meal_id = Column(Integer, ForeignKey('meals.id'), nullable=False)
    uuid = Column(String(256), nullable=False)
    meal = relationship('Meal', back_populates='photos')

    @property
    def meal_photo_url(self):
        return get_presigned_url(object_uuid=self.uuid)
