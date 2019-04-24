#include <stdio.h>
#include <string.h>

int login(char e_uname[],char e_pass[])
{
    FILE *fp = fopen("pass.txt","r");
    char uname[100], pass[100], role[5];
    int pars;
    
    while(1)
    {
        pars = fscanf(fp,"%s %s %s", uname, pass, role);        
        
        if( pars == 1 )
        {
            break;
        }

        if( !strcmp(uname,e_uname) && !strcmp(e_pass,pass) )
        {
            if( !strcmp(role,"doc") )
            {
                return 1;
            } else if ( !strcmp(role,"recp") )
            {
                return 2;
            }
            printf("fb : %s %s %s",uname,pass,role);
        }
    }
    
    fclose(fp);
    return 0;
}


/*
void main()
{
    int cat;

    cat = login("goddf","god");  
    
    
    switch(cat)
    {   
        case 0:
            printf("login failed\n");
            break;
        case 1:
            printf("login successful, role doctor\n");
            break;
        case 2: 
            printf("login successful, role recep\n");
            break;
        default:
            printf("unknown error occured\n");
            break;
    }
}*/
