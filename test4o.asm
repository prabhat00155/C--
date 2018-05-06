include lib.asm
data segment
locvar	dw	12	dup(0)
data ends

code segment
assume cs:code,ds:data
start:
mov ax,data
mov ds,ax
get
mov bl,al
mov bh,0
mov locvar[0],bx
mov bx,1
mov locvar[1],bx
L0:
mov  bx,locvar[0]
mov ax,bx
mov bx,0
mov ah,0
mov bh,0
cmp ax,bx
jbe L1
mov  bx,locvar[1]
mov ax,bx
mov  bx,locvar[0]
mul bx
mov bx,ax
mov locvar[1],bx
mov  bx,locvar[0]
mov ax,bx
mov bx,1
sub ax,bx
mov bx,ax
mov locvar[0],bx
jmp L0
L1:
mov cx,locvar[1]
put cl
mov ah,4ch
int 21h
code ends
end start
