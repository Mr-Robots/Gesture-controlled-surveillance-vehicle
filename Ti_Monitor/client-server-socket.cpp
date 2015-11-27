
//Server.c

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void exit_pkill(void)
{
    system("pkill gesture_client");
    system("pkill python");
}

int main(void)
{
    if(atexit(exit_pkill) != 0)
        printf("Serial Socket Server : Error with Exit Handler!\n");

    int listenfd = 0,connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Serial Socket Server : socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(7000);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    printf("Serial Socket Server : socket Bind success\n");

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

	char a=1;
	FILE *f_socket;
	f_socket = popen("python Threading.py","r");
    printf("Serial Socket Server : Pipe with Python success!\n");

    connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);// accept awaiting request
    printf("Serial Socket Server : socket Accept connection success\n");

    FILE *g_f;
    g_f = popen("./gesture_client","w");
    printf("Serial Socket Server : Gesture Client started as a child process\n");

    if(connfd < 0)
    {
        printf("Serial Socket Server : Error Listening!");
        exit(0);
    }

    fread(sendBuff,sizeof(char),1,f_socket);
    while(1)
    {
        if(sendBuff[0] == 0xFF)
            break;
        fread(sendBuff,sizeof(char),1,f_socket);
        printf("C  Sync : %d\n",sendBuff[0]);
    }
    while(1)
    {
        fread(sendBuff,sizeof(char),15,f_socket);
//        for(int i=0;i<15;++i)
//            printf("%d ",sendBuff[i]);
//        printf("\n");
        sendBuff[15] = '\0';
        write(connfd, sendBuff, 15);
        usleep(60000);
    }
    system("pkill gesture_client");
    close(connfd);
    exit(0);

}


/*
//Client.c

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(void)
{
    int sockfd = 0,n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0' ,sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr("172.16.2.70");

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
            printf("\n Error : Fputs error");
        printf("\n");
    }

    if( n < 0)
        printf("\n Read Error \n");
    return 0;
}
*/
