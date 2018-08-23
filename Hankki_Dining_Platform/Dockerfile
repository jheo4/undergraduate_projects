FROM ubuntu:18.04

# MAINTAINER is deprecated
LABEL author.name="Wonkyo Choe"
LABEL author.email="heysid@ajou.ac.kr"
LABEL description="Ajou Univ > 2018 Spring > SW Capstone Design > Fourmen"

# REF: https://docs.docker.com/engine/examples/postgresql_service/#installing-postgresql-on-docker
# build 시 warning 삭제하기 위한 ENV
ENV DEBIAN_FRONTEND noninteractive

# package 설치
RUN apt-get update && apt-get install -y \
        curl \
        apt-transport-https \
        software-properties-common \
        postgresql-client \
        python3 python3-pip \
        nginx
COPY conf/nginx.conf /etc/nginx/conf.d/default.conf

COPY fourmen_webapp/requirements.txt /tmp/requirements.txt
RUN pip3 install -r /tmp/requirements.txt
RUN pip3 install gunicorn
COPY fourmen_webapp fourmen
WORKDIR fourmen

CMD ./scripts/init.sh
VOLUME ["/etc/ssl/nginx", "/fourmen"]
EXPOSE 80 443
