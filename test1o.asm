include lib.asm
data segment
h	dw	0
locvar	dw	12	dup(0)
data ends

code segment
assume cs:code,ds:data
maxi2	proc
mov  bx,locvar[0]
mov ax,bx
mov  bx,locvar[1]
mov ah,0
mov bh,0
cmp ax,bx
jbe L0
mov  bx,locvar[0]
mov locvar[2],bx
jmp L1
L0:
mov  bx,locvar[1]
mov locvar[2],bx
L1:
mov  bx,locvar[2]
ret
maxi2	endp
maxi3	proc
mov  bx,locvar[0]
mov ax,bx
mov  bx,locvar[1]
mov ah,0
mov bh,0
cmp ax,bx
jbe L2
mov  bx,locvar[0]
mov ax,bx
mov  bx,locvar[2]
mov ah,0
mov bh,0
cmp ax,bx
jbe L21
mov  bx,locvar[0]
mov locvar[3],bx
jmp L22
L21:
mov  bx,locvar[2]
mov locvar[3],bx
L22:
jmp L3
L2:
mov  bx,locvar[1]
mov ax,bx
mov  bx,locvar[2]
mov ah,0
mov bh,0
cmp ax,bx
jbe L33
mov  bx,locvar[1]
mov locvar[3],bx
jmp L34
L33:
mov  bx,locvar[2]
mov locvar[3],bx
L34:
L3:
mov  bx,locvar[3]
ret
maxi3	endp
start:
mov ax,data
mov ds,ax
push locvar[0]
mov dx,2
mov locvar[0],dx
mov dx,3
mov locvar[1],dx
call maxi2
pop locvar[0]
mov locvar[0],bx
mov cx,locvar[0]
put cl
push locvar[0]
mov dx,2
mov locvar[0],dx
mov dx,4
mov locvar[1],dx
mov dx,3
mov locvar[2],dx
call maxi3
pop locvar[0]
mov locvar[0],bx
mov cx,locvar[0]
put cl
mov ah,4ch
int 21h
code ends
end start
