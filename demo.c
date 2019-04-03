#include<stdio.h>
#include<stdlib.h>

int main()
{
     FILE *ptr;
     char array[255];
     int id,i;
     int idno;;

     ptr=fopen("pat_data.txt","r");

      printf("\nEnter the Id to search: ");
          scanf("%d",&idno);
     for(i=0;i<=100;i++)
     {
          fscanf(ptr,"%s",array);
          id=atoi(array);

          if(id==idno)
          {
               fgets(array,255,(FILE*)ptr);
               printf("%d %s",id,array);
               break;
          }
     }

     return 0;
}
