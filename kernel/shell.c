//VAISAKH KUSABHADRAN
//STUDENT ID : 920442702

#include <kernel.h>

struct command_list{						//Linked list to store the commands history	
	char command[50];			
	struct command_list* next;
};

struct con_cmd_list{						//Concatenated command list
	char* con_cmd;						//structure to handle multiple commands in one line	
	int con_cmd_len;					//Eg:ps;clear					
};

struct cmd_info{						//Structure to store the address of con_cmd_list structure
	struct con_cmd_list* cmd_addr;				//and the number of commands in that structure
	int cmd_count;		
};

void process_command(int,char*,struct command_list*);
void echo(int,char*);
void ps(int);
void helpcmd(int);
void histcmd(int,struct command_list*,char*);
int checkdigit(char*);

int shell_x=7;							//Variables to handle the new window position
int shell_y=3;


struct cmd_info parse_command(int window_id,char* command){	//Parses the command to handle multiple commands in one line
	struct cmd_info cmd_struct;
	struct con_cmd_list ccl[20];
	int ccl_ind=0;
	char* cmd=command;
	char* fptr;
	char* sptr;
	int trav_ccl_ind=0;
	int cmd_len=0;
	while(*cmd==' '){
		cmd++;	
	}
	fptr=cmd;
	sptr=cmd;
	while(*fptr!='\0'){
		sptr=fptr;		
		while(*sptr!='\0' && *sptr!=';'){
			sptr++;		
		}
		ccl[ccl_ind].con_cmd=fptr;
		ccl[ccl_ind].con_cmd_len=(int)(sptr-fptr);
		ccl_ind++;
		while(*(sptr+1)==' '){
			sptr++;		
		}	
		if(*sptr=='\0'){
			fptr=sptr;		
		}
		else{
			fptr=sptr+1;		
		}
	}

	while(trav_ccl_ind<ccl_ind){
		cmd_len=ccl[trav_ccl_ind].con_cmd_len;
		ccl[trav_ccl_ind].con_cmd[cmd_len]='\0';
		trav_ccl_ind++;
	}
	cmd_struct.cmd_addr=ccl;
	cmd_struct.cmd_count=ccl_ind;
	return cmd_struct;

}

void shell_process()						//Shell process which handles each key stroke
{
	struct cmd_info cmd_info_ptr;
	struct command_list *head=NULL;
	struct command_list *prev=NULL;
	struct command_list *temp=NULL;

	int window_id=wm_create(shell_x,shell_y,70,15);
	if(shell_x < 25){ 	
		shell_x+=3;
		shell_y+=3;
	}
	else{
		shell_x=7;
		shell_y=3;
	}
	wm_print(window_id,">>");	
	int index=0;
	char command[50];
	while (1) {
		char ch = keyb_get_keystroke(window_id, TRUE);
		if(index < 50 || ch==13){		
			switch(ch){
				case '\b':
					if(index==0){
						continue;
					}
					else{
						index--;
					}
					wm_print(window_id,"%c",ch);
					break;
				case 13:
					command[index]='\0';
					wm_print(window_id,"\n");
					cmd_info_ptr=parse_command(window_id,command);
					int cmd_nos=0;						
					int total_cmd=cmd_info_ptr.cmd_count;			
					while(cmd_nos < total_cmd){
						char* lst_cmd=cmd_info_ptr.cmd_addr[cmd_nos].con_cmd;
				
						temp=malloc(sizeof(struct command_list));
						char* v=lst_cmd;
						int ind=0;
						while(*v!='\0'){
							temp->command[ind]=*v;					
							ind++;						
							v++;					
						}								
						temp->next=NULL;
						if(head==NULL){
							head=temp;				
						}
						else{
							prev->next=temp;					
						}
						prev=temp;	
						process_command(window_id,lst_cmd,head);					
						cmd_nos++;
					}
					for(int j=0;j<index;j++){
						command[j]=' ';
					} 
					index=0;
									
					break;
				default:
					command[index]=ch;				
					wm_print(window_id, "%c", ch);
					index++;
					break;
			}
			
		}
		else{
			if(ch=='\b'){
				index--;
				wm_print(window_id,"%c",ch);
			}		
		}
	}
}

BOOL command_validate(int window_id,char* cmd1,char* cmd2){	//Function to validate the commands
	//wm_print(window_id,"reached command\n");
	char *command=cmd1;
	char first_char;
	char echo[4]="echo";
	int i=0;
	BOOL is_echo=TRUE;
	first_char=*cmd1;
	while(i<4){
		if(*command!=echo[i]){
			is_echo=FALSE;
			break;
		}
		else{
		}
		command++;
		i++;
	}
	while(*cmd2!='\0'){
			if(*cmd1!=*cmd2) return FALSE;
			cmd1++;
			cmd2++;
		}
	if(is_echo || first_char=='!'){
		return TRUE;
	}
	else{
		while(*cmd1==' '){
			cmd1++;		
		}
		return *cmd1=='\0';	
	}
}

int command_select(int window_id,char* cmd){			//Function to choose the command
	if(command_validate(window_id,cmd,"clear"))  return 1;
	if(command_validate(window_id,cmd,"shell"))  return 2;
	if(command_validate(window_id,cmd,"pong"))   return 3;
	if(command_validate(window_id,cmd,"echo"))  return  4;
	if(command_validate(window_id,cmd,"ps"))     return 5;
	if(command_validate(window_id,cmd,"history"))return 6;
	if(command_validate(window_id,cmd,"!"))      return 7;
	if(command_validate(window_id,cmd,"about"))  return 8;
	if(command_validate(window_id,cmd,"help"))   return 9;
	if(command_validate(window_id,cmd,"train"))  return 10;
	return 0;

}

void history(int window_id,struct command_list* head){		//Function to display history
	int num=1;
	while(head!=NULL){
		wm_print(window_id,"%d ",num);
		wm_print(window_id,"%s",head->command);
		wm_print(window_id,"\n");
		num++;
		head=head->next;	
	}
}
void process_command(int window_id,char *cmd,struct command_list* head){	//Function to process each command
	int command_no = command_select(window_id,cmd);
	switch(command_no){
		case(0):
			wm_print(window_id,"%s : command not found\n",cmd);
			break;
		case(1):
			wm_clear(window_id);
			break;
		case(2):
			start_shell();
			break;
		case(3):
			start_pong();
			break;
		case(4):
			if(*(cmd+4)==' ' || *(cmd+4)=='\0'){
				echo(window_id,cmd+5);							
			}
			else{
				wm_print(window_id,"%s : command not found\n",cmd);	
			}			
			break;
		case(5):
			ps(window_id);
			break;
		case(6):
			history(window_id,head);
			break;
		case(7):
			histcmd(window_id,head,cmd);
			break;
		case(8):
			wm_print(window_id,"Vaisakh Kusabhadran\n");
			break;
		case(9):
			helpcmd(window_id);
			break;
		case(10):
			init_train();
			break;
	}
	if(command_no!=7){
		wm_print(window_id,">>");
	}	
}


void helpcmd(int window_id){							//Function for help command
	wm_print(window_id,"                    HELP\n");
   	wm_print(window_id,"--------------------------------------------------\n");
	wm_print(window_id,"help           List of all commands supported in TOS\n");
	wm_print(window_id,"clear          Clears the screen\n");
	wm_print(window_id,"shell          Launches another shell window\n");
	wm_print(window_id,"pong           Launches pong game\n");
	wm_print(window_id,"echo <message> Print message\n");
	wm_print(window_id,"ps             Prints out process table\n");
	wm_print(window_id,"history        Prints all commands typed into shell with number\n");
	wm_print(window_id,"!<number>      Executed the command corresponding to number\n");
	wm_print(window_id,"about          Information about this TOS\n");
}
void histcmd(int window_id,struct command_list* head,char* cmd){		//Function for !<number> command
	struct command_list* real_head=head;
	char* arg=cmd+1;
	if(checkdigit(arg)){
		int cmd_no=0;
		int dig=0;
		while(*arg!='\0'){
			dig=*arg-48;
			cmd_no=(cmd_no*10)+dig;
			arg++;
		}
		int count=1;
		while(head!=NULL){
			//wm_print(window_id,"inside while  ");
			if(count==cmd_no && head->next!=NULL){
				wm_print(window_id,">>%s\n",head->command);
				process_command(window_id,head->command,real_head);
				break;
			}
			count++;
			head=head->next;
		}
		if(cmd_no > count || cmd_no==0 || head->next!=NULL){
			wm_print(window_id,"No commands corresponding to %d\n>>",cmd_no);		
		}
	}
	else{
		wm_print(window_id,"Incorrect syntax. Use !<number>\n>>");
	}


}

int checkdigit(char* arg){							//Function to verify x is always number in !<x>
	while(*arg!='\0'){
		if((int)(*arg) > '9'  || (int)(*arg) < '0'){	
			return 0;		
		}
	arg++;	
	}
	return 1;
}

void echo(int window_id,char* text){						//Function to implement echo
	while(*text == ' ') 
	{
		text++;
	}	
	wm_print(window_id,"%s\n",text);
}

void start_shell()								//Starting shell process
{
	create_process(shell_process,6,0,"Shell Process");
	resign();

}

void ps_heading(int wnd)							//All below functions for ps command. Taken from
{										//process.c
    wm_print(wnd, "State           Active Prio Name\n");			
    wm_print(wnd, "------------------------------------------------\n");
}

void ps_process_details(int  wnd, PROCESS p)
{
    static const char *state[] = { "READY          ",
        "ZOMBIE         ",
        "SEND_BLOCKED   ",
        "REPLY_BLOCKED  ",
        "RECEIVE_BLOCKED",
        "MESSAGE_BLOCKED",
        "INTR_BLOCKED   "
    };
    if (!p->used) {
        wm_print(wnd, "PCB slot unused!\n");
        return;
    }
    /* State */
    wm_print(wnd, state[p->state]);
    /* Check for active_proc */
    if (p == active_proc)
        wm_print(wnd, " *      ");
    else
        wm_print(wnd, "        ");
    /* Priority */
    wm_print(wnd, "  %2d", p->priority);
    /* Name */
    wm_print(wnd, " %s\n", p->name);
}

void ps(int  wnd)
{
    int             i;
    PCB            *p = pcb;

    ps_heading(wnd);
    for (i = 0; i < MAX_PROCS; i++, p++) {
        if (!p->used)
            continue;
        ps_process_details(wnd, p);
    }
}


