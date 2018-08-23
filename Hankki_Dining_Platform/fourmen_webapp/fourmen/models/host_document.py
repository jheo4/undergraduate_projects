from sqlalchemy import Column, String, Integer, ForeignKey
from sqlalchemy.orm import relationship
from fourmen.models.database import base
from fourmen.controllers.file_server import get_presigned_url


class HostDocument(base):
    __tablename__ = 'host_documents'

    id = Column(Integer, primary_key=True)
    application_id = Column(Integer, ForeignKey('host_applications.id'),
                            nullable=False)
    uuid = Column(String(256), nullable=False)
    application = relationship('HostApplication', back_populates='documents')

    @property
    def host_document_url(self):
        return get_presigned_url(object_uuid=self.uuid)
