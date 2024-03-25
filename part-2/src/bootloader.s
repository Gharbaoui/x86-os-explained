start:
  mov ax, 0x07c0
  mov ds, ax

  mov si, title_str
  call print_str
  call load_kernel

  mov si, kernel_loaded_str
  call print_str


  jmp 0x0800:0x0

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
  mov ax, [curr_sector_to_load]
  sub ax, 0x2
  mov bx, 512d
  mul bx ; will store into ax
  mov bx, ax

  mov ax, 0x0800
  mov es, ax

  mov ah, 0x2
  mov al, 0x1 ; how many sectors to read
  mov ch, 0x0 ; 
  mov cl, [curr_sector_to_load] ; which sector to read
  mov dh, 0x0
  mov dl, 0x80

  int 0x13

  jc could_not_load_kernel

  sub byte [number_of_sectors_to_load], 1
  add byte [curr_sector_to_load], 1

  cmp byte [number_of_sectors_to_load], 0

  jne load_kernel

  ret

could_not_load_kernel:
  mov si, failed_load
  call print_str
  hlt

title_str db 'bootloader started', 0
failed_load db 'we could not load your kernel', 0
kernel_loaded_str db 'your kernel now is stored in RAM', 0 
number_of_sectors_to_load db 10d
curr_sector_to_load db 2d
times 510-($-$$) db 0
dw 0xaa55
