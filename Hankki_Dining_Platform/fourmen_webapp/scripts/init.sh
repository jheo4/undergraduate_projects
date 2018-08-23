#!/bin/bash

set -e
# Check DB status
until PGPASSWORD=$POSTGRES_PASSWORD \
    psql -h "$POSTGRES_HOST" -U "$POSTGRES_USER" -d "$POSTGRES_DB" -c '\q'; do
    >&2 echo "Postgres is unavailable - sleeping"
    sleep 5
done
>&2 echo "Postgres is up - executing command"

# Check ES status
while ! curl -s http://${ELASTICSEARCH_HOST}/_cluster/health; do sleep 1; done
curl -s "http://${ELASTICSEARCH_HOST}/_cluster/health?wait_for_status=green&timeout=10s"
curl -XPUT http://${ELASTICSEARCH_HOST}/dining/ \
    --data-binary "@scripts/init_es.json"
>&2 echo "Elasticsearch is up..."

service nginx start
coverage run -m pytest && coverage report
python3 run.py
