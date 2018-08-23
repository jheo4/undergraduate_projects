from sqlalchemy import Column, ForeignKey, Integer, Boolean, String,\
                       DateTime, Text, func
from sqlalchemy.orm import relationship
from fourmen.models.database import base


class HostApplication(base):
    __tablename__ = 'host_applications'

    id = Column(Integer, primary_key=True)
    user_id = Column(Integer, ForeignKey('users.id'), nullable=False)
    check = Column(Boolean, default=False)
    created_at = Column(DateTime, default=func.now())
    description = Column(Text)
    bank_name = Column(String(10), default=False)
    account_number = Column(String(20), default=False)

    documents = relationship('HostDocument', back_populates='application')
    user = relationship('User', back_populates='application')
