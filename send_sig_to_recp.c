#include <stdio.h>
#include <newt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "pqueue.h"
#include "search.h"

#define DEQUEUE 1
#define SEARCH_BI 2
#define SEARCH_BN 3
#define GET_TB 4

extern struct queue *pque;
extern newtComponent tbox;

int send_sig_to_recp(int reason, char *data)
{
  static int clientSocket, portNum, nBytes, first_run = 1;  
  static struct sockaddr_in serverAddr;
  static socklen_t addr_size;
  FILE *fp =fopen("dsend.txt","w");
  char buffer[1024], instr[20];
    
  if(first_run)
  {
      /*Create UDP socket*/
      clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

      /*Configure settings in address struct*/
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(7950);
      serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
      memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

      /*Initialize size variable to be used later on*/
      addr_size = sizeof serverAddr; 
      first_run = 0;
  }    
  
    switch(reason)
    {
        case DEQUEUE:
            dequeue(pque);
            strcpy(buffer,"deq");

            newtTextboxSetText(tbox,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");      
            fill_pque_tbox();
            newtRefresh();

            sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
            break;

        case SEARCH_BI:
            sprintf(instr,"ser %s",data);
	        strcpy(buffer,instr);
            fprintf(fp, "%s %s\n",buffer,instr);

	        sendto(clientSocket,buffer,strlen(instr),0,(struct sockaddr *)&serverAddr, addr_size); 
	        break;        

        case SEARCH_BN:
            sprintf(instr,"sbn %s",data);
            strcpy(buffer,instr);
    
            sendto(clientSocket,buffer,strlen(instr),0,(struct sockaddr *)&serverAddr, addr_size); 
	        break;
        
        case GET_TB:
            sprintf(instr,"gtb");
            strcpy(buffer,instr);
    
            sendto(clientSocket,buffer,strlen(instr),0,(struct sockaddr *)&serverAddr, addr_size); 
	        break;
        default:

            newtPushHelpLine("Fatal Error: doc_server_send");
            newtWaitForKey();
            exit(0);
            break;
    }

    
  fclose(fp);
  return 0;
}
