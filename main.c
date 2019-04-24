#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <newt.h>

//GLOBAL DECLARATION
struct queue *pque;
newtComponent tbox;
char search_result[500];

#include "login.h"
#include "pqueue.h"
#include "recp_loop.h"
#include "recp_client_listen.h"
#include "doc_loop.h"
#include "doc_server_listen.h"
#include "buffer.h"

void newt_finish()
{
    newtPopWindow();
    newtFinished();
    exit(0);
}

int main()
{
    newtInit();
	newtCls();
    
    pque = create_queue(50);
    pthread_t doc_listen, recp_listen, d_loop, r_loop;
	
    unsigned int rows, cols;
    newtGetScreenSize(&cols, &rows);
    
    if(cols < 108 || rows < 31)
    {   
        newtFinished();        
        printf("Error: Terminal Size is too small\n");
        printf("Minimum size required:  108 x 31\n");             
        exit(0);
    }

    char *ename, *epass;
	tbox = newtTextbox( cols- 30, 2, 20, 10, 0 );
    unsigned int winx=cols/2 - 22, winy=rows/2 - 6, role = 0;
    newtComponent lname, lpass, form, ent_name, ent_pass, lin, can, catch;

    newtOpenWindow(winx, winy, 40, 10, "log-in");     

    lname = newtLabel(4, 1, "Username:"); 
    lpass = newtLabel(4, 3, "Password:");
    ent_name = newtEntry(15, 1, NULL, 20, (const char**) &ename, 0);
    ent_pass = newtEntry(15, 3, NULL, 20, (const char**) &epass, NEWT_FLAG_PASSWORD);
    lin = newtButton(9,6, "Log IN");
    can = newtButton(22,6, "Cancel");
    
    form = newtForm(NULL, NULL, 0);        
    newtFormAddComponents(form, lname, lpass, ent_name, ent_pass, lin, can, NULL);
    
    while(1)
    {   
        newtEntrySet(ent_name,"",0);
        newtEntrySet(ent_pass,"",0);
        catch = newtRunForm(form);
           
        if( catch == lin )           
            if( ( role = login(ename,epass) ) != 0 )  
            {
                newtPushHelpLine(" ");
                break;             
            }           
            else
                newtPushHelpLine("\tInvalid login, please try again");            
        else if( catch == can )        
            newt_finish();        
        else     
            newt_finish();        
	} 

    switch(role)
    {
        case 1:
			//doc_loop();
		
            pthread_create(&d_loop, NULL, doc_loop, 0);
			pthread_create(&doc_listen, NULL, doc_server_listen, NULL);
            
            pthread_join(d_loop, NULL);
            //pthread_join(doc_listen, NULL);           
            break;
        case 2:
			//recp_loop();
	
			pthread_create(&r_loop, NULL, recp_loop, 0);    
   			pthread_create(&recp_listen, NULL, recp_client_listen, NULL);
            
			pthread_join(r_loop, NULL);
            //pthread_join(recp_listen, NULL);            
            break;
        default:
            printf("Unknown error occured");
            break;
    }
    //newtWaitForKey();
    newt_finish();
    return 0;
}

