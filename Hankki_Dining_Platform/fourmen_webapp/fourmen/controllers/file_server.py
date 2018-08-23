import uuid
from flask import Blueprint, jsonify
from minio import Minio, ResponseError
from fourmen import app

minio_client = Minio(app.config['MINIO_API_URL'],
                     access_key=app.config['MINIO_ACCESS_KEY'],
                     secret_key=app.config['MINIO_SECRET_KEY'],
                     secure=True)

mod = Blueprint('file_server', __name__, url_prefix='/file_server')


@mod.route('/put_presigned_url', methods=['POST'])
def put_presigned_url():
    try:
        object_uuid = str(uuid.uuid4())
        presigned_url = minio_client.presigned_put_object('fourmen',
                                                          object_uuid)
        return jsonify({"presigned_url": presigned_url,
                        "object_uuid": object_uuid})
    except ResponseError as err:
        print(err)


@mod.route('/get_presigned_url/<string:object_uri>', methods=['GET'])
def get_presigned_url(object_uuid):
    try:
        return minio_client.presigned_get_object(
            'fourmen', object_uuid
        )
    except ResponseError as err:
        print(err)
