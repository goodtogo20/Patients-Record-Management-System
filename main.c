#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <newt.h>

#include "login.c"
#include "recp_loop.c"
#include "doc_loop.c"

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

    unsigned int rows, cols;
    newtGetScreenSize(&cols, &rows);

    unsigned int winx=cols/2 - 22, winy=rows/2 - 6;
    char* ename, *epass;
    int role = 0;
    newtComponent lname,lpass,form,ent_name,ent_pass,lin,can,catch;

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
        {   
            if( ( role = login(ename,epass) ) != 0 )
            {
                newtPushHelpLine("");
                break;
            }
            else
                newtPushHelpLine("\tInvalid login, please try again");     
        }
        else if( catch == can )
        {
            newt_finish();
        }
        else
        {
            printf("Unknown error occured");
            newt_finish();
        }
    } 
    
    switch(role)
    {
        case 1:
            doc_loop(rows,cols);
            break;
        case 2:
            recp_loop(rows,cols);
            break;
        default:
            printf("Unknown error occured");
            break;
    }
    
    //newtWaitForKey();
    newt_finish();
    return 0;
 
}

