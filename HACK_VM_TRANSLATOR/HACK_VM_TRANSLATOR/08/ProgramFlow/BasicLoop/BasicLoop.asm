// push constant 0    
@0
D=A
@SP
M=M+1
A=M-1
M=D
// pop local 0         
@0
D=A
@LCL
D=D+M
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// label LOOP_START
($LOOP_START)
// push argument 0    
@ARG
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
// add
@SP
MD=M-1
@R13
AM=D
D=M
@R13
AM=M-1
M=D+M
// pop local 0	        
@0
D=A
@LCL
D=D+M
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
// push constant 1
@1
D=A
@SP
M=M+1
A=M-1
M=D
// sub
@SP
MD=M-1
@R13
AM=D
D=M
@R13
AM=M-1
M=M-D
// pop argument 0      
@0
D=A
@ARG
D=D+M
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
// if-goto LOOP_START  
@SP
AM=M-1
D=M
@$LOOP_START
D;JNE
// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
