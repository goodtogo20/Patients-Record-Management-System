#include <stdio.h>
#include <newt.h>

void doc_loop(unsigned int rows, unsigned int cols)
{   
    unsigned int cenx = cols/2, ceny = rows/2, quat = cenx/2 ;
    char *search_id, *search_name, *res_search;    

    newtComponent main_form, b_current, b_logout, b_search;    

    newtComponent search_form, b_search_search, b_search_cancel,
                  tb_res_search, l_search_bi, l_search_bn, ent_bid, ent_bname;

    newtComponent cp_form, b_dequeue, b_cp_cancel, tb_report, scr, b_add_report;

    newtComponent add_report_form, b_add, b_ar_cancel, l_complaints, ent_complaints, 
                  ent_tablet, r_morning_one, r_morning_half, r_noon_one, r_noon_half,
                  r_night_one, r_night_half, l_remarks, ent_remarks;
                    
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
    b_add = newtButton(4, rows-9, "Add & Dequeue");
    b_ar_cancel = newtButton(26, rows-9, "Cancel");
    
    l_complaints = newtLabel(5, 2 ,"Complaints: ");
    ent_complaints = newtEntry(12,2,NULL,31, 0, 0);
    ent_tablet = newtEntry(12,4,NULL,31, 0, 0);

    r_morning_one = newtRadiobutton(14,6,"1",0,0); 
    r_morning_half = newtRadiobutton(14,6,"1/2", 0, r_morning_one);

    r_noon_one = newtRadiobutton(14,6,"1",0,0);
    r_noon_half = newtRadiobutton(14,6,"1/2",0,r_noon_one);

    r_night_one = newtRadiobutton(14,6,"1",0,0);
    r_night_half = newtRadiobutton(14,6,"1/2",0,r_night_one);
    
    l_remarks = newtLabel(5, 2 ,"Remarks: ");
    ent_remarks = newtEntry(12,2,NULL,31, 0,0);   

    //general components
    l_queue = newtLabel(cols-26, 0, "Queue");         
    l_line = newtLabel(cols-31, 1, "-------------------");
    tbox = newtTextbox( cols-30, 2, 15, 10, 0 );
    newtTextboxSetText( tbox,"1) there ya go\n2) Nice\n3) Pink FLOYD\n4) ThunderClouds\n5) BIG BUCKS" );        

    //initialising forms
    main_form = newtForm(NULL, NULL, 0);
    cp_form = newtForm(NULL, NULL, 0);
    add_report_form = newtForm(NULL, NULL, 0);
    search_form = newtForm(NULL, NULL, 0);   

    //adding components to form
    newtFormAddComponents(main_form, tbox, l_queue, l_line, b_current,
                          b_search, b_logout, NULL);
    
    newtFormAddComponents(cp_form, l_queue, l_line, tbox, b_add_report, b_dequeue, b_cp_cancel, 
                          tb_report , NULL);
    
    newtFormAddComponents(add_report_form, l_queue, l_line, tbox, b_add, b_ar_cancel, 
                          l_complaints, ent_complaints, ent_tablet, r_morning_one, 
                          r_morning_half, r_noon_one, r_noon_half, r_night_one, r_night_half, 
                          l_remarks, ent_remarks, NULL);

    newtFormAddComponents(search_form, l_queue, l_line, tbox, l_search_bi, 
                          l_search_bn, ent_bid, ent_bname, b_search_search, 
                          b_search_cancel, NULL);
    do
    {
        newtOpenWindow(2,2,cols-5 ,rows-5,"Doctor");
        ch_form = newtRunForm(main_form);
        
        if( ch_form == b_current )        
        {
            newtPopWindow();
            newtOpenWindow(2,2,cols-5 ,rows-5,"Doctor");
            ch_form = newtRunForm(cp_form);
            
            if(ch_form == b_add_report)
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
