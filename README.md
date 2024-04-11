# C programming language

This is the Final project of a laboratory course and there are 4 other projects(in other branches).

* This project is an assembler made in c.
## Input:
* The input is an assembly code file for exp: file_name.as
## Output: 
consists of several files:
  1) file_name.am: the input file after macros spread 
  2) file_name.ob: consists of binary machine code translated to special encoded 4 letters base (!,#,%,*)
  3) file_name.ent: includes local labels from file_name.as
  4) file_name.ext: includes external labels from file_name.as

## Execution steps: :dart:
1) clone this repo to your computer
2) make sure you are in the project folder
3) run: make
4) run: ./driver.exe file_name1.as file_name2.as ...


## Common language: :abcd:
* register - a variable used by assembly language, in this program, we call them r0-r7
* operand - a data item(variable/number) operated on by an instruction.
* line - a single line from the input file
* label - can be defined in a line in the format 'label_name: line_content'
* macro - similar to a function in higher-level languages
* binary line - 14-bit binary data. a line can have multiple binary lines.
binary line structure : [13,12,11,10] [9,8,7,6] [5,4] [3,2] [1,0]
    1) [1,0] - A/R/E (Absolute/Relocatable/External)
    2) [3,2] - target operand addressing type (explanation shortly)
    3) [5,4] - source operand addressing type
    4) [9,8,7,6] - opcode
    5) [13,12,11,10] - not is user, set to 0 always
* operand addressing types:
    1) immediate - exp: #2, #-7 ...
    2) direct - suppose x is defined in the data segment, we can do 'dec x'
    3) constant - mov x[2],x[3]
    4) register - mov r1,r2
* binary code - all binary lines of all lines in order.
* command segment - the first segment in the object file, contains the binary code of the commands
* data segment - the second segment in the object file, contains the binary code of the data of the code
* IC - instruction count, used to index the binary lines of the command segment
* DC - data count, used to index the binary lines of the data segment
* symbol : (can be one of these)
    1) label defined by .entry/.extern
    2) an existing label in the file
    3) variable defined by .define
* symbols table - a table constructed in the first pass, contains all the symbols from the .am file

## further explanation:


## MUST have in the assembly input file: :skull_and_crossbones:
* The first command of the file has to have a label, typically 'MAIN'
* macro (if used)
    1) macro must be declared in the format: 'mcr NAME'
    2) macro name can't be a reserved name of assembly language
    3) macro must be closed by 'endmcr' statement
    4) macro declaration must be before any macro calling

## Take into consideration in the assembly input file: :warning:
(These following conditions will not end the running of the program but will result in errors therefore no output files will be created)
* line
    1) a line can be up to 80 letters long
    2) lines a are differentiable in the input file by '\n' (newline)
    3) a line should be correct in terms of assembly syntax
    4) The label of a line  can't be a reserved name of assembly language
* label
    1) can't have spaces between the label name and the ':' sign
    2) can't have multiple declarations of the same label
* Number
    1) can start with +/-
    2) only digits
* .define:
    1) a definition of a variable can be only for a number between -2048 and 2047
    2) variable name can't be a reserved name of assembly language
    3) can't define a label in .define line
* .data
    1) data can store a number between -8192 and 8191
* .entry
    1) a label declared in the file as .entry must exist in the file
* .extern
    1) a label declared in the file as .entry can't be declared as .extern

