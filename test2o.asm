include lib.asm
data segment
x	dw	0
locvar	dw	12	dup(0)
data ends

code segment
assume cs:code,ds:data
sum2	proc
mov  bx,locvar[0]
mov ax,bx
mov  bx,locvar[1]
add bx,ax
ret
sum2	endp
sum3	proc
mov  bx,locvar[0]
mov ax,bx
mov  bx,locvar[1]
add bx,ax
mov ax,bx
mov  bx,locvar[2]
add bx,ax
ret
sum3	endp
start:
mov ax,data
mov ds,ax
mov dx,3
mov locvar[0],dx
mov dx,7
mov locvar[1],dx
call sum2
mov x,bx
mov cx,x
put cl
mov dx,4
mov locvar[0],dx
mov dx,5
mov locvar[1],dx
mov dx,7
mov locvar[2],dx
call sum3
mov x,bx
mov cx,x
put cl
mov ah,4ch
int 21h
code ends
end start
