#include "mymat.h"

/*
This file contain calculation function
*/

void copy_mat(mat *source, mat *des){
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			des->arr[i][j] = source->arr[i][j];
		}
	}
}

void add_mat(mat *a, mat *b, mat *c){
	mat *res = malloc(sizeof(mat));
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			res->arr[i][j] = a->arr[i][j] + b->arr[i][j];
		}
	}
	copy_mat(res, c);
}

void sub_mat(mat *a, mat *b, mat *c){
	mat *res = malloc(sizeof(mat));
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			res->arr[i][j] = a->arr[i][j] - b->arr[i][j];
		}
	}
	copy_mat(res, c);
}

void mul_mat(mat *a, mat *b, mat *c){
	mat *res = malloc(sizeof(mat));
	int i, j, k;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			res->arr[i][j] = 0;
			for(k=0; k<ROWS; k++){
				res->arr[i][j] += a->arr[i][k] * b->arr[k][j];
			}
		}
	}
	copy_mat(res, c);
}

void mul_scalar(mat *a, float b, mat *c){
	mat *res = malloc(sizeof(mat));
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			res->arr[i][j] = a->arr[i][j] * b;
		}
	}
	copy_mat(res, c);
}

void trans_mat(mat *a, mat *b){
	mat *res = malloc(sizeof(mat));
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			res->arr[j][i] = a->arr[i][j];
		}
	}
	copy_mat(res, b);
}





