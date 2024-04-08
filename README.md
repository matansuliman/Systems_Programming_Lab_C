# Systems_Programming_Lab_C
third project in a systems programming laboratory.

# main_mat, mymat:
* iteractive calulator fror matrices.
* there are 7 matrices defined: MAT_A, MAT_B, ..., MAT_F.
* they are 4x4 and can hold real numbers as values.
* this calculator can perform these operations:
    1. read_mat M_dest val_1, val_2, val_3, ... --> inset this values by columns and rows into M_dest
    2. print_mat M_source --> print(M_1) in 2D
    3. add_mat M_1, M_2, M_dest --> M_dest = M_1 + M_2
    4. sub_mat M_1, M_2, M_dest --> M_dest = M_1 - M_2
    5. mul_mat M_1, M_2, M_dest --> M_dest = M_1 * M_2
    6. muk_scalar M_1, INT_scalar, M_dest --> M_dest = M_1 * INT_scalar
    7. trans_mat M_source, M_dest --> M_dest = transpose(M_source)
    8. stop --> terminate the program

## input:
* this program gets lines of commands of the types we seen from stdin(keyboard/file).
* each line has a single command.
* between two arguments there is a single comma.
* there can be tabs/spaces in the start/end of the line.
* command names are in lowercase, arguments are in uppercase.

## output:
* this program performs the commands it's given and prints when it's told to.
