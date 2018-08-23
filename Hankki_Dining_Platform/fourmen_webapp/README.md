# Fourmen Web app 

## DB schema 변경시  
```bash
$ psql -U username -h IpAddr fourmen
>> \d  # table list
>> drop table 테이블 이름;  # 변경 사항 있는 테이블만 
>> \q  # 종료 

$ python 
>>> from fourmen.models.database import init_db
>>> init_db()
```