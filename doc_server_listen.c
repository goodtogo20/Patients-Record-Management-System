#include <stdio.h>
#include <newt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "pqueue.h"
#include "search.h"
#include "send_sig_to_recp.h"

#define DEQUEUE 1
#define SER_RES 2

extern struct queue *pque;
extern char search_result[200];

void *doc_server_listen()
{
	int i, id;
    FILE *fp = fopen("dlisten.txt","w");
	char buffer[1024], instr[10], temp[100],str_id[100], ser_res[200];
	int udpSocket, nBytes, run_sbi=0, run_sbn = 0;

	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size, client_addr_size;

	/*Create UDP socket*/
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

	/*Configure settings in address struct*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7900);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	/*Bind socket with address struct*/
	bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	/*Initialize size variable to be used later on*/
	addr_size = sizeof serverStorage;

	while(1)
	{
		/* Try to receive any incoming UDP datagram. Address and port of 
		  requesting client will be stored on serverStorage variable */
		nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);
		
		sscanf(buffer,"%s %d",instr,&id);
        sscanf(buffer,"%s %s",temp, str_id);
        fprintf(fp,"%s \n",buffer);

		if(!strcmp(instr,"enq"))
		{
			enqueue(pque, id);
            fill_pque_tbox();
            newtRefresh();
			strcpy(instr,"nul");

		}
        //conditional execution codeblocks
        else if(run_sbi)
        {
            strcpy(search_result, buffer);
            fprintf(fp,"recvd: %s",search_result);
            run_sbi = 0;

        }
        else if(run_sbn)
        {
            strcpy(search_result, buffer);
            run_sbn = 0;
        } 
        //manipulate conditional execution        
        else if( !strcmp(instr,"sbi") )
        {
            run_sbi = 1;  
            fprintf(fp,"cli_res: %s",buffer);          
        }else if( !strcmp(instr,"sbn"))
        {
            run_sbn = 1;
        }
	}

  fclose(fp);
  return 0;
}
