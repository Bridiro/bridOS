print: 
    pusha
    mov $0xe, %ah

print_loop:
    lodsb
    cmp $0, %al
    je done

    int $0x10
    jmp print_loop

done:
    popa
    ret

print_nl:
    pusha

    mov $0x0e, %ah
    mov $0x0a, %al
    int $0x10
    mov $0x0d, %al
    int $0x10

    popa
    ret
