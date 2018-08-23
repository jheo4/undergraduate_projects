---
layout: default
---
[back](../../sc2ai){: .btn.btn-default} 
[Get Started with SC2 API Template](./sc2APItemplate){: .btn.btn-default}

# The Project Structure of SC2 API

[Blizzard SC2 API Github link](https://github.com/Blizzard/s2client-proto){: target="_blank"}

## Setup SC2 API environment
At the very first time, to use SC2 API, we need a StarCraft II execution file. Up to our operating system, we can use commercial versions of StarCraft II for Windows and OS X. For Linux users, only headless versions are available.

After installing StarCraft II, it is required to download Maps and Replay packs provided by Blizzard. These files are already defined the SC2 API configurations. Therefore, it is easy to use without any modification for people who just start SC2 API. 

For building SC2 API client, follow [this document](https://github.com/Blizzard/s2client-api/blob/master/docs/building.md){: target="_blank"}. When the building process is successful, it's time to start SC2 API!


## Structure of SC2 API
If we followed the document explaining how to build sc2client-api, "build" folder is the target folder for cmake. In build folder, these folders are placed(for Windows). 
- build/bin
- build/examples
- build/tests
- build/src

These folders are built from s2client-api. Therefore, the codes in the s2client-api folder is everything for us.

There are bunch of folders, but what we need is "src", "tests", "examples". 

- src

The codes in src are the point of sc2client-api. For implement our own bot, it is necessary to well know this codes. As much as we know, we can make our bot better by using various functions. Especially, codes in "src/sc2api" must be analyzed.

- tests

This folder has codes for testing SC2 API. With this codes, we can figure out how to handle SC2 API. At the very first time, we don't need to know this codes. However, this codes are needed for doing more extended functions such as multiplayer-game and featuring game information.

- examples

We start from this folder. In examples, we can learn how to handle SC2 API. In next document, I will introduce my own bot by using tutorial.cc.
