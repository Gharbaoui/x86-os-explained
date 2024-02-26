start:
  mov ax, 0x07c0
  mov ds, ax

  mov si, title_str
  call print_str
  call load_kernel

  jmp 0x0800:0x0
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

load_kernel:
  mov ax, 0x0800
  mov es, ax

  mov ah, 0x2
  mov al, 0x1
  mov ch, 0x0
  mov cl, 0x2
  mov dh, 0x0
  mov dl, 0x80
  mov bx, 0x0

  int 0x13

  jc could_not_load_kernel

  ret

could_not_load_kernel:
  mov si, failed_load
  call print_str
  hlt

title_str db 'bootloader started', 0
failed_load db 'we could load your kernel', 0
times 510-($-$$) db 0
dw 0xaa55
