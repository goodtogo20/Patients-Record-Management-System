#include <stdio.h>
#include <newt.h>

#define ENTRY 0
#define MOR_ONE 1
#define MOR_HALF 2
#define NOON_ONE 3
#define NOON_HALF 4
#define NIGHT_ONE 5
#define NIGHT_HALF 6
#define LINE_ONE 7
#define LINE_TWO 8

void print_yeah(newtComponent tab[][9])
{
    printf("THERE YOU GO\n LKAJLLDFLLDJS");
}

void add_tablet_entry(newtComponent tab[][9])
{
    static int index = 2;
    static int row = 8;

    tab[index][ENTRY] = newtEntry(5,5,NULL,45, 0, 0);
    tab[index][MOR_ONE] = newtRadiobutton(5,7,"1",0,0); 
    tab[index][MOR_HALF] = newtRadiobutton(5,7,"1",0,tab[index][MOR_ONE]);
    tab[index][NOON_ONE] = newtRadiobutton(5,7,"1",0,0);
    tab[index][NOON_HALF] = newtRadiobutton(5,7,"1",0,tab[index][NOON_ONE]);
    tab[index][NIGHT_ONE] = newtRadiobutton(5,7,"1",0,0);
    tab[index][NIGHT_HALF] = newtRadiobutton(5,7,"1",0,tab[index][NIGHT_ONE]);
    tab[index][LINE_ONE] = newtLabel(5,0,"------");
    tab[index][LINE_TWO] = newtLabel(5,0,"------");
    
    index++;
    row = row + 2 ;

    return;    
}

void doc_loop(unsigned int rows, unsigned int cols)
{   
    unsigned int cenx = cols/2, ceny = rows/2, quat = cenx/2 ;
    char *search_id, *search_name, *res_search;    

    newtComponent main_form, b_current, b_logout, b_search;    

    newtComponent search_form, b_search_search, b_search_cancel, tb_res_search, 
                  l_search_bi, l_search_bn, ent_bid, ent_bname, l_prescription;

    newtComponent cp_form, tablet_form, tablet[10][9], b_add_another, b_dequeue, 
                  b_cp_cancel, tb_report, scr, b_add_report;

    newtComponent add_report_form, b_add, b_ar_cancel, l_complaints ,ent_complaints2, 
                  ent_complaints1, l_remarks, ent_remarks1, ent_remarks2, button;
                    
    newtComponent tbox, l_queue, l_line, ch_form;
    
    //main form components	
    b_current = newtButton(4, rows-9, "Current Patient");
    b_search = newtButton(28, rows-9, "Search");
    b_logout = newtButton(43, rows-9, "Logout & Exit");
    
    //current patient form components
    b_add_report = newtButton(4, rows - 9, "Add Report");
    tb_report = newtTextbox( 5, 2, 40, 10, 0 );
    //scr = newtVerticalScrollbar(4, 4, 8, 0, 0);
    b_dequeue = newtButton(23, rows-9, "Dequeue");
    b_cp_cancel = newtButton(39, rows-9, "Cancel");

    //search form components
    l_search_bi = newtLabel(5,0,"Enter ID:");
    l_search_bn = newtLabel(5,2,"Enter Name:");
    ent_bid = newtEntry(20,0,NULL,28,(const char**) &search_id,0);
    ent_bname = newtEntry(20,2,NULL,28,(const char**) &search_name,0);
    b_search_search = newtButton(4, rows-9, "Search go");
    b_search_cancel = newtButton(19, rows-9, "Cancel");
	tb_res_search = newtTextbox(5, 4, 40,5,0);
    
    //add report components
    scr = newtVerticalScrollbar(52, 5, 9, 0,0);
    b_add = newtButton(4, rows-9, "Add & Dequeue");
    b_ar_cancel = newtButton(26, rows-9, "Cancel");
    b_add_another = newtCompactButton(32,9,"Add Another");
    
    l_complaints = newtLabel(5, 1 ,"Complaints: ");
    l_prescription = newtLabel(5,5, "Prescription: " );
    ent_complaints1 = newtEntry(5,2,NULL,45, 0, 0);
    ent_complaints2 = newtEntry(5,3,NULL,45, 0, 0);
   
    tablet[0][ENTRY] = newtEntry(5,5,NULL,45, 0, 0);

    tablet[0][MOR_ONE] = newtRadiobutton(5,7,"1",0,0); 
    tablet[0][MOR_HALF] = newtRadiobutton(10,7,"1/2", 0, tablet[0][MOR_ONE] );

    tablet[0][NOON_ONE] = newtRadiobutton(22, 7, "1", 0, 0);
    tablet[0][NOON_HALF] = newtRadiobutton(27, 7, "1/2", 0, tablet[0][NOON_ONE] );

    tablet[0][NIGHT_ONE] = newtRadiobutton(38,7,"1",0,0);
    tablet[0][NIGHT_HALF] = newtRadiobutton(43,7,"1/2",0,tablet[0][NIGHT_ONE] );
    
    l_remarks = newtLabel(5, 15 ,"Remarks: ");
    ent_remarks1 = newtEntry(5,16,NULL,45, 0,0);  
    ent_remarks2 = newtEntry(5,17,NULL,45, 0,0); 

    //general components
    l_queue = newtLabel(cols-26, 0, "Queue");         
    l_line = newtLabel(cols-31, 1, "-------------------");
    tbox = newtTextbox( cols-30, 2, 15, 10, 0 );
    newtTextboxSetText( tbox,"1) there ya go\n2) Nice\n3) Pink FLOYD\n4) ThunderClouds\n5) BIG BUCKS" );            
    
    //newtScrollbarSet(scr, 1, 5);

    //initialising forms
    main_form = newtForm(NULL, NULL, 0);
    cp_form = newtForm(NULL, NULL, 0);
    add_report_form = newtForm(NULL, NULL, 0);
    tablet_form = newtForm(scr, NULL, NEWT_FLAG_SCROLL);
    search_form = newtForm(NULL, NULL, 0);   
        
    newtFormSetHeight(tablet_form, 9);
    newtFormSetWidth(tablet_form, 50);

    //newtFormSetScrollPosition(scr, 2);
    
    //adding components to form
    newtFormAddComponents(main_form, tbox, l_queue, l_line, b_current,
                          b_search, b_logout, NULL, NULL, NULL);
    
    newtFormAddComponents(cp_form, l_queue, l_line, tbox, b_add_report, b_dequeue, b_cp_cancel, 
                          tb_report , NULL);

    newtFormAddComponents(tablet_form, scr, tablet[0][ENTRY], tablet[0][MOR_ONE], tablet[0][MOR_HALF],
                          tablet[0][NOON_ONE], tablet[0][NOON_HALF], tablet[0][NIGHT_ONE], 
                          tablet[0][NIGHT_HALF], b_add_another, NULL);

    newtFormAddComponents(add_report_form, tablet_form, l_queue, l_line, tbox, b_add, b_ar_cancel, 
                          l_complaints, l_prescription, ent_complaints1, ent_complaints2, l_remarks, ent_remarks1, 
                          ent_remarks2, NULL);

    newtFormAddComponents(search_form, l_queue, l_line, tbox, l_search_bi, 
                          l_search_bn, ent_bid, ent_bname, b_search_search, 
                          b_search_cancel, NULL);

    /*newtFormAddComponents(tablet_form, scr, 
                          tablet[0][ENTRY], 
                          tablet[0][MOR_ONE], tablet[0][MOR_HALF],
                          tablet[0][NOON_ONE], tablet[0][NOON_HALF], 
                          tablet[0][NIGHT_ONE], tablet[0][NIGHT_HALF],

                          tablet[1][ENTRY], 
                          tablet[1][MOR_ONE], tablet[1][MOR_HALF],
                          tablet[1][NOON_ONE], tablet[1][NOON_HALF], 
                          tablet[1][NIGHT_ONE], tablet[1][NIGHT_HALF],

                          tablet[2][ENTRY], 
                          tablet[2][MOR_ONE], tablet[2][MOR_HALF],
                          tablet[2][NOON_ONE], tablet[2][NOON_HALF], 
                          tablet[2][NIGHT_ONE], tablet[2][NIGHT_HALF],

                          tablet[3][ENTRY], 
                          tablet[3][MOR_ONE], tablet[3][MOR_HALF],
                          tablet[3][NOON_ONE], tablet[3][NOON_HALF], 
                          tablet[3][NIGHT_ONE], tablet[3][NIGHT_HALF],

                          tablet[4][ENTRY], 
                          tablet[4][MOR_ONE], tablet[4][MOR_HALF],
                          tablet[4][NOON_ONE], tablet[4][NOON_HALF], 
                          tablet[4][NIGHT_ONE], tablet[4][NIGHT_HALF],

                          tablet[5][ENTRY], 
                          tablet[5][MOR_ONE], tablet[5][MOR_HALF],
                          tablet[5][NOON_ONE], tablet[5][NOON_HALF], 
                          tablet[5][NIGHT_ONE], tablet[5][NIGHT_HALF],

                          tablet[6][ENTRY], 
                          tablet[6][MOR_ONE], tablet[6][MOR_HALF],
                          tablet[6][NOON_ONE], tablet[6][NOON_HALF], 
                          tablet[6][NIGHT_ONE], tablet[6][NIGHT_HALF],

                          tablet[7][ENTRY], 
                          tablet[7][MOR_ONE], tablet[7][MOR_HALF],
                          tablet[7][NOON_ONE], tablet[7][NOON_HALF], 
                          tablet[7][NIGHT_ONE], tablet[7][NIGHT_HALF], 

                          tablet[8][ENTRY], 
                          tablet[8][MOR_ONE], tablet[8][MOR_HALF],
                          tablet[8][NOON_ONE], tablet[8][NOON_HALF], 
                          tablet[8][NIGHT_ONE], tablet[8][NIGHT_HALF],

                          tablet[9][ENTRY], 
                          tablet[9][MOR_ONE], tablet[9][MOR_HALF],
                          tablet[9][NOON_ONE], tablet[9][NOON_HALF], 
                          tablet[9][NIGHT_ONE], tablet[9][NIGHT_HALF], NULL);*/      
    do
    {
        newtOpenWindow(2,2,cols-5 ,rows-5,"Doctor");
        ch_form = newtRunForm(main_form);
        
        if( ch_form == b_current )        
        {
            newtPopWindow();
            newtOpenWindow(2,2,cols-5 ,rows-5,"Doctor");
            ch_form = newtRunForm(cp_form);
            
            //TODO: ADD CALLBACK FUNCTION
            newtComponentAddCallback(b_add_another,(newtCallback)print_yeah,NULL);
    
            if( ch_form == b_add_report )
            {                   
                newtRunForm( add_report_form );
            }

        } else if( ch_form == b_search )
        {
            newtPopWindow();
            newtOpenWindow(2,2,cols-5 ,rows-5,"Doctor");
            ch_form = newtRunForm(search_form);

            if( ch_form == b_search_search )
            {
                res_search = search_by_id(search_id);
			    newtTextboxSetText( tb_res_search, res_search );
			    
			    newtFormAddComponents(search_form, l_queue, l_line, tbox, l_search_bi, 
                                      l_search_bn, ent_bid, ent_bname, b_search_search, 
                                      b_search_cancel, tb_res_search, NULL);

			    newtRunForm( search_form );
            }	
        }
        
    }while( ch_form !=  b_logout );
    
}
