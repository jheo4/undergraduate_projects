---
layout: default
---
[back](./docker1){: .btn.btn-default} [Basic3](./docker3){: .btn.btn-default}

# Docker Basic2

[Docker Official Website](https://www.docker.com/){: target="_blank"}

## Basic Commands For Docker
Docker is for Linux. When someone downloads Docker for Mac or Windows, it work like a virtual machine. After starting Dockr, we can use docker commands.

    docker run [OPTIONS] IMAGES [COMMAND] [ARGUMENTS]

there are many options for Docker. The frequently used options are these:
-p : port fowarding
-d : background execution 
-e : environment varaialbes in Container
-v : mount directory
-link : link containers
--name : Container name
-it : combination of i and t options for terminal
-rm : exiting process makes Container exit.

## Docker Command Examples

For executing Ubuntu terminal,

	docker run ubuntu:17.0 /bin/bash

This executes Ubuntu 17.0 with bash shell.

For executing Tensorflow,

	docker run -d -p 8888:8888 -p 6006:6006 .../pydata_tensorflow:0.1

This executes Tensorflow with prot fowarding. 

## Docker Optinos
More than the "run" option, there are other options. the frequently used options are these:

	docker ps [OPTIONS] : list Container
	docker stop [OPTIONS] Container : stop Container
	docker rm [OPTIONS] Container : remove Container
	docker rmi [OPTIONS] Image : remove a image
	docker images [COMMAND] : manage images
	docker pull [OPTIONS] [NAME] : pull an image or a repository from a registry
	docker logs [OPTIONS] : print logs
	docker exec [OPTIONS] Container [COMMAND] : make an exec order to the existing Container
