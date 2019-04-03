#include <stdio.h>
#include "write_pr_file.c"
#include "newt.h"
#include "search.c"

#define NAME 0
#define DAY 1
#define MONTH 2
#define YEAR 3
#define AGE 4
#define ADDRESS 5
#define NO 6

void recp_loop(unsigned int rows, unsigned int cols)
{   
    unsigned int cenx = cols/2, ceny = rows/2, quat = cenx/2, uint_search_id;
    char *record[7][100], add1[100], add2[100], add3[100];
    char *search_id, *search_name, *ptr, *res_search;
    int gender = -1;    

    newtComponent main_form, b_new_rec, b_settings, b_search, 
                  b_logout, tbox, l_queue, l_line;

    newtComponent new_rec_form, b_add_rec, l_name, l_dob, 
                  l_gender, l_address, l_age, l_no, ent_name,
                  ent_gender, ent_address1, ent_address2, 
                  r_male, r_female, ent_address3, ent_age, 
                  ent_no, ent_date, ent_month, ent_year;

    newtComponent search_form, b_search_search, b_search_cancel,tb_res_search,
                  l_search_bi, l_search_bn, ent_bid, ent_bname;

    newtComponent settings_form;
    newtComponent b_cancel, ch_form;

    //main form components
    b_new_rec = newtButton(4, rows-9, "New Record");
    b_search = newtButton(23, rows-9, "Search");
    b_settings = newtButton(38, rows-9, "Settings");
    b_logout = newtButton(55, rows-9, "Logout & Exit");
    l_queue = newtLabel(cols-26, 0, "Queue");         
    l_line = newtLabel(cols-31, 1, "-------------------");
    
    //new record form components
    b_add_rec = newtButton(4, rows-9, "Add Record");
    b_cancel = newtButton(23, rows-9, "Cancel"); 
    l_name = newtLabel(5, 2 ,"Name: ");
    l_dob = newtLabel(5, 4,"Birth Date: ");
    l_age = newtLabel(34,4,"Age: ");
    l_gender = newtLabel(5,6,"Gender: ");
    l_address = newtLabel(5,8,"Address: ");
    l_no = newtLabel(5,14,"Contact No: ");

    ent_name = newtEntry(12,2,NULL,31, (const char**) record[NAME],0);
    ent_date = newtEntry(18,4,NULL,2,(const char**) record[DAY],0);
    ent_month = newtEntry(22,4,NULL,2,(const char**) record[MONTH],0);
    ent_year = newtEntry(26,4,NULL,4,(const char**) record[YEAR],0);
    ent_age = newtEntry(40,4,NULL,3,(const char**) record[AGE],0);
    r_male = newtRadiobutton(14,6,"Male",0,0);
    r_female = newtRadiobutton(24,6,"Female",0,r_male);  
    ent_address1 = newtEntry(15,8,NULL,28,(const char**) record[ADDRESS],0);
    ent_address2 = newtEntry(15,10,NULL,28,(const char**) &add2,0);
    ent_address3 = newtEntry(15,12,NULL,28,(const char**) &add3,0);
    ent_no =  newtEntry(17,14,NULL,10,(const char**) record[NO],0);
    
    //search form components
    l_search_bi = newtLabel(5,0,"Enter ID:");
    l_search_bn = newtLabel(5,2,"Enter Name:");
    ent_bid = newtEntry(20,0,NULL,28,(const char**) &search_id,0);
    ent_bname = newtEntry(20,2,NULL,28,(const char**) &search_name,0);
    b_search_search = newtButton(4, rows-9, "Search");
    b_search_cancel = newtButton(19, rows-9, "Cancel");
	tb_res_search = newtTextbox(5, 4, 40,5,0);
    
    //settings form components
    //NULL

    //texbox system for queue
    tbox = newtTextbox( cols-30, 2, 15, 10, NEWT_FLAG_BORDER );
    newtTextboxSetText( tbox,"1) there ya go\n2) Nice\n3) Pink FLOYD\n4) ThunderClouds\n5) BIG BUCKS" );
    
    //intialisation for forms
    new_rec_form = newtForm(NULL, NULL, 0);
    main_form = newtForm(NULL, NULL, 0);
    search_form = newtForm(NULL, NULL, 0);
    settings_form = newtForm(NULL,NULL,0);
    
    //adding components into forms
    newtFormAddComponents(main_form, l_queue, l_line, tbox, b_new_rec, 
                          b_search, b_settings, b_logout,NULL);

    newtFormAddComponents(new_rec_form, l_queue, l_line, tbox, l_name, 
                          l_dob, l_gender, l_address, ent_name, ent_date, 
                          ent_month, ent_year, ent_age, r_male, r_female, 
                          l_age, ent_address1, ent_address2, ent_address3, 
                          l_no,ent_no, b_add_rec, b_cancel,NULL);

    newtFormAddComponents(search_form, l_queue, l_line, tbox, l_search_bi, 
                          l_search_bn, ent_bid, ent_bname, b_search_search, 
                          b_search_cancel, NULL);

    newtFormAddComponents(settings_form,l_queue, l_line,tbox, b_cancel, NULL);
              
    do
    {   
        newtOpenWindow(2,2,cols-5 ,rows-5,"Recep");
        ch_form = newtRunForm(main_form);
        
        if( ch_form == b_new_rec )
        {   
           newtPopWindow();
           newtOpenWindow(2,2,cols-5 ,rows-5,"Recep");
           ch_form = newtRunForm(new_rec_form);

           if( ch_form == b_add_rec /*&& validate(record,gender)*/ )
           {
                wr_file(record,gender);
           }
            
        }else if( ch_form == b_search )
        {
           newtPopWindow();
           newtOpenWindow(2,2,cols-5 ,rows-5,"Recep");
           ch_form = newtRunForm(search_form);            
	   
	       if( ch_form == b_search_search )
           {	
				uint_search_id = str_to_uint(search_id);
	        	res_search = search_by_id(uint_search_id);
				newtTextboxSetText( tb_res_search,res_search );
				
				
				newtFormAddComponents(search_form, l_queue, l_line, tbox, l_search_bi, 
                          l_search_bn, ent_bid, ent_bname, b_search_search, 
                          b_search_cancel,tb_res_search, NULL);

				newtRunForm(search_form);
				
           }

        }else if( ch_form == b_settings )
        {
            newtPopWindow();
            newtOpenWindow(2,2,cols-5 ,rows-5,"Recep");
            newtRunForm(settings_form); 
        }
        
    }while( ch_form !=  b_logout );
}

/*
    newtOpenWindow(cenx + quat - 2 , 2, quat, rows-5, "Queue");
    newtOpenWindow(2,2,cenx+quat-8,rows-5,"Recep");
*/
