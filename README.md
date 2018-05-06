# Compiler for C--, a subset of C

main.c is the program that includes all the files that have been used in
designing the compiler. This is the file that is compiled with C-compiler
as shown below.<br/>
$ gcc main.c

The above will generate an executable file, a.out, which
corresponds to our compiler. This file is executed at the command
line along with a command line argument, which is the name of the
file that is to be executed, as shown below.<br/>
$ ./a.out test1.c<br/>

This generates an assembly code as its output. We can save
this assembly code by redirecting the output to a file.<br/>
$ ./a.out test1.c > test1o.asm<br/>

Now, this assembly code can be simulated by MASM assembler,
using the file lib.asm for various input/output
operations, to get the final output.<br/>
$ masm test1o.asm<br/>
$ link test1o.obj<br/>
$ test1o<br/>

