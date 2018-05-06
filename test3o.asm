include lib.asm
data segment
m	dw	0
locvar	dw	12	dup(0)
data ends

code segment
assume cs:code,ds:data
fact1	proc
mov  bx,locvar[0]
mov ax,bx
mov bx,0
mov ah,0
mov bh,0
cmp ax,bx
jne L0
mov bx,1
ret
jmp L1
L0:
L1:
push locvar[0]
mov  bx,locvar[0]
mov ax,bx
mov bx,1
sub ax,bx
mov bx,ax
mov locvar[0],bx
call fact1
pop locvar[0]
mov ax,bx
mov  bx,locvar[0]
mul bx
mov bx,ax
mov m,bx
mov bx,m
ret
fact1	endp
start:
mov ax,data
mov ds,ax
get
mov bl,al
mov bh,0
mov m,bx
mov dx,m
mov locvar[0],dx
call fact1
mov m,bx
mov cx,m
put cl
mov ah,4ch
int 21h
code ends
end start
