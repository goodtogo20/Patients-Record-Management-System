#include <stdio.h>
#include <string.h>

unsigned int gen_id()
{   
    unsigned ref;
    FILE *fp = fopen("conf","r");
    
    fscanf(fp,"%u",&ref);
    fclose(fp);
    return ref+1;
}

void update_conf()
{
    unsigned ref;
    FILE *fp = fopen("conf","r+");
    
    fscanf(fp,"%u",&ref);
    fseek(fp,0,0);
    fprintf(fp,"%u",ref+1);

    fclose(fp);
    return;
}
/*
int main()
{
    update_conf();
}
*/
