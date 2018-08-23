#ifndef __BITMAP_H__
#define __BITMAP_H__


typedef struct ch_drawObj{
	int pageNum;
	char* filename;
	int xposition;
	int yposition;
	int xlength;
	int ylength;
} ch_drawObj;

typedef struct {
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	//unsigned short bfReserved2;
	unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} BITMAPINFOHEADER;//color table

typedef struct {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;

//Pixel data
typedef struct {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
} BITMAPINFO;

void close_bmp(char **pDib);
void bitmaptest(char* filename, unsigned char* pfbmap, int x_1, int y_1);

void ch_read_bmp(char *filename, char **pDib, char **data, int *cols, int *rows)
{
 
	unsigned char* pBmpImage;
     BITMAPFILEHEADER bfh;     // 비트맵 파일헤더를 선언합니다.
     BITMAPINFOHEADER bih;     // 비트맵 인포헤더를 선언합니다.
     memset(&bfh, 0, 14);   // 구조체를 초기화합니다.
     memset(&bih, 0, 40);//sizeof(BITMAPINFOHEADER)); 
     unsigned char trash[12];    // 쓸모없는 데이터를 읽어 들일 배열입니다.
     int trash_width;                    // 4bite 정렬을 위해 사용된 쓸모없는 데이터의 크기를 나타냅니다.
     int temp;                             // 4로 나눈 나머지 값을 저장하는데 사용합니다.
     FILE *fp;                              // 파일 포인터입니다.

     // 인수로 받은 파일명에 해당하는 비트맵 파일을 엽니다.
     fp = fopen(filename, "rb");
     if(fp == NULL)
          return -1;

     // 파일헤더를 앞서 선언한 bfh구조체로 읽어 들입니다.
     fread(&bfh, 14, 1, fp);
 
     // 읽어온 비트맵이 비트맵 파일이 맞는지 확인합니다. 0x4D42는 비트맵 파일임을 나타냅니다.
     if(bfh.bfType != 0x4D42)
     {
          fclose(fp);
          return -1;
     }

     // 인포헤더를 앞서 선언한 bih구조체로 읽어 들입니다.
     fread(&bih, 40, 1, fp);
 
     // 우리는 24비트 비트맵만을 처리할 것이기 때문에 비트맵이 24비트 형식이 아니면 종료합니다.
     if(bih.biBitCount != 24)
     {
            fclose(fp);
            return -1;
     }

     // 실제 비트맵 데이터를 읽어들일 메모리 부분을 할당합니다.
     // 24비트 비트맵은 하나의 픽셀이 Red, Green, Blue 각각 8비트씩으로 이루어져
     // 총 3바이트를 이루므로
     // 실제 필요한 메모리의 양은 비트맵의 가로x세로크기에 3을 곱합니다.
	*cols = bih.biWidth;
	*rows = bih.biHeight;
  
     *data = (unsigned char *)malloc(bih.biWidth * bih.biHeight * 3);
 
      // 메모리 할당이 이루어지지 않은 경우 파일을 닫고 종료합니다.
     if(*data == NULL)
     {
          fclose(fp);
          return -1;
     }
 
      // 비트맵의 가로폭이 4의 배수이면 그냥 한 방에 읽어옴.
      if((temp = (bih.biWidth*3) % 4) == 0)
           fread(*data, bih.biWidth*3, bih.biHeight, fp);
      else  // 그게 아니면 쓰레기 값을 걸러주면서 읽어옵니다.
     {
          // 쓰레기 값을 폭을 계산, 4의 배수일 경우는 맞지 않는 값이 계산되지만
          // 4의 배수일 경우 이쪽 분기로 들어오지 않으므로 상관없다.
          trash_width = (((bih.biWidth*3) + 4) - temp) - (bih.biWidth*3);
  
          // 한 줄씩 읽어온다.
	int Y;
          for(Y = 0; Y < bih.biHeight; Y++)
          {
               fread(*data + bih.biWidth*3 * Y, bih.biWidth, 3, fp);  // 데이터를 읽는다.
               fread(trash, trash_width, 1, fp);   // 쓰레기 값을 읽는다.
          }
     }
     // 이미지 데이터를 읽어들였으면 이제 파일을 닫습니다.
     fclose(fp);


}

#endif
