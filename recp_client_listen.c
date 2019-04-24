#include <stdio.h>
#include <newt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "pqueue.h"
#include "search.h"
#include "send_sig_to_doc.h"

extern struct queue *pque;
extern newtComponent tbox;
extern char search_result[200];

void *recp_client_listen()
{
	int i; FILE *fp = fopen("rlisten.txt","w");
	char buffer[1024], instr[10], ser_res[500], str_id[50], temp[100];
	int udpSocket, nBytes, run = 0,jk=0;

	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size, client_addr_size;

	/*Create UDP socket*/
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

	/*Configure settings in address struct*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7950);
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
		
        
        sscanf(buffer,"%s %s",instr, str_id);
        
		if( !strcmp(buffer,"deq") )
		{
			dequeue(pque);
                     
            newtTextboxSetText(tbox,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");      
            fill_pque_tbox();
            newtRefresh();

            strcpy(buffer,"nul");

		}else if( !strcmp(instr,"ser") )
        {
             strcpy(ser_res,search_by_id(str_id));
             fprintf(fp,"%s %s",ser_res,str_id);
             //ser_res->2
             send_sig_to_doc(2, ser_res);
        }else if( !strcmp(instr,"sbn"))
        {
            strcpy(ser_res, get_search_by_name_data(str_id));
            send_sig_to_doc(3,ser_res);
        }
        jk++;

        if(jk==2)
            fclose(fp);
	}

  fclose(fp);
  return 0;
}

