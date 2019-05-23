#ifndef WR_REPORT_FILE
#define WR_REPORT_FILE 1
void wr_report_file(char *data[][200],newtComponent tab[][9], unsigned id, int upto);
int fill_report(char data[][200], unsigned id);
char* get_line(int len, char type);
void set_report(unsigned id);
#endif
