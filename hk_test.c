#include <stdio.h> 
#include <stdlib.h>
#include <newt.h>

void main()
{
    newtInit();
    newtCls();
    
    unsigned int rows, cols;
    newtGetScreenSize(&cols, &rows);

    newtOpenWindow(2,2,cols-5 ,rows-5,"Doctor");

    newtComponent form, btn,ent1,ent2, scr;
    
    ent1=newtEntry(15,8,NULL,28,0,0);
    ent2=newtEntry(15,20,NULL,28,0,0);
    btn=newtCompactButton(40,8,"Add Another");

    scr = newtVerticalScrollbar(60, 5, 10, 0,0);
    form = newtForm(scr,NULL,0);


    newtFormAddHotKey(form, NEWT_KEY_F1);


    newtFormSetHeight(form, 9);
    newtFormSetWidth(form, 50);
    newtFormAddHotKey(form,NEWT_KEY_F1);
    newtFormAddComponents(form, scr, 
                          ent1,
                          ent2,
                          btn, 
                          NULL);


    newtRunForm(form);
    
    newtPopWindow();
    newtFinished();
    exit(0);
}
