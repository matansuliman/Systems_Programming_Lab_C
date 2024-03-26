#include "mymat.h"

/*
This file contain helping functions
*/

void init_line(char *s){
	char *e;
	int i;
	for(e=s, i=0; i<MAX_LINE ; i++, e++) e[0] = '\0';
}


void init_mat_zero(mat *x){
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			(*x).arr[i][j] = 0;
		}
	}
}


int get_line(char *s){
	int c, i;
	printf("\nEnter new command:");
	/*stop conditions (OR):
		1)max line length excceded
		2)EOF
		3)\n meaning new line encounter	
	*/
	for (i=0; i<MAX_LINE-1 && ((c=getchar()) != EOF) && (c!= '\n'); ++i) {
		s[i] = c;/*store input*/
		printf("%c", c);/*print input*/
	}
	if(c == EOF){
		printf("\n\n*** EOF ***\n");
		return 0;
	}
	return 1;
}


char *get_word(char *s){
	char *e;
	s = skip_spaces(s);
	for(e = s; IS_VALID(*e); e++) ;
	return s;

}

int get_word_len(char *s){
	char *e;
	s = skip_spaces(s);
	for(e = s; IS_VALID(*e); e++) ;
	return e-s;

}

char *skip_spaces(char *s){
	char *p;
	for(p = s; IS_BLANK(*p); p++) ;
	return p;
}


void print_mat(mat *x){
	int i, j;
	printf("\n------------------------------------\n");
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			printf("|%7.2f|", (*x).arr[i][j]);
		}
		printf("\n");
	}
	printf("------------------------------------");
}



int matrix_nametonum(char *s, int len){
	if(!len && *s != ',') return missing_argument();
	if(*s == ',') return missing_argument();
	if(len != 5) return undefined_matrix(s, len);
	if(!strncmp(s, "MAT_A", len)) return 1;
	if(!strncmp(s, "MAT_B", len)) return 2;
	if(!strncmp(s, "MAT_C", len)) return 3;
	if(!strncmp(s, "MAT_D", len)) return 4;
	if(!strncmp(s, "MAT_E", len)) return 5;
	if(!strncmp(s, "MAT_F", len)) return 6;
	return undefined_matrix(s, len);
}

int illegal_comma(char *p){
	if(*p == ','){
		printf("\n*** Illegal comma");
		return 1;
	}
	return 0;
}

int missing_comma(char *p){
	if(*p != ','){
		printf("\n*** missing comma");
		return 1;
	}
	return 0;
}

int missing_argument(void){
	printf("\n*** Missing argument");
	return 0;
}

int undefined_matrix(char *p, int len){
	char *i;
	printf("\n*** Undefined matrix name ");
	for(i=p; i < p+len; i++) printf("%c", *i);
	return 0;
}

double is_number(char *s, int len){
	char *p=s, *i;
	char **temp = &s;
	int count = 0;
	if(!len) return missing_argument();
	/*checks for a valid number*/
	if(!isdigit(*p) && *p != '-' && *p != '.') goto error;
	if(*p == '.') count++;
	for(p++; p < s+len; p++){
		if(*p == '.' && count) goto error;
		if(*p == '.') count++;
		if(!isdigit(*p) && *p != '.') goto error;
	}
	return 1;
	error:
	printf("\n*** Argument is not a scalar: ");
	for(i=*temp; i < *temp+len; i++) printf("%c", *i);
	return 0;

}

int extraneous_text(char *p){
	if(get_word_len(p)){
		printf("\n*** Extraneous text after end of command: %s", p);
		return 1;
	}
	return 0;
}


/*cmd*/


int cmd_check_name(char* pos, const char* str,int len){
	return (!strncmp(pos, str, len) && len == strlen(str));
}

int cmd_stop(char *pos, int len){
	/*extraneous_text*/
	pos = skip_spaces(pos += len);
	if(extraneous_text(pos)) goto error;
	printf("\nstopping...\n");
	exit(0);
	error:
	return 1;
}

int cmd_read_mat(char *pos, int len, mat *mats[]){
	int mat_num = 0, MAX_ARGS = 16, count, i, j;
	double scalar = 0;
	char *temp_scalar;
	mat* temp = (mat*)malloc(sizeof(mat));
	
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*mat*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_num = matrix_nametonum(pos, len))) goto error;
	/*args*/
	init_mat_zero(temp);
	count = i = j = 0;
	while(count < MAX_ARGS){
		pos = skip_spaces(pos += len);
		/*end of line*/
		if(!strlen(pos)) goto done;
		/*missing_comma*/
		if(missing_comma(pos++)) goto error;
		/*extraneous_text*/
		if(!strlen(pos)){
			printf("*** Extraneous text after end of command: ,");
			goto error;
		}
		/*illegal another comma*/
		pos = skip_spaces(pos);
		if(illegal_comma(pos)) goto error;
		/*scalar*/
		len = get_word_len(pos);
		pos = get_word(pos);
		if(!is_number(pos, len)) goto error;
		temp_scalar = strncpy(temp_scalar, pos, len);
		temp_scalar[len] = '\0';
		scalar = atof(temp_scalar);
		(*temp).arr[i][j] = scalar;
		count++;
		/*maintaining the i,j positions from 2d array*/
		j++;
		if(j == 4){
			j = 0;
			i++;
		}
		
	}
	done:
	copy_mat(temp, mats[mat_num-1]);
	return 0;
	error:
	return 1;
}

int cmd_print_mat(char *pos, int len, mat *mats[]){
	int mat_num = 0;
	
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*mat*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_num = matrix_nametonum(pos, len))) goto error;
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*extraneous_text*/
	if(extraneous_text(pos)) goto error;
	/*print_mat*/
	print_mat(mats[mat_num-1]);
	return 0;
	error:
	return 1;
}

int cmd_AddMulSub_mat(char* pos, int len, mat *mats[], void(*f)(mat * MAT_A, mat *, mat *)){
	int mat_1 = 0, mat_2 = 0, mat_3 = 0;
	
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*mat 1*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_1 = matrix_nametonum(pos, len))) goto error;
	/*missing_comma*/
	pos = skip_spaces(pos += len);
	if(missing_comma(pos++)) goto error;
	/*mat 2*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_2 = matrix_nametonum(pos, len))) goto error;
	/*missing_comma*/
	pos = skip_spaces(pos += len);
	if(missing_comma(pos++)) goto error;
	/*mat 3*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_3 = matrix_nametonum(pos, len))) goto error;
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*extraneous_text*/
	if(extraneous_text(pos)) goto error;
	/*add_mat*/
	f(mats[mat_1-1], mats[mat_2-1], mats[mat_3-1]);
	return 0;
	error:
	return 1;
}

int cmd_scalar_mat(char* pos, int len, mat *mats[]){
	int mat_1 = 0, mat_2 = 0;
	double scalar = 0;
	char *temp_scalar = (char *)malloc(sizeof(char)*100);
	
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*mat 1*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_1 = matrix_nametonum(pos, len))) goto error;
	/*missing_comma*/
	pos = skip_spaces(pos += len);
	if(missing_comma(pos++)) goto error;
	/*scalar*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!is_number(pos, len)) goto error;
	temp_scalar = strncpy(temp_scalar, pos, len);
	temp_scalar[len] = '\0';
	scalar = atof(temp_scalar);
	/*missing_comma*/
	pos = skip_spaces(pos += len);
	if(missing_comma(pos++)) goto error;
	/*mat 2*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_2 = matrix_nametonum(pos, len))) goto error;
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*extraneous_text*/
	if(extraneous_text(pos)) goto error;
	/*mul_scalar*/
	mul_scalar(mats[mat_1-1], scalar, mats[mat_2-1]);
	return 0;
	error:
	return 1;
}

int cmd_trans_mat(char* pos, int len, mat *mats[]){
	int mat_1 = 0, mat_2 = 0;
	
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*mat 1*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_1 = matrix_nametonum(pos, len))) goto error;
	/*missing_comma*/
	pos = skip_spaces(pos += len);
	if(missing_comma(pos++)) goto error;
	/*mat 2*/
	len = get_word_len(pos);
	pos = get_word(pos);
	if(!(mat_2 = matrix_nametonum(pos, len))) goto error;
	/*illegal_comma*/
	pos = skip_spaces(pos += len);
	if(illegal_comma(pos)) goto error;
	/*extraneous_text*/
	if(extraneous_text(pos)) goto error;
	/*trans_mat*/
	trans_mat(mats[mat_1-1], mats[mat_2-1]);
	return 0;
	error:
	return 1;
}

