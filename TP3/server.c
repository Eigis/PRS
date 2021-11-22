#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define RCVSIZE 1024
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"

// function to clear buffer
void clearBuf(char* b)
{
    int i;
    for (i = 0; i < RCVSIZE; i++)
        b[i] = '\0';
}

// function to encrypt
char Cipher(char ch)
{
    return ch ^ cipherKey;
}

// function sending file
int sendFile(FILE* fp, char* buf, int s)
{
    int i, len;
    if (fp == NULL) {
        fseek(fp, 0L, SEEK_END);
        len = ftell(fp);
        memcpy(buf, nofile,len);
        
        /*strcpy(buf, nofile);
        len = strlen(nofile);
        /*/
        buf[len] = EOF;
        for (i = 0; i <= len; i++)
            buf[i] = Cipher(buf[i]);
        return 1;
    }
  
    char ch, ch2;
    for (i = 0; i < s; i++) {
        ch = fgetc(fp);
        ch2 = Cipher(ch);
        buf[i] = ch2;
        if (ch == EOF)
            return 1;
    }
    return 0;
}


int main (int argc, char *argv[]) {

  struct sockaddr_in adresse, client,adresse2 ;     ////////////////////////////////
  //struct sockadddr_in adresse2, client2;      ////////////////////////////////
  int port= atoi(argv[1]);
  char *port2 = "3800";                       ////////////////////////////////
  int Port2 = atoi(port2);
  int valid= 1;                       ////////////////////////////////
  socklen_t alen= sizeof(client);
 
  char buffer[RCVSIZE];
  char buffer4[RCVSIZE];
  char buffer2[RCVSIZE];
  char *syncAck = "SYNCAck:3800";

  FILE* fp;
  int nBytes;

  //create socket
 // int server_desc = socket(AF_INET, SOCK_STREAM, 0);
  int server_desc = socket(AF_INET, SOCK_DGRAM, 0);
  int server_desc2 = socket(AF_INET, SOCK_DGRAM, 0);      ////////////////////////////////

  //handle error
  if (server_desc < 0) {
    perror("Cannot create socket\n");
    return -1;
  }

  if (server_desc2 < 0) {                 ////////////////////////////////
    perror("Cannot create socket\n");
    return -1;
  }

  setsockopt(server_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));
  setsockopt(server_desc2, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));     ////////////////////////////////

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port);
  adresse.sin_addr.s_addr= htonl(INADDR_ANY);

  adresse2.sin_family= AF_INET;              ////////////////////////////////
  adresse2.sin_port= htons(Port2);
  adresse2.sin_addr.s_addr= htonl(INADDR_ANY);

  //initialize socket
  if (bind(server_desc, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
    perror("Bind failed\n");
    close(server_desc);
    return -1;
  }

  //initialize socket                                 ////////////////////////////////
  if (bind(server_desc2, (struct sockaddr*) &adresse2, sizeof(adresse2)) == -1) {
    perror("Bind failed\n");
    close(server_desc2);
    return -1;
  }
  

 

  recvfrom(server_desc,(char *)buffer,RCVSIZE,MSG_WAITALL,( struct sockaddr *) &client,&alen);
  printf("%s\n",buffer);
  // shif se kjo spunoi .... ka tboj me dergimin e ACK
  if(strcmp("SYNC",buffer)==0){
     sendto(server_desc,(char *)syncAck,strlen(syncAck), MSG_CONFIRM,(const struct sockaddr *) &client, alen);
  }

/////////// On receive le ACK dans server_desc2
  int size=recvfrom(server_desc2,(char *)buffer4,RCVSIZE,MSG_WAITALL,( struct sockaddr *) &client,&alen);
  buffer4[size]='\0';
  printf("%s\n",buffer4);


  while (1) {
    // me marr ca don .... punon
    /*int k =recvfrom(server_desc2,(char *)buffer,RCVSIZE,MSG_WAITALL,( struct sockaddr *) &client,&alen);
    buffer[k]='\n';
    printf("%s",buffer);*/
    //sendto(server_desc2,(char *)buffer,RCVSIZE, MSG_CONFIRM,(const struct sockaddr *) &client, alen);
    
    printf("\nWaiting for file name...\n");
  
        // receive file name
        clearBuf(buffer);
  
        nBytes = recvfrom(server_desc2, buffer,
                          RCVSIZE, sendrecvflag,
                          (struct sockaddr*)&client, &alen);
  
        fp = fopen(buffer, "r");
        printf("\nFile Name Received: %s\n", buffer);
        if (fp == NULL)
            printf("\nFile open failed!\n");
        else
            printf("\nFile Successfully opened!\n");
  
        while (1) {
            int n =fread(buffer,1,nBytes,fp);
            // process
            if (sendFile(fp, buffer, n)) {
                sendto(server_desc2, buffer, n,
                       sendrecvflag,
                    (struct sockaddr*)&client, alen);
                break;
            }
  
            // send
            sendto(server_desc2, buffer, RCVSIZE,
                   sendrecvflag,
                (struct sockaddr*)&client, alen);
            clearBuf(buffer);
        }
        if (fp != NULL)
            fclose(fp);
    }
    return 0;




  
  
 // }/// e shtova pr whilen e udps
}