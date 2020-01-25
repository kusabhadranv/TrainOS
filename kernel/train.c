
#include <kernel.h>

int zamboni_ticks=15;  									//Ticks to detect
int pause_ticks=10;									//the Zamboni
int xquick_ticks=2;
int stop_ticks=10;
int config4_zambonimeet_ticks=10;							//Ticks after meeting wagon in config4
int config4_last_ticks=2;
int zamboni_present=0;									//Ticks after reaching end point in cofig4
	

COM_Message send_command(char*, char*, int);

// **************************
// run the train application
// **************************

void clear_s88buffer(){
	send_command("R\015","",0);
}


COM_Message send_command(char* out_buf, char* in_buf, int in_buf_len){			//Function sending messages to COM1
	
	COM_Message msg;
	msg.input_buffer=in_buf;
	msg.len_input_buffer=in_buf_len;
	msg.output_buffer=out_buf;
	send(com_port,&msg);
	return msg;
}

void init_tracks(int window_id){						//Initialize tracks to keep Zamboni in outer loop
	wm_print(window_id,"Initializing tracks...\n");
	send_command("M4G\015","",0);
	send_command("M1G\015","",0);
	send_command("M9R\015","",0);
	send_command("M8G\015","",0);
	send_command("M5G\015","",0);
	wm_print(window_id,"Tracks initialized\n");
}

void check_zamboni(int window_id){						//Function to detect Zamboni
	
	wm_print(window_id,"Looking for Zamboni \n");
	int i=0;
	while(i<10){
		char track7_res[3];
		char track3_res[3];
		clear_s88buffer();
		sleep(pause_ticks);	
		send_command("C7\015",track7_res,3);
		if(track7_res[1]=='1'){
			zamboni_present=1;			
			break;
		}
		else{
			sleep(zamboni_ticks);
		}
		clear_s88buffer();
		sleep(pause_ticks);	
		send_command("C3\015",track3_res,3);
		if(track3_res[1]=='1'){
			zamboni_present=1;			
			break;
		}
		else{
			sleep(zamboni_ticks);
		}
		i++;
	}
	if(zamboni_present==1){
		wm_print(window_id,"Zamboni detected\n");
	}
	else{
		wm_print(window_id,"No Zamboni detected\n");
	}
}

int identify_config(int window_id){						//Function to identify the configuration
	wm_print(window_id,"Detecting configuration...\n");
	char track5_resp[3];
	char track11_resp[3];
	char track8_resp[3];
	sleep(xquick_ticks);
	clear_s88buffer();
	sleep(xquick_ticks);	
	send_command("C5\015",track5_resp,3);
	if(track5_resp[1]=='1') return 1;
	sleep(xquick_ticks);
	clear_s88buffer();
	sleep(xquick_ticks);
	send_command("C11\015",track11_resp,3);
	if(track11_resp[1] == '1') return 2;	
	sleep(xquick_ticks);
	clear_s88buffer();
	sleep(xquick_ticks);
	send_command("C8\015",track8_resp,3);
	if(track8_resp[1]=='1') return 4;
	return 3;
	
}

void probe_track(char* command,char result){					//Function to probe track
	char track_resp[3];	
	while(1){
		sleep(xquick_ticks);
		clear_s88buffer();
		send_command(command,track_resp,3);
		if(track_resp[1]==result) break;
	}

}


void process_config1(int window_id){						//Processing configuration 1
	if(zamboni_present){
		send_command("M3R\015","",0);
		sleep(xquick_ticks);
		send_command("M6G\015","",0);
		sleep(xquick_ticks);
		send_command("M7G\015","",0);
		sleep(xquick_ticks);
		send_command("M2R\015","",0);
		sleep(xquick_ticks);
		probe_track("C7\015",'1');
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		probe_track("C10\015",'1');
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("M5R\015","",0);
		probe_track("C7\015",'1');
		sleep(xquick_ticks);
		send_command("M4G\015","",0);
		probe_track("C7\015",'0');
		sleep(xquick_ticks);
		send_command("M5G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		probe_track("C4\015",'1');
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		probe_track("C12\015",'0');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C12\015",'1');
		sleep(xquick_ticks);
		send_command("M1G\015","",0);
		probe_track("C12\015",'0');	
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);	
		probe_track("C10\015",'1');
		probe_track("C10\015",'0');
		sleep(xquick_ticks);
		send_command("M5R\015","",0);	
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);	
		probe_track("C7\015",'1');
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		probe_track("C6\015",'1');
		sleep(xquick_ticks);
		send_command("M5G\015","",0);
		probe_track("C5\015",'1');
		sleep(xquick_ticks);
		send_command("M4G\015","",0);
		sleep(stop_ticks);
		send_command("L20S0\015","",0);	
			

	}
	else{
		send_command("M3R\015","",0);
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		sleep(xquick_ticks);
		send_command("M5R\015","",0);
		sleep(xquick_ticks);
		send_command("M6G\015","",0);
		sleep(xquick_ticks);
		send_command("M7G\015","",0);
		sleep(xquick_ticks);
		send_command("M2R\015","",0);
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		sleep(xquick_ticks);
		send_command("M9R\015","",0);
		sleep(xquick_ticks);
		send_command("M8G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C14\015",'1');		
		sleep(xquick_ticks);		
		send_command("M5G\015","",0);	
		probe_track("C5\015",'1');		
		sleep(xquick_ticks);		
		send_command("L20S0\015","",0);	
	
	}
	
	
}

void process_config2(int window_id){						//Processing configuration 2
	if(zamboni_present){
		send_command("M7R\015","",0);
		sleep(xquick_ticks);
		send_command("M2R\015","",0);
		sleep(xquick_ticks);
		send_command("M3G\015","",0);
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C14\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		sleep(xquick_ticks);
		send_command("M2G\015","",0);	
		probe_track("C1\015",'1');	
		sleep(xquick_ticks);
		send_command("M1G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		probe_track("C7\015",'1');
		probe_track("C3\015",'1');
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);	
		probe_track("C7\015",'1');		
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(stop_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C2\015",'1');		
		send_command("M4G\015","",0);
		probe_track("C1\015",'1');		
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		probe_track("C10\015",'1');		
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		sleep(xquick_ticks);
		send_command("M8R\015","",0);
		probe_track("C14\015",'1');		
		sleep(xquick_ticks);
		send_command("M1G\015","",0);
		sleep(xquick_ticks);
		probe_track("C11\015",'1');
		sleep(stop_ticks);
		send_command("M8G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
	}
	else{
		send_command("M7R\015","",0);
		sleep(xquick_ticks);
		send_command("M2R\015","",0);
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		sleep(xquick_ticks);
		send_command("M9R\015","",0);
		sleep(xquick_ticks);
		send_command("M8G\015","",0);
		sleep(xquick_ticks);
		send_command("M5G\015","",0);
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		sleep(xquick_ticks);
		send_command("M3G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C7\015",'1');
		sleep(xquick_ticks);
		send_command("M2G\015","",0);
		sleep(xquick_ticks);
		send_command("M8R\015","",0);
		probe_track("C11\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
	}
}

void process_config3(int window_id){						//Processing configuration 3
	if(zamboni_present){
		sleep(xquick_ticks);
		send_command("M3G\015","",0);
		sleep(xquick_ticks);
		send_command("M2G\015","",0);
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("M9G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C10\015",'1');
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		sleep(xquick_ticks);
		send_command("M9R\015","",0);
		sleep(xquick_ticks);	
		send_command("M1R\015","",0);
		sleep(xquick_ticks);
		probe_track("C1\015",'1');
		sleep(xquick_ticks);
		send_command("M1G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		probe_track("C3\015",'1');
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C7\015",'1');	
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(stop_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C2\015",'1');
		sleep(xquick_ticks);
		send_command("M4G\015","",0);
		sleep(xquick_ticks);
		probe_track("C1\015",'1');
		send_command("L20S0\015","",0);
		probe_track("C14\015",'1');
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		probe_track("C10\015",'1');
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("M9G\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C16\015",'1');
		sleep(stop_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("M9R\015","",0);
	}
	else{
		send_command("M9G\015","",0);
		sleep(xquick_ticks);
		send_command("M8G\015","",0);
		sleep(xquick_ticks);
		send_command("M5G\015","",0);
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		sleep(xquick_ticks);
		send_command("M3G\015","",0);
		sleep(xquick_ticks);
		send_command("M2G\015","",0);
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C7\015",'1');
		send_command("M9R\015","",0);
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("M9G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C16\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
	}
}


void process_config4(int window_id){						//Processing configuration 4
	if(zamboni_present){
	
		send_command("M2G\015","",0);
		sleep(xquick_ticks);
		send_command("M3G\015","",0);
		sleep(xquick_ticks);
		send_command("M6R\015","",0);
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("M9G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C10\015",'1');
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		sleep(xquick_ticks);
		send_command("M9R\015","",0);
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		probe_track("C1\015",'1');
		sleep(xquick_ticks);
		send_command("M1G\015","",0);
		probe_track("C2\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		probe_track("C14\015",'1');
		probe_track("C4\015",'1');
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		sleep(xquick_ticks);
		send_command("M5R\015","",0);
		probe_track("C7\015",'1');
		probe_track("C7\015",'0');
		sleep(xquick_ticks);
		send_command("M5G\015","",0);
		sleep(xquick_ticks);
		send_command("M4G\015","",0);
		sleep(config4_zambonimeet_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		probe_track("C6\015",'1');
		sleep(xquick_ticks);
		send_command("M5R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C3\015",'1');
		sleep(xquick_ticks);
		send_command("M4R\015","",0);
		probe_track("C6\015",'1');
		sleep(xquick_ticks);
		send_command("M5G\015","",0);
		probe_track("C1\015",'1');	
		sleep(xquick_ticks);
		send_command("M4G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		probe_track("C14\015",'1');
		sleep(xquick_ticks);
		send_command("M1R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C10\015",'1');
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(stop_ticks);
		send_command("L20S5\015","",0);
		sleep(xquick_ticks);
		send_command("M9G\015","",0);
		probe_track("C16\015",'1');
		sleep(xquick_ticks);
		sleep(config4_last_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("M9R\015","",0);
	}
	else{
		send_command("M9G\015","",0);
		sleep(xquick_ticks);
		send_command("M6R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C6\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("M5R\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C7\015",'1');
		probe_track("C7\015",'0');
		sleep(config4_zambonimeet_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C4\015",'1');
		sleep(xquick_ticks);
		send_command("M9R\015","",0);
		probe_track("C13\015",'1');
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
		sleep(xquick_ticks);
		send_command("L20D\015","",0);
		sleep(xquick_ticks);
		send_command("M9G\015","",0);
		sleep(xquick_ticks);
		send_command("L20S5\015","",0);
		probe_track("C16\015",'1');		
		sleep(xquick_ticks);
		send_command("L20S0\015","",0);
	}

}
void train_process(PROCESS self, PARAM param)					//Train process
{
	int window_id = wm_create(10,6,70,15);
	int config_id;
	wm_print(window_id,"                         Welcome to TOS\n");
	wm_print(window_id,"                         --------------\n");

	init_tracks(window_id);
	check_zamboni(window_id);
	config_id=identify_config(window_id);
	wm_print(window_id,"Configuration %d detected\n",config_id);
	wm_print(window_id,"Probing tracks in every %d ticks \n",xquick_ticks);
	wm_print(window_id,"Trying to retrieve wagon\n");

	switch(config_id){
		case(1):
			process_config1(window_id);
			break;
		case(2):
			process_config2(window_id);
			break;
		case(3):
			process_config3(window_id);
			break;
		case(4):
			wm_print(window_id,"Will wait for %d ticks to dock the wagon\n",config4_zambonimeet_ticks);
			wm_print(window_id,"Will wait for %d ticks before stopping\n",config4_last_ticks);
			process_config4(window_id);
			break;
			
	}
	wm_print(window_id,"Process ended. Becoming Zombie!!");
	become_zombie();
	while(1);

}




void init_train()							//Initalize function to create train process
{
	create_process(train_process,5,0,"Train process");
	resign();
}

/*	Below two functions were written only for the train demo. 
	display_trainvars - Displays the various ticks configured
	change_values     - Function to change these ticks through TOS 
	Shell.c was amended to included these function 
*/

void display_trainvars(int window_id){
	wm_print(window_id,"xquick_ticks = %d(2)\n",xquick_ticks);
	wm_print(window_id,"stop_ticks = %d\n",stop_ticks);
	wm_print(window_id,"config4_zambonimeet_ticks = %d(8)\n",config4_zambonimeet_ticks);
	wm_print(window_id,"config4_last_ticks = %d(2)\n",config4_last_ticks);
			
}

void change_values(int window_id,int value1, int value2, int value3){
	xquick_ticks=value1;
	config4_zambonimeet_ticks=value2;
	config4_last_ticks=value3;

}

