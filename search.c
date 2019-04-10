#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

