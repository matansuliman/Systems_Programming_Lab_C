#define LOW pow(10, -6) /*the desired accurecy of the result*/

#define PI 3.14159265358979323846 /*value of PI*/

double GetInput();/*input: none, output: radian between 0 and 2PI*/

void PrintOutput(double radian);/*input: radian, output: result of my_cos and cos with respect to radian*/

double my_cos(double radian);/*input: radian, output: result of power series of consine with radian to LOW accuracy*/

double CalculateTerm(int i, double radian);/*input: i'th position and radian, output: i'th term in cosine power series with radian*/

double factorial(int num);/*input: integer, output: factorial of number*/
