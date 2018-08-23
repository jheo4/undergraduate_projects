from flask_sqlalchemy import BaseQuery, SQLAlchemy
from sqlalchemy.orm import Mapper
from fourmen import app


class QueryWithSoftDelete(BaseQuery):
    _with_deleted = False

    def __new__(cls, *args, **kwargs):
        obj = super(QueryWithSoftDelete, cls).__new__(cls)
        obj._with_deleted = kwargs.pop('_with_deleted', False)
        if len(args) > 0:
            super(QueryWithSoftDelete, obj).__init__(*args, **kwargs)
            query_entity = args[0].entity if isinstance(args[0], Mapper) \
                else args[0]
            if hasattr(query_entity, "deleted_at"):
                return obj.filter_by(
                    deleted_at=None) if not obj._with_deleted else obj
        return obj

    def __init__(self, *args, **kwargs):
        pass

    @property
    def with_deleted(self):
        return self.__class__(db.class_mapper(self._mapper_zero().class_),
                              session=db.session(), _with_deleted=True)

    def _get(self, *args, **kwargs):
        # this calls the original query.get function from the base class
        return super(QueryWithSoftDelete, self).get(*args, **kwargs)

    def get(self, *args, **kwargs):
        # the query.get method does not like it if there is a filter clause
        # pre-loaded, so we need to implement it using a workaround
        obj = self.with_deleted._get(*args, **kwargs)
        if obj is None:
            return
        elif self._with_deleted or not getattr(obj, 'deleted_at', None):
            return obj


db = SQLAlchemy(app, query_class=QueryWithSoftDelete)
db.session.configure(autocommit=True, autoflush=False)
db_session = db.session

# Set the base
base = db.Model
base.query = db.session.query_property(query_cls=QueryWithSoftDelete)


def init_db(drop=False):
    if drop:
        base.metadata.drop_all(bind=db.engine)
    base.metadata.create_all(bind=db.engine)
