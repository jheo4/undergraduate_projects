
class BaseConfig(object):
    DEBUG = False
    TESTING = False
    SECRET_KEY = '4men-secret'


class DevelopConfig(BaseConfig):
    DEBUG = True
    TESTING = True
    SERVER_NAME = 'local.huntrax.com'
    SQLALCHEMY_DATABASE_URI = 'postgres://fourmen:fourmen@dining-db:5432/dining'
    SQLALCHEMY_TRACK_MODIFICATIONS = False

    MINIO_ACCESS_KEY = 'BWC0AAJ01XPZHIR80EY6'
    MINIO_SECRET_KEY = 'KAjBejHq0iJSjPfbucFVQdkIAmFnmmeZcjQx5HQF'
    MINIO_API_URL = "local.huntrax.com:443"
    MINIO_BUCKET = "fourmen"

    IAMPORT_API_KEY = '6265905865507685'
    IAMPORT_API_SECRET = 'Ix2pXHyBYb6WeELSekUVZKt69veuNqRvXTVVAuwyGDvUhgMvAHrzrUGBqU6ekXKiQFdraJ0QmvLELy9m'
