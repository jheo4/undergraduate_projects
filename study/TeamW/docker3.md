---
layout: default
---
[back](./docker2){: .btn.btn-default}

# Docker Basic3

[Docker Official Website](https://www.docker.com/){: target="_blank"}

## How To Use Docker?
Until Basic2 I just introduce the basic concepts and the basic commands of Docker. However, it is necessary to know how to use Docker in the real environment. 

![Docker Usage](dockerUsing.png)

There are some steps to use Docker for a basic application. 
1. Make a directory, DIR1
1. Write source codes into DIR1
1. Make Dockerfile into DIR1

	it is possible to install exsisting images from [Docker Hub](hub.docker.com){: target="_blank"}
```
FROM [BASE_IMAGE]:[VERSION] (IMAGE comes from Docker Hub)
COPY [SOURCE_CODE] [DESTINATION] (copy execution codes into a destination directory)
EXPOSE [PORT] 
```

1. build the docker image form Dockerfile
```
docker build -t [IMAGE_NAME]
```
1. run a container by using the built image
```
docker run [OPTIONS] IMAGE_NAME [COMMAND] [ARGUMENTS]
* [COMMAND] and [ARGUMENTS] can be included into Dockerfile by using CMD.
```
* Caution : If the source codes are changed, it should be rebuilt. it is quite annoying, so I mount the source code directory into a directory of the container.
```
docker run -p [PORT]:[PORT] -v [SOURCE]:[DESTINATION] IMAGE_NAME
```
There are two kinds of shared volume in Docker. 
- Shared volumes between Containers
- Shared volumes between Containers and the host

In this case, it's between Containers and the host.

## For Multiple Containers
Each Container represents reach process in Docker. Therefore, the multiple containers are used for the applications which multiple processes cooperate with each other. The communications between containers are necessary in this environment. For this purpose, "Docker Compose" is used.

To use Docker Compose, some steps are required.
2. write SUPERDIR/DIR1/Source_Codes
2. write SUPERDIR/DIR1/Dockerfile
2. write SUPERDIR/docker-compose.yml
```    
version : COMPOSE_VER
services : 
    SERVICE_NAME_1 :
    build : SERVICE_SOURCE_CODE_DIR
    volumes :
	    - SOURCE_VOL:DEST_VOL
    ports :
		- PORT:FORWARDING
```
2. write SUPERDIR/DIR2/Source_Codes
2. write SUPERDIR/DIR2/Dockerfile
2. append a new service to SUPERDIR/docker-compose.yml
```
version : COMPOSE_VER
services : 
    SERVICE_NAME_1 :
	    build : SERVICE_SOURCE_CODE_DIR or images : 
		volumes :
        	- SOURCE_VOL:DEST_VOL
        ports :
            - PORT:FORWARDING
    SERVICE_NAME_2 :
        build : SERVICE_SOURCE_CODE_DIR or images : IMAGE_NAME:VERSION
        volumes :
    	    - SOURCE_VOL:DEST_VOL
        ports :
            - PORT:FORWARDING
        depends_on :
            - SERVICE_NAME_1
```
2. install base images and build the images from source codes, and run the containers by images
```
docker-compose up -d
```

* Docker Composer will make a virtual network between containers. This includes the shared volumes between Containers. Services are connected by SERVICE_NAME.

## More For Dockerfile And Docker Compose
[Dockerfile Reference](https://docs.docker.com/engine/reference/builder/){: target="_blank"}

[Docker Compose Reference](https://docs.docker.com/compose/compose-file/){: target="_blank"}
