;file kelet2.as
;macro spread

.define sz = 2
mcr EXP1
sub r1, r4
cmp K, #sz
jmp LOOP
endmcr
.entry DO
.extern LOOP
DO:mov STR[5], STR[2]
EXP1
STR:.string "abcdef"
K:.data 22,1,-8,-5,-7
