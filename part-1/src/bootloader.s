start:
  mov ax, 0x07c0
  mov ds, ax

  mov si, title_str
  call print_str


  mov si, test_str
  call print_str
  hlt



print_str:
  mov ah, 0x0e
.print_char:
  lodsb

  cmp al, 0x0
  je .print_done

  int 0x10
  jmp .print_char

.print_done:
  mov al, 0xa
  int 0x10

  mov ah, 0x3
  int 0x10
  
  mov ah, 0x02
  mov dl, 0x0
  mov bh, 0
  int 0x10

  ret



title_str db 'bootloader started', 0
test_str db 'test string'
times 510-($-$$) db 0
dw 0xaa55
