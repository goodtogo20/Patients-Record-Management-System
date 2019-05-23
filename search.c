#include <stdio.h>
#include <newt.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> 
#include <string.h>
#include "buffer.h"
#include "pqueue.h"
#include "send_sig_to_recp.h"

#define MALE 1
#define FEMALE 2 

extern struct queue *pque;
extern newtComponent tbox;
extern buffer_t *  pinfo;

extern char search_result[200];
extern char search_name_result[500];

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

    /*newtPushHelpLine(search_id);
    newtWaitForKey();*/

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
  
	return (char*)0;
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

    if(ftell(fp) == 0)
    {   
        fclose(test);
	    fclose(fp);
        return -1;
    }else
    {   
        fclose(test);
	    fclose(fp);
        return i;
    }

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
	sleep(2);
	newtPopHelpLine();

	return search_result;
}

void fill_doc_name_search_file(char* name)
{
    send_sig_to_recp(3,name);
    newtPushHelpLine("  Searching ...");
    sleep(3);
    newtPopHelpLine();

    FILE* fp = fopen("res_grep_doc.txt","w");

    fputs(search_name_result,fp);
    /*write search_result to file
    while(ch = fgetc(fp) !=  '\n')
    {
        fputc(search_result[i],fp);
        i++;
    }
    fputc('\0',fp);*/

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
    
    FILE *f = fopen("temp", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = '\0';
    /*while( ch = fgetc(fp) != EOF)
    {
        temp[i] = ch;
    }      
    temp[i] = '\0';
    
    buffer_append(ret,temp);*/
    FILE* fp = fopen("gnd_test.txt","w");
    fprintf(fp,"%s",string);
    fclose(fp);

    return string;
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
        return;
	}

    for(i=pque->head ; (i < pque->size + pque->head) ; i++) 
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

void fill_pque_tbox_over_network()
{
    send_sig_to_recp(4,"gtb");
}

char *fetch_tb_data()
{
    buffer_t *buf = buffer_new();  
    char pass_id[10];
	int i;	

    buffer_clear(buf);    
	
	if(isEmpty(pque))
	{
		buffer_append(buf,"Queue is Empty!");
        return buf->data;
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
    
    return buf->data;
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

void set_pinfo(char *search_id)
{   
    FILE *fp = fopen("pat_data.txt","r");
    char fname[200],lname[200], temp[200];
    int query_id = str_to_uint(search_id);      
    int id;

    buffer_clear(pinfo);

    while( fscanf(fp,"%u %s %s",&id,fname,lname) != EOF )    
    {   
        if( query_id == id ) 
        {
            if( isdigit(lname[0]) ) 
            {
                buffer_append(pinfo,"\nName: ");
                buffer_append(pinfo,fname); 
                buffer_append(pinfo,"\n");               
            }       
            else 
            {
                buffer_append(pinfo,"Name: ");
                buffer_append(pinfo,fname); 
                buffer_append(pinfo," ");
                buffer_append(pinfo,lname);
                buffer_append(pinfo,"\n\n");
            }  

            buffer_append(pinfo,"Age: ");
            fscanf(fp,"%s",temp);
            buffer_append(pinfo,temp);
            buffer_append(pinfo,"            ");
            
            buffer_append(pinfo,"DOB: ");
            fscanf(fp,"%s",temp);
            buffer_append(pinfo,temp);
            buffer_append(pinfo,"/");
            fscanf(fp,"%s",temp);
            buffer_append(pinfo,temp);
            buffer_append(pinfo,"/");
            fscanf(fp,"%s",temp);
            buffer_append(pinfo,temp);
            buffer_append(pinfo,"\n\n");
            
            fscanf(fp,"%d",&id);
            buffer_append(pinfo,"Gender: ");
            if(id == MALE)
            {
                buffer_append(pinfo,"Male");  
                buffer_append(pinfo,"       ");
            } else
            {
                buffer_append(pinfo,"Female");
                buffer_append(pinfo,"     ");
            }
            
            
            buffer_append(pinfo,"Contact No: ");
            fscanf(fp,"%s",temp);
            buffer_append(pinfo,temp);  
            buffer_append(pinfo,"\n\n");

            buffer_append(pinfo,"Address: ");
            fgets(temp,200,fp);
            buffer_append(pinfo,temp);
            buffer_append(pinfo,"\n");

            break;
        
        }  

        //MOVE POINTER TO NEXT LINE
        while( fgetc(fp) != '\n')
            continue;      
               
    }

    fclose(fp);      
}
