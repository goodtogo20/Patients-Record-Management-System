#ifndef SEARCH_
#define SEARCH_ 1
unsigned int str_to_uint(char *st);
char* search_by_id( char search_id[] );
char* search_by_id_req( char id[]);
int search_by_name_recp(char* qname, char data[][200]);
int search_by_name_doc(char* qname, char data[][200]);
void change_password(int role, char* old_p, char* new_p, char *new_pa);
char* get_search_by_name_data(char* name);
void fill_doc_name_search_file(char* name);
void fill_pque_tbox();
void fill_pque_tbox_over_network();
char *fetch_tb_data();
void set_pinfo(char* );
#endif

