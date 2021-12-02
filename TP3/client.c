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
//#define cipherKey 'S'
#define sendrecvflag 0

// function to clear buffer
/*void clearBuf(char* b)
{
    int i;
    for (i = 0; i < RCVSIZE; i++)
        b[i] = '\0';
}

// function for decryption
char Cipher(char ch)
{
    return ch ^ cipherKey;
}

// function to receive file
int recvFile(char* buf, int s)
{
    int i;
    char ch;
    for (i = 0; i < s; i++) {
        ch = buf[i];
        ch = Cipher(ch);
        if (ch == EOF)
            return 1;
        else
            printf("%c", ch);
           
    }
    return 0;
}

    */
int main (int argc, char *argv[]) {

  struct sockaddr_in adresse , adresse2;  ////////////////////////////////
  //struct sockadddr_in adresse2; ////////////////////////////////
  int port = atoi(argv[2]);
  //int port2;                    ////////////////////////////////
  int serverAddr = atoi(argv[1]);
  int valid = 1;
  char msg[RCVSIZE];
  char blanmsg[RCVSIZE];
  char port2[RCVSIZE];
  char *sync = "SYNC";
  char *ack = "ACK";
  socklen_t alen= sizeof(adresse);
  socklen_t alen2= sizeof(adresse2);    ////////////////////////////////

  FILE* fp;
  char *FileName = "copy.png";
  int nBytes;


  //create socket
  //int server_desc = socket(AF_INET, SOCK_STREAM, 0);
  int server_desc = socket(AF_INET, SOCK_DGRAM, 0);
  int server_desc2 = socket(AF_INET, SOCK_DGRAM, 0);    ////////////////////////////////
  

  // handle error
  if (server_desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  if (server_desc2 < 0) {                     ////////////////////////////////
    perror("cannot create socket\n");
    return -1;
  }


  setsockopt(server_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));
  setsockopt(server_desc2, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));       ////////////////////////////////

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port);
  adresse.sin_addr.s_addr= htonl(serverAddr);

  


  // connect
  //int rc = connect(server_desc, (struct sockaddr*)&adresse, sizeof(adresse));
  //printf("value of connect is: %d\n", rc);

  // handle error
  //if (rc < 0) {
  //  perror("connect failed\n");
  //  return -1;
 // }

/*   perrrr tcp
  int cont= 1;
  while (cont) {
    fgets(msg, RCVSIZE, stdin);
    int sss= write(server_desc,msg,strlen(msg));
    printf("the value of sent is:%d\n", sss);
    read(server_desc, blanmsg, RCVSIZE);
    printf("%s",blanmsg);
    memset(blanmsg,0,RCVSIZE);
    if (strcmp(msg,"stop\n") == 0) {
      cont= 0;
    }
  }*/


  int sss=sendto(server_desc, (const char *)sync, sizeof(sync), 0, (const struct sockaddr *) &adresse, sizeof(adresse));
  //printf("the value of sent is:%d\n", sss);
  int size = recvfrom(server_desc, (char *)blanmsg, RCVSIZE,MSG_WAITALL, (struct sockaddr *) &adresse,&alen);
  blanmsg[size]='\0';
  printf("%s\n",blanmsg);
  /*if(strcmp("SYNCAck",blanmsg)==0){
     sendto(server_desc,(const char *)ack,strlen(ack), MSG_CONFIRM,(const struct sockaddr *) &adresse, alen);            

  }*/
////////////////////////per tu heq mdk po n fillim duhet me obtenu Port2
  /*int size2 =recvfrom(server_desc2, (char *)port2, RCVSIZE,MSG_WAITALL, (struct sockaddr *) &adresse,&alen);               ////////////////////////////////
  blanmsg[size2]='\0';
  printf("%s\n",port2);
  int Port2 = atoi(port2);*/
 
 // Obtenir le numero de port
  char *Cport2;
  char *strToken = strtok(blanmsg , ":" );
  while ( strToken != NULL ) {
        /*printf ( "%s\n", strToken );
        // On demande le token suivant.*/
        Cport2 = strToken;
        strToken = strtok ( NULL, ":" );
    }
int Port2 = atoi(Cport2);
printf("%d",Port2);


  adresse2.sin_family= AF_INET;              ////////////////////////////////
  adresse2.sin_port= htons(Port2);
  adresse2.sin_addr.s_addr= htonl(INADDR_ANY);

// Envoyer un ACK au server_desc2  puisqu'on a obtenu son numero de port
int ss=sendto(server_desc2, (const char *)ack, sizeof(ack), 0, (const struct sockaddr *) &adresse2, sizeof(adresse2));

  
  int cont= 1;
  while (cont) {
    // me cu ca don ..... punon
    /*fgets(msg, RCVSIZE, stdin);    
    int sss=sendto(server_desc2, (const char *)msg, RCVSIZE, MSG_CONFIRM, (const struct sockaddr *) &adresse2, sizeof(adresse2));
    
    //printf("the value of sent is:%d\n", sss);
    //recvfrom(server_desc2, (char *)blanmsg, RCVSIZE,MSG_WAITALL, (struct sockaddr *) &adresse2,&alen2);
    //printf("%s\n",blanmsg);
   // cont=0;

  }

close(server_desc2);
return 0;*/

  printf("\nPlease enter file name to receive:\n");
        scanf("%s", msg);
        sendto(server_desc2, msg, RCVSIZE,
               sendrecvflag, (struct sockaddr*)&adresse2,
               alen2);
  
        printf("\n---------Data Received---------\n");
  fp=fopen(FileName,"wb");
        while (1) {
            // receive
          //  clearBuf(msg);
            nBytes = recvfrom(server_desc2, msg, RCVSIZE,
                              sendrecvflag, (struct sockaddr*)&adresse2,
                              &alen2);
  
            // process
            
          
           /* msg[nBytes]='\0';
            printf("%s\n",msg);*/
            if(fwrite(msg,1,nBytes,fp) < 0 ){
              perror("ERRORR");
            }

            if(nBytes<RCVSIZE){  
              break;
            }
            /*if (recvFile(msg, RCVSIZE)) {
                break;
            }*/
        }
        fclose(fp);
        printf("\n-------------------------------\n");
    }
    return 0;


}