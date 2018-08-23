#!/bin/bash

# This host is just for locals.
# Please change `PORT` as yours.
ES="http://localhost:9200"
ESIDX="dining"

curl -XDELETE $ES/$ESIDX/
python3 -c "from fourmen.models.database import drop_db; drop_db()"
curl -XPUT $ES/$ESIDX --data-binary "@init_es.json"
python3 -c "from fourmen.models.database import init_db; init_db()"
