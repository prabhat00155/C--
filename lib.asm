

put macro val
        local en1,en2
        push ax
        push cx
        push dx
        mov al,val
        mov cl,100
        cmp al,cl
        jb en2
        mov ah,00h
        div cl
        mov dh,ah
        add al,30h
        mov ah,02
        mov dl,al
        int 21h
        mov al,dh
        en2:
        mov cl,10
        cmp al,cl
        jb en1
        mov ah,00h
        div cl
        mov dh,ah
        add al,30h
        mov ah,02
        mov dl,al
        int 21h
        mov al,dh
        en1:
        add al,30h
        mov ah,02
        mov dl,al
        int 21h
        putln
        pop dx
        pop cx
        pop ax
endm


putln macro
        push ax
        push dx
        mov al,10d
        mov ah,02
        mov dl,al
        int 21h
        mov al,13d
        mov ah,02
        mov dl,al
        int 21h
        pop dx
        pop ax
endm



get macro
        mov ah,01
        int 21h
        sub al,30h
        putln
endm


