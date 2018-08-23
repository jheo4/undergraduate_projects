from sqlalchemy import Column, ForeignKey, Integer, DateTime, Text, func
from sqlalchemy.orm import relationship
from fourmen.models.database import base


class Message(base):
    __tablename__ = 'messages'

    id = Column(Integer, primary_key=True)
    sender_id = Column(Integer, ForeignKey('users.id'))
    receiver_id = Column(Integer, ForeignKey('users.id'))
    meal_id = Column(Integer, ForeignKey('meals.id'))
    content = Column(Text, nullable=False)
    created_at = Column(DateTime(timezone=True), default=func.now())

    sender = relationship('User', back_populates='sent_messages',
                          foreign_keys=sender_id)
    receiver = relationship('User', back_populates='received_messages',
                            foreign_keys=receiver_id)
    meal = relationship('Meal', back_populates='messages')
