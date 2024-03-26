#include "mymat.h"

/*
This program get commmands from user and performs matrix calculations.

Matrices:
	matrix display is as follows,
	------------------------------------
	| (1,1) || (1,2) || (3,1) || (4,1) |
	| (2,1) || (2,2) || (3,2) || (4,2) |
	| (3,1) || (3,2) || (3,3) || (4,3) |
	| (4,1) || (4,2) || (4,3) || (4,4) |
	------------------------------------

Errors:
	The program checks the input from left to right,
	Meaning the leftmost error will alert the user.
	error display is as follows, ***error_type.

*/

int main(){
	int i;
	char line[MAX_LINE];
	char *pos = line;
	mat *mats[6];

	/*init*/
	init_line(pos);
	for(i=0; i<6; i++){
		mats[i] = (mat*)malloc(sizeof(mat));
		init_mat_zero(mats[i]);
	}
	
	/*input*/
	while(get_line(pos)){
		int len;
		if(!strlen(pos)) goto nextline;
		/*illegal_comma*/
		if(illegal_comma(pos)) goto nextline;
		/*command*/
		len = get_word_len(pos);
		pos = get_word(pos);
		if(cmd_check_name(pos, "stop", len)){
			if(cmd_stop(pos, len)) goto nextline;
		}
		else if(cmd_check_name(pos, "read_mat", len)){
			if(cmd_read_mat(pos, len, mats)) goto nextline;
		}
		else if(cmd_check_name(pos, "print_mat", len)){
			if(cmd_print_mat(pos, len, mats)) goto nextline;
		}
		else if(cmd_check_name(pos, "add_mat", len)){
			if(cmd_AddMulSub_mat(pos, len, mats, add_mat)) goto nextline;
		}
		else if(cmd_check_name(pos, "sub_mat", len)){
			if(cmd_AddMulSub_mat(pos, len, mats, sub_mat)) goto nextline;
		}
		else if(cmd_check_name(pos, "mul_mat", len)){
			if(cmd_AddMulSub_mat(pos, len, mats, mul_mat)) goto nextline;
		}
		else if(cmd_check_name(pos, "mul_scalar", len)){
			if(cmd_scalar_mat(pos, len, mats)) goto nextline;
		}
		else if(cmd_check_name(pos, "trans_mat", len)){
			if(cmd_trans_mat(pos, len, mats)) goto nextline;
		}
		else {
			char *k;
			printf("\nUndefined command name");
			for(k=pos; k < pos+len; k++) printf("%c", *k);
		}
		nextline:
		pos = line; 
		init_line(pos);
	}
	printf("\n");
	return 1;
}




