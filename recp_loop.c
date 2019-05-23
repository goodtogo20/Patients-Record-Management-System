#include <stdio.h>
#include <newt.h>
#include <string.h>

#include "buffer.h"
#include "write_pr_file.h"
#include "search.h"
#include "validate.h"
#include "send_sig_to_doc.h"

#define NAME 0
#define DAY 1
#define MONTH 2
#define YEAR 3
#define AGE 4
#define ADDRESS 5
#define NO 6

#define MALE 1
#define FEMALE 2

extern newtComponent tbox;
extern buffer_t *pinfo;

void *recp_loop()
{ 
    unsigned int rows, cols; FILE *fp = fopen("recptest.txt","w");
    newtGetScreenSize(&cols, &rows);
  
    unsigned int cenx = cols/2, ceny = rows/2, quat = cenx/2 ;
    char *record[7][100], *add2, *add3, *old_p, *new_p, *new_pa;
    char *search_id, *search_name, *ptr, *res_search, name_data[20][200], sbn_id[1000];
    int gender = -1; 

	void* keys[20];

	//global varibles initialization
    pinfo = buffer_new();

    newtComponent main_form, b_new_rec, b_settings, b_search, 
                  b_logout, l_queue, l_line, l_op, l_np, l_npa, ent_op, 
                            ent_np, ent_npa, cpword_form;

    newtComponent new_rec_form, b_add_rec, l_name, l_dob, 
                  l_gender, l_address, l_age, l_no, ent_name,
                  ent_gender, ent_address1, ent_address2, 
                  r_male, r_female, ent_address3, ent_age, 
                  ent_no, ent_date, ent_month, ent_year, b_wf_cancel, b_change;

    newtComponent search_form, b_search_cancel, tb_res_search,
                  l_value ,b_search_bi, b_search_bn,  ent_value,b_change_pword, lbox, b_enq, b_detail;

    newtComponent settings_form;
	newtComponent detail_form;
    newtComponent b_cancel, ch_form, b_set_cancel;

    //main form components
    b_new_rec = newtButton(5, 1, "New Record");
    b_search = newtButton(5, 6, "Search");
    b_settings = newtButton(5, 11, "Settings");
    b_logout = newtButton(5, 16, "Logout & Exit");
    l_queue = newtLabel(cols-26, 0, "Queue");         
    l_line = newtLabel(cols-31, 1, "---------------------");
    
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
    l_value = newtLabel(5,0,"Enter Value:");
    ent_value = newtEntry(20,0,NULL,28,(const char**) &search_id,0);

    b_search_bi = newtButton(4, 2, "Search by ID");
	b_search_bn = newtButton(22, 2, "Search by name");
    b_search_cancel = newtButton(42, 2, "Cancel");
    b_enq = newtCompactButton(5, 21, "Add to queue");
    b_detail = newtCompactButton(22, 21, "See Details");
    lbox = newtListbox(5, 9, 12, NEWT_FLAG_SCROLL | NEWT_FLAG_BORDER);
    newtListboxSetWidth(lbox,60);
    //settings form components

    //texbox system for queue
    newtTextboxSetText( tbox,"Queue is Empty!" );
    
    //settings forms    
    b_change_pword = newtButton(5,1,"Change Password");
    b_set_cancel = newtButton(5,7,"Cancel");

    //    cpword_form
    l_op = newtLabel(5, 2, "Old Password: ");
    l_np = newtLabel(5, 5, "New Password: ");
    l_npa = newtLabel(5, 7, "Enter Again: "); 
    ent_op = newtEntry(20,2,0,30,(const char**) &old_p,0);
    ent_np = newtEntry(20,5,0,30,(const char**) &new_p,0);
    ent_npa = newtEntry(20,7,0,30,(const char**) &new_pa,0);

    b_change = newtButton(5,10,"Change"); 
    b_wf_cancel  = newtButton(20,10,"Cancel");

    //intialisation for forms
    new_rec_form = newtForm(NULL, NULL, 0);
    main_form = newtForm(NULL, NULL, 0);
    search_form = newtForm(NULL, NULL, 0);
    settings_form = newtForm(NULL,NULL,0);
	detail_form = newtForm(NULL,NULL,0);
    cpword_form = newtForm(NULL,NULL,0);
    
    //adding components into forms
    newtFormAddComponents(main_form, l_queue, l_line, tbox, b_new_rec, 
                          b_search, b_settings, b_logout,NULL);

    newtFormAddComponents(new_rec_form, l_queue, l_line, tbox, l_name, 
                          l_dob, l_gender, l_address, ent_name, ent_date, 
                          ent_month, ent_year, ent_age, r_male, r_female, 
                          l_age, ent_address1, ent_address2, ent_address3, 
                          l_no,ent_no, b_add_rec, b_cancel,NULL);

    newtFormAddComponents(search_form, l_queue, l_line, tbox, l_value, 
                          ent_value, b_search_bi, b_search_bn, b_search_cancel, 
						   lbox, b_enq, b_detail, NULL);

    newtFormAddComponents(settings_form,l_queue, l_line,tbox, b_change_pword, b_set_cancel, NULL);        
     
    newtFormAddComponents(cpword_form, l_queue, l_line,tbox, l_op, l_np, l_npa, ent_op, 
                            ent_np, ent_npa, b_change, b_wf_cancel, NULL);

    do
    {   
        newtOpenWindow(2,2,cols-5 ,rows-5,"Main Menu");
        ch_form = newtRunForm(main_form);
        
        if( ch_form == b_new_rec )
        {   
           newtPopWindow();
           newtOpenWindow(2,2,cols-5 ,rows-5,"New Record");
           ch_form = newtRunForm(new_rec_form);

           if( ch_form == b_add_rec && validate(record,gender) )
           {	
				if( newtRadioGetCurrent(r_male) == r_male )
				{
					gender = MALE;
				} else
				{
					gender = FEMALE;
				}
				
				//append remaining addresses to record 
				strcat(*record[ADDRESS]," ");
				strcat(*record[ADDRESS],add2);
				strcat(*record[ADDRESS]," ");				
				strcat(*record[ADDRESS],add3);
			
				newtPushHelpLine(add2)				;
				newtWaitForKey();


                wr_pinfo_file(record,gender);
           }
            
        } else if( ch_form == b_search )
        {
           newtPopWindow();
           newtOpenWindow(2,2,cols-5 ,rows-5,"Search");
			
            newtComponentTakesFocus(lbox, 0);
			newtComponentTakesFocus(b_enq, 0);
			newtComponentTakesFocus(b_detail, 0);
				      
           ch_form = newtRunForm(search_form);            
	   
	       if( ch_form == b_search_bi )
           {					
	        	//res_search = lbox,search_by_id(search_id);
				//newtTextboxSetText( tb_res_search, res_search ); 						

				//Handles error if no result is found
                if( (res_search = search_by_id(search_id)) == 0 )
				{
					newtListboxAppendEntry(lbox, "No Result Found!",0);
				}else
				{
					newtListboxAppendEntry(lbox, res_search,0);

					//take focus back
					newtComponentTakesFocus(b_enq, 1);
					newtComponentTakesFocus(b_detail, 1);
				}				              
				
				ch_form = newtRunForm( search_form );	
                
                if( ch_form == b_enq )                
                {
                    //1->enqueue
                    send_sig_to_doc(1, search_id);
                } else if( ch_form == b_detail)
                {
                   //extract patient info
                    set_pinfo(search_id);          
                    newtWinMessage("Patient Info","Cancel",pinfo->data); 					
                }

                //clear value field while exiting
                newtListboxClear(lbox);
                newtEntrySet(ent_value, "", 0);
                
           } else if ( ch_form == b_search_bn )
		   {	 						
				int max_bn = search_by_name_recp(search_id, name_data);

				if(max_bn == -1)
				{
					newtListboxAppendEntry(lbox, "No Results Found!",0);
				}else
				{	
					for(int i=0 ; i < max_bn ; i++)
					{
						newtListboxAppendEntry(lbox, name_data[i], name_data[i]);
					}
				
					//Take focus back
		            newtComponentTakesFocus(lbox, 1);
					newtComponentTakesFocus(b_enq, 1);
					newtComponentTakesFocus(b_detail, 1);
				}			
				
				ch_form = newtRunForm( search_form );
					
				//get id of currently selected value in lbox
				for(int i=0; i < max_bn; i++)
				{
					if(newtListboxGetCurrent(lbox) == name_data[i])
					{
						sscanf(name_data[i],"%s",sbn_id);
						break;
					}
				}

				if( ch_form == b_enq )                
                {   
                    //1->enqueue
					send_sig_to_doc(1,sbn_id);	
									
                } else if( ch_form == b_detail)
                {
                   //extract patient info
                    set_pinfo(sbn_id);              
                    newtWinMessage("Patient Info","Cancel",pinfo->data); 
                }

                //clear value field while exiting
                newtListboxClear(lbox);
                newtEntrySet(ent_value, "", 0);

     	   }
        } else if( ch_form == b_settings )
        {
            newtPopWindow();
            newtOpenWindow(2, 2, cols-5 ,rows-5,"Settings");
            ch_form = newtRunForm(settings_form);
            if(b_change_pword == ch_form) 
            {
                newtPopWindow();
                newtOpenWindow(2,2,cols-5 ,rows-5,"Change Password");
                ch_form = newtRunForm(cpword_form);  

                if(ch_form == b_change)
                {
                    change_password(2, old_p, new_p, new_pa);
                }
            }
        }
        
    }while( ch_form !=  b_logout );
    fclose(fp);
}

/*
    newtOpenWindow(cenx + quat - 2 , 2, quat, rows-5, "Queue");
    newtOpenWindow(2,2,cenx+quat-8,rows-5,"Recep");
*/
