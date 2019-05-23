#include <stdio.h>
#include <newt.h>
#include "buffer.h"

#include "pqueue.h"
#include "search.h"
#include "wr_report_file.h"
#include "send_sig_to_recp.h"

#define MAX_TABLET_ENTRIES 7

#define ENTRY 0
#define MOR_ONE 1
#define MOR_HALF 2
#define NOON_ONE 3
#define NOON_HALF 4
#define NIGHT_ONE 5
#define NIGHT_HALF 6
#define LINE_ONE 7
#define LINE_TWO 8

extern newtComponent tbox;
extern newtComponent rtbox;
extern struct queue *pque;
extern buffer_t *pinfo;

int add_tablet_entry(newtComponent tab[][9], newtComponent add, char *data[][200])
{
    static int index = 0;
    static int row = 6;
    static int data_index= 4;

    index++;
    data_index++;

    if( index == MAX_TABLET_ENTRIES )
        return -1;

    tab[index][ENTRY] = newtEntry(5,row,NULL,48,(const char**)data[data_index], 0);

    tab[index][MOR_ONE] = newtRadiobutton(5,row+1,"1",0,0);
    tab[index][MOR_HALF] = newtRadiobutton(10,row+1,"1/2",0,tab[index][MOR_ONE]);
    tab[index][NOON_ONE] = newtRadiobutton(24,row+1,"1",0,0);
    tab[index][NOON_HALF] = newtRadiobutton(29,row+1,"1/2",0,tab[index][NOON_ONE]);
    tab[index][NIGHT_ONE] = newtRadiobutton(41,row+1,"1",0,0);
    tab[index][NIGHT_HALF] = newtRadiobutton(46,row+1,"1/2",0,tab[index][NIGHT_ONE]);
    tab[index][LINE_ONE] = newtLabel(17, row+1, "-------");
    tab[index][LINE_TWO] = newtLabel(36, row+1, "-----");

    row = row + 2;
    return index;
}

void *doc_loop()
{
    unsigned int rows, cols;
    newtGetScreenSize(&cols, &rows);

    unsigned int cenx = cols/2, ceny = rows/2, quat = cenx/2, pid = 108;
    char *search_id, *search_name, *res_search, *data[11][200], name_data[20][200],
          *old_p, *new_p, *new_pa, str[100], sbn_id[10];
    int ind = 0, ctr = 0, tab_time[7][2] = { 0, 0 }, p_ser_req, id = 0;

    struct newtExitStruct stat;

    newtComponent main_form, b_current, b_logout, b_search;

    newtComponent search_form, b_search_search, b_search_cancel, tb_res_search,
                  l_search_bi, l_search_bn, ent_bid, ent_bname, l_prescription;

    newtComponent cp_form, tablet_form, complaint_form, remarks_form, tablet[10][9] = {0,0},
                  b_add_another, b_dequeue, b_cp_cancel, tb_report, scr, b_add_report;

    newtComponent add_report_form, b_add, b_ar_cancel, l_complaints ,ent_complaints2,
                  ent_complaints1, l_remarks, ent_remarks1, ent_remarks2, button, b_change_pword, b_set_cancel;

    newtComponent l_queue, l_line, ch_form, b_done, l_value,ent_value, b_search_bi,
                  b_search_bn, lbox, b_detail, detail_form,b_settings, b_change,l_op, l_np, l_npa, ent_op,
                  ent_np, ent_npa, settings_form, cpword_form, b_wf_cancel;
    
    //global varibles initialization
    pinfo = buffer_new();

    //main form components
    b_current = newtButton(5, 1, "Current Patient");
    b_search = newtButton(5, 6, "Search");
    b_settings = newtButton(5, 11,"Settings");
    b_logout = newtButton(5, 16, "Logout & Exit");

    //current patient form components
    b_add_report = newtButton(4, rows - 9, "Add Report");
    //scr = newtVerticalScrollbar(4, 4, 8, 0, 0);
    rtbox = newtTextbox(5, 1, 60, 20, NEWT_FLAG_SCROLL);
    b_dequeue = newtButton(23, rows-9, "Dequeue");
    b_cp_cancel = newtButton(39, rows-9, "Cancel");

    //search form components
    l_value = newtLabel(5,0,"Enter Value:");
    ent_value = newtEntry(20,0,NULL,28,(const char**) &search_id,0);

    b_search_bi = newtButton(4, 2, "Search by ID");
	b_search_bn = newtButton(22, 2, "Search by name");
    b_search_cancel = newtButton(42, 2, "Cancel");
    b_detail = newtCompactButton(5, 21, "See Details");
    lbox = newtListbox(5, 9, 12, NEWT_FLAG_SCROLL | NEWT_FLAG_BORDER);
    newtListboxSetWidth(lbox,60);

    //add report components
    scr = newtVerticalScrollbar(54, 4, 12, 0,0);
    b_add = newtButton(4, rows-9, "Add & Dequeue");
    b_ar_cancel = newtButton(26, rows-9, "Cancel");

    l_complaints = newtLabel(5, 0 ,"Complaints: ");
    l_prescription = newtLabel(5, 3, "Prescription: " );
    ent_complaints1 = newtEntry(5, 1, NULL, 50, (const char**) data[0], 0);
    ent_complaints2 = newtEntry(5, 2, NULL, 50, (const char**) data[1], 0);

    tablet[0][ENTRY] = newtEntry(5,4,NULL,48, (const char**) data[4], 0);
    tablet[0][MOR_ONE] = newtRadiobutton(5,5,"1",0,0);
    tablet[0][MOR_HALF] = newtRadiobutton(10,5,"1/2", 0, tablet[0][MOR_ONE] );
    tablet[0][NOON_ONE] = newtRadiobutton(24, 5, "1", 0, 0);
    tablet[0][NOON_HALF] = newtRadiobutton(29, 5, "1/2", 0, tablet[0][NOON_ONE] );
    tablet[0][NIGHT_ONE] = newtRadiobutton(41,5,"1",0,0);
    tablet[0][NIGHT_HALF] = newtRadiobutton(46,5,"1/2",0,tablet[0][NIGHT_ONE] );
    tablet[0][LINE_ONE] = newtLabel(17,5,"-------");
    tablet[0][LINE_TWO] = newtLabel(36,5,"-----");

    l_remarks = newtLabel(5, 16 ,"Remarks: ");
    ent_remarks1 = newtEntry(5,17,NULL,50, (const char**) data[2],0);
    ent_remarks2 = newtEntry(5,18,NULL,50, (const char**) data[3],0);

    //general components
    l_queue = newtLabel(cols-26, 0, "Queue");
    l_line = newtLabel(cols-31, 1, "---------------------");

    //settings forms    
    b_change_pword = newtButton(5,1,"Change Password");
    b_set_cancel = newtButton(5,7,"Cancel");

    newtTextboxSetText( tbox,"Queue is Empty!" );
    //newtTextboxSetText( tbox,"1) there ya go\n2) Nice\n3) Pink FLOYD\n4) ThunderClouds\n5) BIG BUCKS" );
    //fill_pque_tbox();

    //    cpword_form
    l_op = newtLabel(5, 2, "Old Password: ");
    l_np = newtLabel(5, 5, "New Password: ");
    l_npa = newtLabel(5, 7, "Enter Again: ");
    ent_op = newtEntry(20,2,0,30,(const char**) &old_p,0);
    ent_np = newtEntry(20,5,0,30,(const char**) &new_p,0);
    ent_npa = newtEntry(20,7,0,30,(const char**) &new_pa,0);

    b_change = newtButton(5,10,"Change");
    b_wf_cancel  = newtButton(20,10,"Cancel");

    //initialising forms
    main_form = newtForm(NULL, NULL, 0);
    cp_form = newtForm(NULL, NULL, 0);
    add_report_form = newtForm(NULL, NULL, 0);
    complaint_form = newtForm(NULL,NULL,0);
    tablet_form = newtForm(scr, NULL, NEWT_FLAG_SCROLL);
    remarks_form =newtForm(NULL,NULL,0);
    search_form = newtForm(NULL, NULL, 0);
    settings_form = newtForm(NULL, NULL,0);
    cpword_form = newtForm(NULL,NULL,0);

    //width and height of tablet form
    newtFormSetHeight(tablet_form, 12);
    newtFormSetWidth(tablet_form, 50);

    //adding components to form
    newtFormAddComponents(main_form, tbox, l_queue, l_line, b_current, b_search, b_settings, b_logout, NULL);

    newtFormAddComponents(cp_form, l_queue, l_line, tbox, rtbox, b_add_report, b_dequeue, b_cp_cancel, NULL);

    newtFormAddComponents(complaint_form, l_complaints, ent_complaints1, ent_complaints2, NULL);

    newtFormAddComponents(tablet_form, tablet[0][ENTRY], tablet[0][MOR_ONE], tablet[0][MOR_HALF],
                          tablet[0][NOON_ONE], tablet[0][NOON_HALF], tablet[0][NIGHT_ONE],
                          tablet[0][NIGHT_HALF], tablet[0][LINE_ONE],tablet[0][LINE_TWO],
                          scr, NULL);

    newtFormAddComponents(remarks_form, l_remarks, ent_remarks1, ent_remarks2, NULL);

    newtFormAddComponents(settings_form, l_queue, l_line, tbox, b_change_pword, b_set_cancel, NULL);

    newtFormAddComponents(add_report_form, complaint_form, tablet_form, remarks_form, l_queue,
                          l_line, tbox, b_add, b_ar_cancel, l_prescription, NULL);

    newtFormAddComponents(search_form, l_queue, l_line, tbox, l_value,
                          ent_value, b_search_bi,
                          b_search_bn, b_search_cancel,
						  lbox, b_detail, NULL);
    newtFormAddComponents(cpword_form, l_queue, l_line, tbox, l_op, l_np, l_npa, ent_op,
                            ent_np, ent_npa, b_change, b_wf_cancel, NULL);

    //HOTKEY to add new tablet entries in tablet form
    newtFormAddHotKey(add_report_form, NEWT_KEY_F1);

    do
    {
        newtOpenWindow(2,2,cols-5 ,rows-5,"Main Menu");
        ch_form = newtRunForm(main_form);

        //MAIN->CURRENT_PATIENT
        if( ch_form == b_current )
        {
            newtPopWindow();
            newtOpenWindow(2,2,cols-5 ,rows-5,"Patient");
               
            //get current patient in "id" variable
            if(!isEmpty(pque))
            {
                id = pque->array[pque->head];
                set_report(id);

                sprintf(str,"%d",id);
                newtPushHelpLine(str);
            }else
            {
                id = -1;
                newtWinMessage("Error","Cancel","No Patients in queue");
                continue;                
            }
            
            //newtWaitForKey();
            ch_form = newtRunForm(cp_form);

            //MAIN->CURRENT_PATIENT->ADD_REPORT
            if( ch_form == b_add_report )
            {
                newtSetColor(NEWT_COLORSET_ENTRY, "black", "cyan");
                //newtFormAddComponent(tablet_form,b_add_another);
                while( (ch_form = newtRunForm(add_report_form)) != b_ar_cancel )
                {
                    if( ch_form == 0 )
                    {
                        if( (ind = add_tablet_entry(tablet, b_add_another, data)) != -1)
                        {
                            newtFormAddComponent(tablet_form,tablet[ind][ENTRY]);
                            newtFormAddComponent(tablet_form,tablet[ind][MOR_ONE]);
                            newtFormAddComponent(tablet_form,tablet[ind][MOR_HALF]);
                            newtFormAddComponent(tablet_form,tablet[ind][NOON_ONE]);
                            newtFormAddComponent(tablet_form,tablet[ind][NOON_HALF]);
                            newtFormAddComponent(tablet_form,tablet[ind][NIGHT_ONE]);
                            newtFormAddComponent(tablet_form,tablet[ind][NIGHT_HALF]);
                            newtFormAddComponent(tablet_form,tablet[ind][LINE_ONE]);
                            newtFormAddComponent(tablet_form,tablet[ind][LINE_TWO]);
                        }
                    }
                    //??
                    else if(ch_form == b_add)
                    {
                        wr_report_file(data, tablet, id, ind+5);
                        //1->dequeue
                        send_sig_to_recp(1,0);
                        //newtTextboxSetText(tb_report," ");
                        newtRefresh();
                        break;
                    }
                }//end while
                    newtSetColor(NEWT_COLORSET_ENTRY, "gray", "blue");
            }else if(ch_form == b_dequeue)
            {
                //1->dequeue
                send_sig_to_recp(1,0);
                //newtTextboxSetText(tb_report," ");
            }
        }
        //MAIN->SEARCH
        else if( ch_form == b_search )
        {
            newtPopWindow();
            newtOpenWindow(2,2,cols-5 ,rows-5,"Search");
            
            //remove focus
            newtComponentTakesFocus(lbox, 0);
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
					newtComponentTakesFocus(b_detail, 1);
				}  
				
				ch_form = newtRunForm( search_form );

                if( ch_form == b_detail )
                {
                    //extract patient info
                    set_pinfo(search_id);                
                    newtWinMessage("Patient Info","Cancel",pinfo->data);

                   /*newtPopWindow();
				   newtOpenWindow(2,2,cols-5 ,rows-5,"Details");

				   ch_form = newtRunForm(detail_form);*/
                }
                //clear value field while exiting
                newtListboxClear(lbox);
                newtEntrySet(ent_value, "", 0);

           } else if ( ch_form == b_search_bn )
		   {   
                //using recp side search by name temporarily
				int max_bn = search_by_name_recp(search_id, name_data);
               
                if(max_bn == -1)
				{
					newtListboxAppendEntry( lbox, "No Results Found!", 0 );
				} else
				{	
					for( int i=0 ; i < max_bn ; i++ )
					{
						newtListboxAppendEntry(lbox, name_data[i],name_data[i]);
					}
				
					//Take focus back
		            newtComponentTakesFocus(lbox, 1);
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

			    if( ch_form == b_detail)
                {   
                    //extract patient info
                    set_pinfo(sbn_id);                
                    newtWinMessage("Patient Info","Cancel",pinfo->data);
                }
                //clear value field while exiting
                newtListboxClear(lbox);
                newtEntrySet(ent_value, "", 0);   
      
            }
        } else if(ch_form == b_settings)
        {
            newtPopWindow();
            newtOpenWindow(2,2,cols-5 ,rows-5,"Settings");
            ch_form = newtRunForm(settings_form);

            if(b_change_pword == ch_form)
            {
                newtPopWindow();
                newtOpenWindow(2,2,cols-5 ,rows-5,"Change Password");
                ch_form = newtRunForm(cpword_form);

                if(ch_form == b_change)
                {
                    change_password(1, old_p, new_p, new_pa);
                }
            }
        }
        else if (ch_form == 0 )
        {
            printf("ALL HAIL AJEY THE AWPER");
        }

    }while( ch_form !=  b_logout );

}
