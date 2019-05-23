#include <stdio.h>
#include <newt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "pqueue.h"
#include "search.h"

#define ENQUEUE 1
#define SEARCH_RES 2
#define SEND_SBN_RES 3
#define SEND_TB 4
#define SEND_DONE 5

extern struct queue *pque;

int send_sig_to_doc(int reason, char* data)
{
  static int clientSocket, portNum, nBytes, first_run = 1, enq_id;  
  static struct sockaddr_in serverAddr;
  static socklen_t addr_size;
  FILE *fp =fopen("rsend.txt","w");
  char buffer[1024], instr[20];
    
  if(first_run)
  {
      /*Create UDP socket*/
      clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

      /*Configure settings in address struct*/
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(7900);
      serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
      memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

      /*Initialize size variable to be used later on*/
      addr_size = sizeof serverAddr; 
      first_run = 0;
  }    
  
  switch(reason)
  {
    case ENQUEUE:
      sscanf(data,"%d",&enq_id);
      enqueue(pque,enq_id);  
	  fill_pque_tbox();
		
	  sprintf(instr,"enq %s",data);
	  strcpy(buffer,instr);
	  fprintf(fp,"%s %s\n",buffer,instr);

	  sendto(clientSocket,buffer,strlen(instr),0,(struct sockaddr *)&serverAddr, addr_size); 
	  break;

	case SEARCH_RES:
        strcpy(buffer,"sbi");
            fprintf(fp,"res_buffer:%s\n",buffer);
        sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
        
            strcpy(buffer,data);
            fprintf(fp,"data: %s",buffer);
        sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
        break;
    
    case SEND_SBN_RES:
        strcpy(buffer,"sbn");
            fprintf(fp,"sbname:%s\n",buffer);
        sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
        strcpy(buffer,data);
            fprintf(fp,"data: %s",buffer);
        sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
        break;

    case SEND_TB:
        strcpy(buffer,"rtb");
        sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
        strcpy(buffer,data);
            fprintf(fp,"data: %s",buffer);
        sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
        break;
        
    case SEND_DONE: 
        strcpy(buffer,"dne");
        sendto(clientSocket,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr, addr_size);
        break;

    default: 
		newtPushHelpLine("Fatal Error: recp_client_send");
		newtWaitForKey();
		exit(0);
		break;
  }
  
    
  fclose(fp);
  return 0;
}
