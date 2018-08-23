#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#define  INSERT_BUFF_SIZE   10


void Woo_sql(char*, char*);
void db_insert(char*,char*);

void db_insert(char * sig_name,char * sig_value){
    char * mongo_address="192.168.1.21";
    int                         hsocket;
    struct      sockaddr_in     server_addr;
  //  struct      hostent        *host_entry;
    char                       *p_enter= "\r\n";
    
    char                       *p_get_form= "GET /owhy/mongo_insert.jsp?name=%s&value=%s HTTP/1.1\r\nHost: %s:8080\r\n\r\n";
    char p_get[strlen(p_get_form)+strlen(sig_name)+strlen(sig_value)+strlen(mongo_address)];
    sprintf(p_get,p_get_form, sig_name,sig_value,mongo_address);
    char                        buff[INSERT_BUFF_SIZE+1];
   
    hsocket = socket( PF_INET, SOCK_STREAM, 0);
    if( -1 == hsocket)
    {
        printf( "socket 생성 실패\n");
        exit( 1);
    }
    memset( &server_addr, 0, sizeof( server_addr));
    server_addr.sin_family     = AF_INET;
    server_addr.sin_port       = htons(8080);
    inet_aton(mongo_address,&server_addr.sin_addr);
    if( -1 == connect( hsocket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
    {
       printf( "db connect fail!\n");
    }
    else
    {
        memset( buff, '\0', INSERT_BUFF_SIZE+1);
        send( hsocket, p_get, strlen( p_get)+1,0);
        if ( 0 == read( hsocket, buff, INSERT_BUFF_SIZE))
        {
           printf( "insert fail!\n");
        }
        else
        {	printf("insert success!\n");
            //printf( "%s\n", buff);
        }
        close( hsocket);
    }

}


void Woo_sql(char* _opCode, char* _caValue){
     char* query_Name;
    //Query Making
    if(_opCode[2]=='F'&&_opCode[3]=='F'){
        query_Name="FLAME";
    }
    else if(_opCode[2]=='S'&&_opCode[3]=='S'){
        query_Name="SOUND";
    }
    else if(_opCode[2]=='V'&&_opCode[3]=='V'){
        query_Name="VIBRATE";
    }
    else if(_opCode[2]=='G'&&_opCode[3]=='G'){
        query_Name="GAS";
    }
    else{
        printf("Message Error\n");
        return ;
    }

    db_insert(query_Name,_caValue);
}


