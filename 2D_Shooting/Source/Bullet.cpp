#include "Bullet.h"


CBullet::CBullet(void)
{
	rtPoint.top = -1;
	rtPoint.bottom = -1;
	rtPoint.left = -1;
	rtPoint.right = -1;
	bLife = FALSE;
	iAP = 0;
	bDying = FALSE;
}


CBullet::~CBullet(void)
{
}
