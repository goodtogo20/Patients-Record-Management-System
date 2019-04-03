#include<stdio.h>
#include<stdlib.h>

unsigned int str_to_uint(char *st) {
  char *x;
  for (x = st ; *x ; x++) {
    if (!isdigit(*x))
      return 0L;
  }
  return (strtoul(st, 0L, 10));
}

char* search_by_id(unsigned int idno)
{
	FILE *ptr;
	char *array;
	int id, i;

	array = (char*)malloc(sizeof(char)*200);
	
    ptr=fopen("pat_data.txt","r");

    for(i=0;i<=100;i++)
    {
         fscanf(ptr,"%s",array);
         id=atoi(array);

         if(id==idno)
         {
              fgets(array,255,(FILE*)ptr);
              break;
         }
    }
	return array;
}

