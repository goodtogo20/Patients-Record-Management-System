#include <stdio.h>
#include <stdlib.h>
#include <newt.h>
#include <time.h>
#include "buffer.h"

#define ENTRY 0
#define MOR_ONE 1
#define MOR_HALF 2
#define NOON_ONE 3
#define NOON_HALF 4
#define NIGHT_ONE 5
#define NIGHT_HALF 6
#define LINE_ONE 7
#define LINE_TWO 8

void wr_report_file(char *data[][200],newtComponent tab[][9], unsigned id, int upto)
{
    char file_id[26];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(file_id,"pat_med_reports/%u.txt",id);

    printf("%s",file_id);

    FILE *fp=fopen(file_id,"a");
    
    for(int i=0; i < upto; i++)
    {   
        if(i<3)        
        {
            fprintf(fp,"%s |",*data[i]);      
        } else if(i == 3)
        {
            fprintf(fp,"%s \n",*data[i]);
			fprintf(fp,"%d-%d-%d %d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
        } else
        {
            fprintf(fp,"%s |",*data[i]);

			if(newtRadioGetCurrent(tab[i-4][MOR_ONE]) == tab[i-4][MOR_ONE])
				fprintf(fp,"%s","1");
			else if(newtRadioGetCurrent(tab[i-4][MOR_ONE]) == tab[i-4][MOR_HALF])
				fprintf(fp,"%s","2");	
			else 
				fprintf(fp,"%s","0");

			if(newtRadioGetCurrent(tab[i-4][NOON_ONE]) == tab[i-4][NOON_ONE])
				fprintf(fp,"%s","1");
			else if(newtRadioGetCurrent(tab[i-4][NOON_ONE]) == tab[i-4][NOON_HALF])
				fprintf(fp,"%s","2");	
			else 
				fprintf(fp,"%s","0");

			if(newtRadioGetCurrent(tab[i-4][NIGHT_ONE]) == tab[i-4][NIGHT_ONE])
				fprintf(fp,"%s\n","1");
			else if(newtRadioGetCurrent(tab[i-4][NIGHT_ONE]) == tab[i-4][NIGHT_HALF])
				fprintf(fp,"%s\n","2");	
			else 
				fprintf(fp,"%s\n","0");
        } 
    }
    
    fprintf(fp,"^");    

    fclose(fp);
}

int fill_report(char data[][200], unsigned id)
{   
    int i=0, j=0, special_flag = 0;
    static int cursor = 0;
    char file_id[26], ch;
    sprintf(file_id,"pat_med_reports/%u.txt",id);
     
    //intialise data array to 0 for saftey   
    for(int row = 0 ; row < 11; row++)
        for(int col = 0 ; col < 200 ; col++)
            data[row][col] = 0;

    FILE *fp = fopen(file_id, "r"); 
    FILE *test = fopen("test.txt","a");

    if(cursor != 0) 
        fseek(fp,cursor,0);     

    if(fp == 0)
    {   
        newtPushHelpLine("Error Occured, press any key!");
        newtWaitForKey();
        fseek(fp,cursor,0);
    }

    while( (ch = fgetc(fp)) != '^'  )
    {   
        if ( feof(fp) ) 
            return i;         
        
        fprintf(test,"go: %c\n",ch);  

        if(ch == '|' || ch == '\n')
        {
            ch = '\0';
            special_flag = 1;
            i++;
        }	
		
		//decision
        if( special_flag )
        {
            data[i][j] = ch;
            special_flag = 0;
            j=0;   
        } else         
            data[i][j++] = ch;      
    }       
    
    fprintf(test,"______________________--------->index : %d \n",i);
    for(int jk=0; jk < i; jk++)
    {
        fprintf(test,"_______FIELDS[%d]: %s \n",i,data[jk]);
    }

    cursor = ftell(fp);
    fclose(test);    
    fclose(fp);

    return i;
}

char* get_line(int len, char type)
{
	char* line = (char*)malloc(sizeof(char)*len);
	for(int i=0; i < len-1; i++)
		line[i] = type;

	line[len-1] = '\0';
	return line;
}

newtComponent get_report(unsigned id)
{   
    int i = 0;    
    char data[19][200], go[10];
	buffer_t *buf = buffer_new();

    newtComponent tb = newtTextbox(5, 1, 60, 20, NEWT_FLAG_SCROLL);
    
    while( (i = fill_report(data,id)) > 0 ) 
    {   
		buffer_append(buf, get_line(20,'_'));
		buffer_append(buf, data[4]);
		buffer_append(buf, get_line(20,'_'));
		buffer_append(buf, "\n\n");

        buffer_append(buf, "Complaints:\n");   
        buffer_append(buf,data[0]);
        buffer_append(buf,data[1]);   
			
		  buffer_append(buf, "\n\n"); 
			
        buffer_append(buf, "Prescription:\n");
        for( int j = 5; j < i ; j+=2 ) 
		{				
			buffer_append(buf, data[j]);	
			
			if(data[j+1][0] == '1')
				buffer_append(buf, "\n1-----------");
			else if(data[j+1][0] == '2')
				buffer_append(buf, "\n1/2---------");
			else 
				buffer_append(buf, "\n0-----------");

			if(data[j+1][1] == '1')
				buffer_append(buf, "1---------");
			else if(data[j+1][1] == '2')
				buffer_append(buf, "1/2-------");
			else 
				buffer_append(buf, "0---------");

			if(data[j+1][2] == '1')
				buffer_append(buf, "--1");
			else if(data[j+1][2] == '2')
				buffer_append(buf, "1/2");
			else 
				buffer_append(buf, "--0");
            buffer_append(buf, "\n");
		}
			
		  buffer_append(buf, "\n");         
			
        buffer_append(buf, "Remarks:\n");
        buffer_append(buf, data[2]);
        buffer_append(buf, data[3]);
			
	      buffer_append(buf, "\n");	
		  buffer_append(buf, get_line(55,'_'));
		  buffer_append(buf, "\n\n"); 
    }  
    
    newtTextboxSetText(tb, buf->data);
    return tb;
}

/*
void main()
{   
    char data[7][200] = 
    {
        "there ya go",
        "lush life",
        "do we care?",
        "no",
        "hell yeah",
        "how's it goin!",
        "stocked"
    };

    wr_report_file(data,108,6);
}*/
