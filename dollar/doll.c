/*
this program gets from the user a list of numbers.
the first one is the dollar value with reespect to shekel and the others are sums of money.
the program outputs a table with the sums in one column and the corresponding shekel values in another columns.
we assume the input is only numbers seperated by spaces and at least the dollar value.
*/

#include <stdio.h>
#include "data.h"

/*
main gets the dollar value from user and calls PrintTable.
*/
int main() {
   	float dollarValue;/*dollar value from input*/
    	scanf("%f", &dollarValue);
	PrintTable(dollarValue);
   	return 0;
}

/*
PrintTable loops through the input.
*/
void PrintTable(double dollarValue){
	float current; /*current number from input*/
	float sum = 0; /*init sum to 0*/

	PrintHeaderLine();

	/*print content of table as long as we get an input*/
    	while (scanf("%f", &current) == SUCCESS) {
		
		/*update sum*/
		sum += current; 
		
        	PrintLine(current, dollarValue);
    	}

	PrintLine(sum, dollarValue);
}

/*
PrintHeaderLine prints the header line of the table
*/
void PrintHeaderLine() {
	printf("%10s%10s\n", "$", "IS");
}

/*
PrintLine calcs the corresponding value and prints the table line
*/
void PrintLine(double val, double dollarValue) {
	double result = dollarValue * val;
	printf("%10.2f%10.2f\n", val, result);
}

