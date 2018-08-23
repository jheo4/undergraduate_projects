#pragma once
#include <pthread.h>
#include "Global.h"
#include "InSun.h"
#include "Woo.h"
#include "OH.h"
#include "TLCD.h"
#include "MATRIXLCD.h"
#include "Jin_Thread.h"
#include "bitmap2.c"

void Jin_TLCD(char);
void Jin_DOT();




void Jin_Initializing();
void Jin_Destroy();
void Jin_Internal();
void Jin_External();
void ClearBoard();
void Jin_Locking();

//////////////////////////////////// Thread Function...
void* Jin_Parsing(void *_arg);
void *Jin_SEVENThread(void *_arg);
void *Jin_BUZZERThread(void *_arg);
void *Jin_TOUCHCAMThread(void *_arg);
void *Jin_SendSignal(void *_arg);
void *network(void* ptr);
//////////////////////////////////////////////////////////////////////////////////////////
void Jin_Initializing() {
	// Arduino 연결... 
	fArduino = open(ARDUINO_DEV, O_RDWR | O_NOCTTY);
	fdTLCD = open(TLCD_DRIVER_NAME,O_RDWR);
	fdDOT = open(DRIVER_DOT,O_RDWR);
	cled_fd = open(YOO_CLED, O_RDWR);
	key_fd = open(YOO_KEY, O_RDWR);
	busled_fd = open(YOO_BUSLED, O_RDWR);
	fdDip = open(DRIVER_DIP,O_RDWR);
	fdSevnSeg = open(WOO_DRIVER,O_RDWR);
	oled_fd = open(DRIVER_OLED, O_RDWR);
	buzzer_fd = open(YOO_BUZZER, O_RDWR);
	if(fArduino<0 || fdTLCD<0 || fdDOT<0 || cled_fd<0 || key_fd<0 || busled_fd<0 || fdDip<0 || fdSevnSeg<0 )
	{
		perror("Diver Open Error.\n");
		exit(-1);
	}
	tcgetattr(fArduino, &oldtio); /* save current serial port settings */
	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	tcflush(fArduino, TCIFLUSH);
	tcsetattr(fArduino, TCSANOW, &newtio);
	ClearBoard();
	YOO_OLED();
	Jin_Locking();
}
void Jin_Destroy(){
	close(fdDOT);
	close(fdTLCD);
	close(fdSevnSeg);
	close(fdDip);
	close(busled_fd);
	close(cled_fd);
	close(key_fd);
	close(oled_fd);
	close(Buzzer_thd);
	tcsetattr(fArduino, TCSANOW, &oldtio);
	close(fArduino);
}

void Jin_External(){
	if(caOPcode[1] == 'E'){
		iBunkerState = CLOSE;
		Yoo_Busled(-1, caValue);
		Yoo_Bright(-1);
		displayDotLed_Jin(fdDOT, 0);
		strcpy(strWtext[0], "You underattack");
		if(caOPcode[2]=='F' && caOPcode[3]=='F'){	// Flame Sensor...
			strcpy(strWtext[1], "Flame Detect"); 
			iAttack = FLAME_INDEX;
			
		}
		else if(caOPcode[2]=='V' && caOPcode[3]=='V'){	// Vibration Sensor...
			strcpy(strWtext[1], "Vibr Detect");
			iAttack = VIBE_INDEX;
		}
		else if(caOPcode[2]=='S' && caOPcode[3]=='S'){ // Sound Sensor...
			strcpy(strWtext[1], "Sound Detect");
			iAttack = SOUND_INDEX;
		}
		else if(caOPcode[2]=='G' && caOPcode[3]=='G'){ // Gas Sensor...
			strcpy(strWtext[1], "Gas Detect");
			iAttack = GAS_INDEX;
		}
		YOO_OLED();
		pthread_create(&Sevenseg_thd, NULL, Jin_SEVENThread, NULL);
		pthread_create(&Buzzer_thd, NULL, Jin_BUZZERThread, NULL);
		Yoo_Busled(iAttack, caValue);
		Yoo_Bright(iAttack);
		Jin_TLCD('A');
		Jin_DOT();
		Woo_sql(caOPcode, caValue);

	} 
}
void Jin_Internal(){
	if(caOPcode[1] == 'I'){
		iBunkerState = OPEN;
		Yoo_Busled(-1, caValue);
		Yoo_Bright(-1);
		displayDotLed_Jin(fdDOT, 0);
		strcpy(strWtext[0], "=BUNKER CLOSED=");

		if(caOPcode[2]=='T' && caOPcode[3]=='T'){	// Temperature Sensor...
			char Temp[25]; strcpy(Temp, "Temp : ");
			// string...
			strcat(Temp, caValue); strcpy(strWtext[1], Temp);
			int i; 
			for(i = strlen(Temp) ; i < COLUMN_NUM ; i++) 
				strWtext[1][i] = '\0';
			pthread_create(&Sevenseg_thd, NULL, Jin_SEVENThread, NULL);
		}
		else if(caOPcode[2]=='P' && caOPcode[3]=='P'){	// PIR Sensor...
			strcpy(strWtext[1], "Danger detect");
			Woo_SevenSeg("000000");
			Jin_DOT();
		} 
		Jin_TLCD('N');
	}
} 

void* Jin_Parsing(void *_arg) {
	while (1) {
		iSizeOfString = read(fArduino, caBuf1, SIZE_OF_BUFF);	// 프레임 앞에 식별자...
		//write(fArduino, "1", 1);
		if(caBuf1){
			printf("%s\n", caBuf1);
			if(caBuf1[0] == '1')
				iBunkerState = OPEN;
			if(caBuf1[0] == '2')
				iBunkerState = CLOSE;
			caBuf1[iSizeOfString] = '\0';	// OEFF400\0
			/*		Parsing...		*/
			int i, j; i = j = 0;
			for(i = 0 ; i < 4 ; i ++) caOPcode[i] = caBuf1[i];
			for(i = 4 ; i < strlen(caBuf1); i++, j++) caValue[j] = caBuf1[i];
			printf("Signal: %s %s\n", caOPcode, caValue);
		} 
	}
}

void Jin_TLCD(char _cSort){
	functionSet_TLCD();
	clearScreen_TLCD(0); clearScreen_TLCD(1);
	clearScreen_TLCD(0); clearScreen_TLCD(1);
	if(_cSort == 'A'){			// Bunker state OPEN
		setDDRAMAddr_TLCD(0, 1); usleep(3000);
		writeStr_TLCD(strWtext[0]); 
		setDDRAMAddr_TLCD(0, 2); usleep(3000);
		writeStr_TLCD(strWtext[1]); 
		strcpy(strWtext[0], "==Team.4 O-WHY==");
		strcpy(strWtext[1], "BUNKER CLOSED");
	}else{
		setDDRAMAddr_TLCD(0, 1);
		writeStr_TLCD(strWtext[0]); usleep(3000);
		setDDRAMAddr_TLCD(0, 2); 
		writeStr_TLCD(strWtext[1]); usleep(3000);
	}
}

void Jin_DOT(){
	int durationTime ,Num ;
	int counter;
	durationTime = PRINT_DURATION;	//10
	Num = 55;
	changemode_Dot(1);
	// open  driver  
	displayDotLed_Jin(fdDOT , 1);
	changemode_Dot(0);
}

void Jin_Locking(){
	char N = 'N';
	strcpy(strWtext[0], "==Team.4 O-WHY==");
	strcpy(strWtext[1], "LOCKING...");
	Jin_TLCD(N);
	while(1){
		if(Woo_DipSW())
			if(Yoo_KEY()){
				strcpy(strWtext[0], "==Team.4 O-WHY==");
				strcpy(strWtext[1], "UNLOCKING...");
				Jin_TLCD(N);
				sleep(1);
				pthread_create(&ArduinoThread, NULL, Jin_Parsing, NULL);	// Arduino Thread...
				pthread_create(&TouchCam_thd, NULL, Jin_TOUCHCAMThread, NULL);
				write(fArduino, "3", 1);
				pthread_create(&Signal_thd, NULL, Jin_SendSignal, NULL);
				pthread_create(&Sock_thd, NULL, network, NULL);	
				return;
			}
	}
}

void *Jin_SEVENThread(void *_arg){
	Woo_SevenSeg(caValue);
}

void ClearBoard(){
	displayDotLed_Jin(fdDOT, 0);
	Jin_TLCD('N');
	Woo_SevenSeg("000000");
	Yoo_Busled(-1, caValue);
	Yoo_Bright(-1);
}
 
void *Jin_BUZZERThread(void *_arg){
	printf("Buzzer On\n");
	Yoo_BUZZER(iAttack);
}


void *Jin_TOUCHCAMThread(void *_arg){
	printf("Touch Cam On");
	Oh_TOUCHCAM();
}

void *Jin_SendSignal(void *_arg){
	while(1){
		if(iOpenSignal == 1){
			iOpenSignal = 0 ;
			write(fArduino, "1", 1);	// 1 : Open
			caOPcode[2] = 0 ;
			caOPcode[3] = 0 ; 
			YOO_OLED();
			printf("open send\n");
		}else if(iCloseSignal == 1){
			iCloseSignal = 0;
			write(fArduino, "2", 1);	// 2 : Close 	
			caOPcode[2] = 0 ;
			caOPcode[3] = 0 ; 
			YOO_OLED();
			printf("close send\n");
		}
	}
}
///////////////////////////////////////////////// Network
void *network(void* ptr) {
	int server_sockfd = 0;
	int client_sockfd = 0;
	int client_len = 0;
	int n = 0;
	int cBuf;

	struct sockaddr_in clientaddr, serveraddr;
	// making Socket
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) { 
		perror("socket error:");
		exit(0);
	}
	while(1){
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // Port & IP setting
	serveraddr.sin_port = htons(10000);  // Port & IP setting
	
		bind(server_sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 
		if(listen(server_sockfd, 5) == -1){
			perror("failed to set Redy mode\n");
			exit(0);
		}
		int sendret;
		while (1) {
			client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len); 
			printf("Accept!!\n");
			read(client_sockfd, cBuf, 1);
			printf("Android : %d\n", cBuf);
			if(iBunkerState == CLOSE && iOpenSignal==0){
				iBunkerState = OPEN;
				iOpenSignal = 1;
				printf("Andriod Open\n");
			}else if(iBunkerState == OPEN && iCloseSignal==0){
				iBunkerState = CLOSE;
				iCloseSignal = 1;
				printf("Andriod Close\n");
			}
			close(client_sockfd);
		} 
	}
}