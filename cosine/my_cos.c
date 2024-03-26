/*
this program gets from the user a number which represents a radian.
first it does mod(radian, 2PI) beacuse cosine is cyclic by 2PI.
the program outputs two values.
the first one is the result of a power series calculation to a certian accurecy.
the second one is the result of cos from math.h library.
we assume the input is a single number.
*/

#include <stdio.h>
#include <math.h> /*pow, cos*/
#include <stdlib.h> /*fmod*/
#include "data.h" /*defines and prototypes*/

/*
main call input and output functions
*/
int main(){
	double radian = GetInput();
	PrintOutput(radian);
	return 0;
}

/*
gets the input from user
and calcs the mod of the radian with respect to 2PI
*/
double GetInput() {
	double radian;
	printf("Enter angle in radian\n");
	scanf("%lf\n", &radian);
	printf("You Entered: %.2f\n", radian);
	radian = fmod(radian, 2*PI); /*the period of the cosine function is 2 pi*/
	return radian;
}

/*
prints results from my_cos and cos
*/
void PrintOutput(double radian) {
	printf("my_cos returned: %f\n", my_cos(radian));
	printf("cos from math.h returned: %f\n", cos(radian));
}

/*
my_cos returns Power Series of cosine(val) to certian accuracy
*/
double my_cos(double radian){
	
	double sum = 0;/*init sum to 0*/
	int i = 0;/*start index of sum from 0*/
	double current = 1; /*first value in the sum is always 1*/
	
	/*loops until the accuracy test fails*/
	while(fabs(current) > LOW) { 
		
		/*update sum*/
		sum += current;
		
		/*increase i*/
		i++;

		current = CalculateTerm(i, radian);
	}
	return sum;
}

/*
CalculateTerm returns the i'th term in cosine power series with respect to the radian
*/
double CalculateTerm(int i, double radian) {
	return pow(-1,i) * pow(radian,2*i) / factorial(2*i);
}

/*
factorial returns n(n-1)(n-2)...
if num = 0 or 1 returns 1
*/
double factorial(int num) {
	double res = 1;
	/*stop at 1*/
	while(num > 1) {
		res *= num;
		num--;
	}
	return res;
}
