start:
  mov ax, cs
  mov ds, ax

  mov si, kernel_welcome
  call print_str
  jmp $

print_str:
  mov ah, 0xE
.print_char:
  lodsb

  cmp al, 0x0
  je .print_done

  int 0x10

  jmp .print_char

.print_done:
  ret

kernel_welcome db 'welcome to our kernel', 0
