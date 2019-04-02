#include <stdio.h>
#include <stdlib.h>
#include "gen_id.c"

#define MALE 1
#define FEMALE 2

#define NAME 0
#define DAY 1
#define MONTH 2
#define YEAR 3
#define AGE 4
#define ADDRESS 5
#define NO 6

int wr_file(char *record[][100], int gender)
{
    FILE *fp = fopen("pat_data.txt","a");
    
    if(fp == NULL)
    {
        printf("Error opening file");
    }
    
    //generates id and adds before each entry in file
    fprintf(fp,"%u ",gen_id());
    update_conf();        

    fprintf(fp,"%s ",*record[NAME]);
    fprintf(fp,"%s ",*record[DAY]);
    fprintf(fp,"%s ",*record[MONTH]);
    fprintf(fp,"%s ",*record[YEAR]);
    fprintf(fp,"%s ",*record[AGE]);
    fprintf(fp,"%d ",gender);
    fprintf(fp,"%s ",*record[NO]);
    fprintf(fp,"%s\n",*record[ADDRESS]);
    
    fclose(fp);
    return 1;
}
/*
void main()
{   
    char record[7][100] = {
        "Mehul",
        "20","02","2000",
        "19",
        "9422613544",
        "kolhapur"
    };

    wr_file(record,898);
}   */
