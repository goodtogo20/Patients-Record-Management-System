#include <stdio.h>
#include <newt.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> 
#include <string.h>
#include "buffer.h"
#include "pqueue.h"
#include "send_sig_to_recp.h"

extern struct queue *pque;
extern newtComponent tbox;
char search_result[200];

unsigned int str_to_uint(char *st) {
  char *x;
  for (x = st ; *x ; x++) {
    if (!isdigit(*x))
      return 0L;
  }
  return strtoul(st, 0L, 10);
}

char* search_by_id( char search_id[] )
{
	FILE *fp;
    int cha;
    unsigned int id, query_id;
	char fname[200],lname[200], *temp, *ret_str, ch;
	
    query_id = str_to_uint(search_id);

    fp=fopen("pat_data.txt","r");
	ret_str = (char*)malloc(sizeof(char)*200);
    
    while( fscanf(fp,"%u %s %s",&id,fname,lname) != EOF )    
    {   
        if( query_id == id ) 
        {
            if( isdigit(lname[0]) ) 
            {
                sprintf(ret_str,"%u %s", id, fname);
                return ret_str;
            }       
            else 
            {
                sprintf(ret_str,"%u %s %s", id, fname, lname);
                return ret_str;
            }   
        }     
        printf("%u %s %s", id, fname, lname);
        
        //MOVE POINTER TO NEXT LINE
        while( fgetc(fp) != '\n')
            continue;
            
    }        
	return 0;
}


int search_by_name_recp(char* qname, char data[][200])
{   
    FILE *fp, *test;
    int i = 0, id;
    char instr[50], temp[100], fname[200], lname[200];

    sprintf(instr,"grep -i %s pat_data.txt > res_grep_recp.txt",qname);
    system(instr);
	test = fopen("ztest","w");
    fp=fopen("res_grep_recp.txt", "r");
    
    while( fscanf(fp,"%u %s %s",&id,fname,lname) != EOF )    
    {        
        if( isdigit(lname[0]) ) 
        {   
            sprintf(temp, "%u ", id);
            strcpy(data[i],temp);
            strcat(data[i],fname);
			i++;       
        }       
        else 
        {
            sprintf(temp, "%u ", id);
            strcpy(data[i],temp);
            strcat(data[i],fname);
            strcat(data[i]," ");
            strcat(data[i],lname);
			i++;
        }             
        
        //MOVE POINTER TO NEXT LINE
        while( fgetc(fp) != '\n')
            continue;           
    }
	for(int jk = 0 ; jk < i; jk++)	
		fputs(data[jk],test);

	fclose(test);
	fclose(fp);
	return i;
}

int search_by_name_doc(char* qname, char data[][200])
{   
    FILE *fp, *test;
    int i = 0, id;
    char instr[50], temp[100], fname[200], lname[200];

	test = fopen("ztest","w");
    fp=fopen("res_grep_doc.txt", "r");
    
    while( fscanf(fp,"%u %s %s",&id,fname,lname) != EOF )    
    {        
        if( isdigit(lname[0]) ) 
        {   
            sprintf(temp, "%u ", id);
            strcpy(data[i],temp);
            strcat(data[i],fname);
			i++;       
        }       
        else 
        {
            sprintf(temp, "%u ", id);
            strcpy(data[i],temp);
            strcat(data[i],fname);
            strcat(data[i]," ");
            strcat(data[i],lname);
			i++;
        }             
        
        //MOVE POINTER TO NEXT LINE
        while( fgetc(fp) != '\n')
            continue;           
    }
	for(int jk = 0 ; jk < i; jk++)	
		fputs(data[jk],test);

	fclose(test);
	fclose(fp);
	return i;
}


//search by id over network
char* search_by_id_req(char* id)
{	
	send_sig_to_recp(2, id);
	
	newtPushHelpLine("  Searching ...");
	sleep(3);
	newtPopHelpLine();

	return search_result;
}

void fill_doc_name_search_file(char* name)
{
    send_sig_to_recp(3,name);
    newtPushHelpLine("  Searching ...");
    sleep(3);
    newtPopHelpLine();

    FILE* fp = fopen("pat_data_inter.txt","w");
    char ch;
    int i = 0; 

    while(ch = fgetc(fp) !=  '\n')
    {
        fputc(search_result[i],fp);
        i++;
    }
    fputc('\0',fp);

    fclose(fp);
    return;
}

char* get_search_by_name_data(char* name)
{     
    int i = 0;  
    buffer_t *ret = buffer_new();
    char instr[50], temp[1000], ch;
    
    sprintf(instr,"grep -i %s pat_data.txt > temp",name);
    system(instr);

    FILE* fp = fopen("temp","r");

    while( ch = fgetc(fp) != EOF)
    {
        temp[i] = ch;
    }      
    temp[i] = '\0';
    
    buffer_append(ret,temp);
    fclose(fp);

    return ret->data;
}

void fill_pque_tbox()
{   
    buffer_t *buf = buffer_new();  
    char pass_id[10];
	int i;	

    buffer_clear(buf);    
	
	if(isEmpty(pque))
	{
		newtTextboxSetText( tbox,"Queue is Empty!" );
	}

    for(i=pque->head ; (i < pque->size+pque->head) ; i++) 
    {   
        sprintf(pass_id,"%d",pque->array[i]);

        buffer_append(buf,search_by_id(pass_id));
        buffer_append(buf,"\n");
    }

    if( i > pque->tail )
    {
        for(int i = 0 ; i < pque-> tail ; i++)    
        {
            sprintf(pass_id,"%d",pque->array[i]);

		    buffer_append( buf, search_by_id(pass_id) );
		    buffer_append( buf, "\n");
        }
    }    

    newtTextboxSetText(tbox,buf->data);
}

void change_password(int role, char* old_p, char* new_p, char *new_pa)
{
	FILE *fp = fopen("pass.txt","r");
    char dname[50], dpass[50], drole[50];
    char rname[50], rpass[50], rrole[50];
	fscanf(fp,"%s %s %s",dname,dpass,drole);
    fscanf(fp,"%s %s %s",rname,rpass,rrole);
    fclose(fp);   

    if(strcmp(new_p,new_pa))
    {
        return;
    }
    if(role == 1)
    {
        if(strcmp(old_p,dpass))
            return;
    }
    if(role == 2)
    {
        if(strcmp(old_p,rpass))
            return;
    }

    fp = fopen("pass.txt","w");
    if(role == 1)
    {
        fprintf(fp,"%s %s %s\n",dname,new_p,drole);
        fprintf(fp,"%s %s %s\n",rname,rpass,rrole);
        fprintf(fp,"end");
    }
    if(role == 2)
    {
        fprintf(fp,"%s %s %s\n",dname,dpass,drole);
        fprintf(fp,"%s %s %s\n",rname,new_p,rrole);
        fprintf(fp,"end");  
    }
    	
    fclose(fp);
}

