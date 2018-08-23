#pragma once
//Map
#define MAPIMG_Y 800
#define MAP_Y 750
#define MAP_X 500

// My Unit & Bullet
#define MYUNIT_W 40
#define MYUNIT_H  40
#define MYBULLET_W 3
#define MYBULLET_H 30
#define MYBULLETBOOM_W 10
#define MYBULLETBOOM_H 10

// Mutal Define
#define MUTAL_W 30
#define MUTAL_H 30
#define MUTALBULLET_W 10
#define MUTALBULLET_H 10
#define YAMATO_W 30
#define YAMATO_H 80

// Mutal Define
#define SCOURGE_W 15
#define SCOURGE_H 15
#define ZERGBOOM_W 35
#define ZERGBOOM_H 35

// Game State
enum{LOADING=0, MENU, START, RESTART, LOSE};
// Menu State
enum{NORMAL=0, NOTYET, LOVEHHJ, DIRECTION};

// MenuRect
enum{SINGLE=0, EDITOR, MULTI, EXIT, INTRO};
/*
Rectangle(hdc, 50, 50, 380, 230);	//single Play
Rectangle(hdc, 400, 170, 600, 400);	//campaign editor
Rectangle(hdc, 50, 300, 380, 600);	//multiplay
Rectangle(hdc, 430, 500, 700, 750);	//EXIT
Rectangle(hdc, 20, 680, 250, 750);	//Intro;*/