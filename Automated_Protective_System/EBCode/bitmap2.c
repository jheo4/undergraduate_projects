#include <stdio.h>
#include <stdlib.h> // for exit
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/mman.h>
#include <linux/fb.h> // for fb_var_screeninfo, FBIOGET_VSCREENINFO
#include <pthread.h>
#include "bitmap.h"
#include "TouchPanel.h"
#include "camera.h"//header code from TA
#include "team4camera.h"//our code.. 
#include "Global.h"
//#include <signal.h>

#define FBDEV_FILE "/dev/fb0"
#define BIT_VALUE_24BIT 24



int ch_curPage;
int numOfObj[3]={0,0,0};//obj num in each page(3page)

//pages. declare should be global
ch_drawObj page0[4];
ch_drawObj page1[5];
ch_drawObj page2[3];

//layout
ch_drawObj* layout[3];



//--------------------------main!!!--------------------------------------------------

void Oh_TOUCHCAM(){

	//draw objects

	//page1 obj
	ch_drawObj testBtn1;
	ch_drawObj testBtn2;
	ch_drawObj testBtn3;

	ch_drawObj testBtn4;

	ch_drawObj btnBack;
	ch_drawObj logo1;
	ch_drawObj logo2;
	
	
//mutual obj
	btnBack.xposition=0;
	btnBack.yposition=0;
	btnBack.filename="btn/go_page.bmp";


	logo2.xposition=1152;
	logo2.yposition=0;
	logo2.filename="btn/logo.bmp";


	//set draw object
	//page0 obj
	logo1.xposition=0;
	logo1.yposition=0;
	logo1.filename="btn/logo.bmp";

	testBtn2.xposition=128;
	testBtn2.yposition=0;
	testBtn2.filename="btn/bunker_project.bmp";
	
	testBtn3.xposition=400;
	testBtn3.yposition=250;
	testBtn3.filename="btn/bunker3.bmp";

	testBtn4.xposition=400;
	testBtn4.yposition=450;
	testBtn4.filename="btn/check_camera.bmp";

	//page1 obj--bunker controll
	ch_drawObj bunkerTitle;
	ch_drawObj btnOpen;
	ch_drawObj btnClose;

	bunkerTitle.xposition=128;
	bunkerTitle.yposition=0;
	bunkerTitle.filename="btn/bunker3.bmp";

	
	btnOpen.xposition=250;
	btnOpen.yposition=300;
	btnOpen.filename="btn/btn_open.bmp";	

	btnClose.xposition=600;
	btnClose.yposition=300;
	btnClose.filename="btn/btn_close.bmp";	

	//page2 --camera..
	/*btnGoPage2_1.xposition=200;
	btnGoPage2_1.yposition=200;
	btnGoPage2_1.filename="go_page1.bmp";

	btnGoPage2_2.xposition=400;
	btnGoPage2_2.yposition=200;
	btnGoPage2_2.filename="go_page2.bmp";
	*/
	ch_drawObj cameraTitle;
	cameraTitle.xposition=128;
	cameraTitle.yposition=0;
	cameraTitle.filename="btn/go_camera.bmp";
	



	//pages
	page0[0]=logo1;
	page0[1]=testBtn2;

	page0[2]=testBtn3;
	page0[3]=testBtn4;


	numOfObj[0]=4;//버튼추가하면 여기랑 맨위에 배열크기늘려야됨.
	//page0+sizeof(ch_drawObj)=&testBtn2;

	page1[0]=btnBack;
	page1[1]=bunkerTitle;
	page1[2]=logo2;
	page1[3]=btnOpen;
	page1[4]=btnClose;
	numOfObj[1]=5;

	page2[0]=btnBack;
	page2[1]=cameraTitle;
	page2[2]=logo2;
	numOfObj[2]=3;

	//layout=malloc(sizeof(page1)*3);//3page allocate
	layout[0]=page0;
	layout[1]=page1;
	layout[2]=page2;
	//layout+sizeof(page0)=page1;
	//layout+sizeof(page0)*2=page2;

//--------------------------------------------------------------------------------------------------
	//set fb_mapped as display start memory
	ch_setTouchPanel();//code for EB/when testing not in EB, hide this code;

	//clear(set white all) 
	ch_clearScreen();//clear

	//drawpage(1);
//리눅스에선 위에 주석할것 -------------------------------------------------------
	ch_drawpage(0);
	//startCamera();//threadCamera();
	ch_checkTouchPanel();//리눅스에선 주석할것

	return 0;
}



/*--------------------------------------------------------------------------
				draw part
----------------------------------------------------------------------------*/


//functions
//read bmp, store value in data


void ch_drawpage(int pageNum){
	printf("fbmapped:%x",fb_mapped);
	ch_curPage=pageNum;
	printf("cur_page:%d\n",ch_curPage);

	//fb_mapped;//TouchPanel.h에서 시작주소를 fb_mapped 로할당(init메소드실행후에)
	
	
	
//printf("sizeof(layout0):%d,sizeof(drawObj):%d\n",sizeof(layout[pageNum]),sizeof(ch_drawObj));//원래 페이지에 있는 엘리먼트갯수 구하려고햇는데 사이즈잘안나와서 걍 아래 스위치문으로 넣음;
	int numOfElement=numOfObj[pageNum];
	printf("pageNum:%d\n",pageNum);
	printf("numOfElement:%d\n",numOfObj[pageNum]);
	
	int i,j,k=0;
	ch_drawObj* tem;
	printf("numofelement:%d\n",numOfElement);

	for(k=0;k<numOfElement;k++)
	{
		printf("drawing object_page:%d/obj_num:%d\n",pageNum,k);
		tem=&layout[pageNum][k];
		printf("\ndrawingobj_name:%s",tem->filename);	
		//read file. and close file
	char *pData;
	char *data;
	unsigned long *ptr;	
		ch_read_bmp(tem->filename, &pData, &data, &(tem->xlength), &(tem->ylength));
			
	//	printf("\nbefore close bmp, sizeof(data):%d\n",sizeof(data));
		
		printf("close bmp pdata\n");	

	//	ptr=malloc(sizeof(data));//test code for linux
		int a=1;
	//	printf("xlength:%d,ylength:%d\n",tem->xlength,tem->ylength);
	//	printf("fb_mapped:%d, ch_screen_width:%d, yposi:%d,xpos:%d",fb_mapped,ch_screen_width,tem->yposition,tem->xposition);
		printf("tem:%d\n",tem->ylength);
		int b,g,r;
		long p;
		long pixel;

		for (j = 0; j<tem->ylength; j++)
		{
			//printf("%d,%d,%d,%d\n",fb_mapped,ch_screen_width,tem->yposition,tem->ylength);
//------------------------------------------------------------------------------------
			ptr=(unsigned long*) fb_mapped +ch_screen_width*((tem->yposition+tem->ylength)-j);//linux에서할때는 주석할것
//------------------------------------------------------------------------------------------------------------

			//printf("good!%d,%d,%d\n",tem->xlength,j,j*3*(tem->xlength));
			p = j*3*(tem->xlength);
	//		int t;t = (tem.ylength - 1 - j) * tem.xlength;
			//printf("p ");

			for (i = 0; i<tem->xlength; i++)
			{
				
	//			printf("good!%d",k);
				//printf("");
				b = *(data + (p + i * 3));
				g = *(data + (p + i * 3 + 1));
				r = *(data + (p + i * 3 + 2));

				pixel = ((r << 16) | (g << 8) | b);
				//printf("k :%x / ",pixel);
//--------------------------------------------------------------------------------------
				*(ptr+tem->xposition+i)=pixel;//리눅스에선 주석할것
//-----------------------------------------------------------------------------------------
			}
		}

		printf("\nafter draw\n");
	}



}





/*--------------------------------------------------------
		button action
---------------------------------------------------------*/

void ch_btnAct(int pageNum, int btnNum){
	switch(pageNum){
		case 0://page0==home
			if(btnNum==0)//go page0 home
			{
				//ch_clearScreen();
				//ch_drawpage(0);

			}		
			else if(btnNum==1)//project name -- do nothing
			{	//ch_clearScreen();
				//ch_drawpage(1);

			}	
			else if(btnNum==2)//go bunker control(page1)
			{
				ch_clearScreen();
				ch_drawpage(1);

			}
			else if(btnNum==3)//go camera check (page2)
			{
				ch_clearScreen();
				ch_drawpage(2);
				startCamera();
				ch_checkTouchPanel();
			}
			break;
		case 1://page1
			if(btnNum==0)//go back(page0)
			{	ch_clearScreen();
				ch_drawpage(0);
			}		
			else if(btnNum==1)//name
			{	//ch_clearScreen();
				//ch_drawpage(2);
			}
			else if(btnNum==2)//logo
			{	//ch_clearScreen();
				//ch_drawpage(1);
			}
			else if(btnNum==3){//open bunker
				if(iBunkerState == CLOSE && iOpenSignal==0){
					iBunkerState = OPEN;
					iOpenSignal = 1;
					printf("OPEN BUTTON!\n");
				}
			}
			else if(btnNum==4){//close bunker
				if(iBunkerState == OPEN && iCloseSignal==0){
					iBunkerState = CLOSE;
					iCloseSignal = 1;
					printf("CLOSE BUTTON!\n");
				}
			}
			break;
		case 2://page2--camera
			if(btnNum==0)//go back(page0)
			{
				stopCamera();
				ch_clearScreen();
				ch_drawpage(0);
			}		
			else if(btnNum==1)//go page1
			{	ch_clearScreen();
				ch_drawpage(1);
			}
			else if(btnNum==1)//go page2
			{	ch_clearScreen();
				ch_drawpage(2);
			}
			break;
		case 3://page3
			if(btnNum==0)
			{	ch_clearScreen();
				ch_drawpage(0);
			}		
			else if(btnNum==1)
			{	ch_clearScreen();
				ch_drawpage(0);
			}
			break;
		default://default
			ch_clearScreen();
			ch_drawpage(0);
			break;
	}
}

void ch_checkTouchPanel()
{
	int btnNo=0;
	int pageNo=0;
	
	ch_drawObj* tem;
	while(1)
	{
		ch_readFirstCoordinate(fp,&x, &y);
		switch(ch_curPage){
			case 1:pageNo=1;break;
			case 2:pageNo=2;break;
			default: pageNo=0;break;
		}
		//printf("curpage:%d, page:%d, x:%d, y: %d\n",ch_curPage,pageNo,x,y);
		//printf("numOfObj:%d, ",numOfObj[ch_curPage]);
		for(btnNo=0;btnNo<numOfObj[ch_curPage];btnNo++){
			tem=&layout[pageNo][btnNo];
		//	printf("btnNo:%d, %d<=x<=%d, %d<=y<=%d\n",btnNo, tem->xposition,tem->xlength, tem->yposition, tem->ylength);
			if(tem->xposition<=x && x<=(tem->xposition+tem->xlength) && tem->yposition<= y && y<=(tem->yposition+tem->ylength)){
				//printf("act page:%d,btn:%d\n",pageNo,btnNo);
				ch_btnAct(pageNo,btnNo);//do but_back;
				break;
			}
		}
		//ch_drawCoordinate(fb_mapped,x,y, prex, prey);
		//prex = x;
		//prey = y;
	}

	ch_closeTouchDev();

}






