bits 16
extern kernel_entry


start:
  mov ax, cs
  mov ds, ax

  mov si, starter_str
  sub si, start
  call print_str

  call load_gdt
  call init_video_mode
  call switch_to_protected_mode
  call setup_interrupts

  call 0x8:start_kernel


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

setup_interrupts:
  call remap_pic
  call load_idt
  ret

remap_pic:
  mov al, 0x11

  ; init master pic
  out 0x20, al
  ; init slave pic
  out  0xa0, al

  mov al, 32
  ;telling the master that you should start at 32
  out 0x21, al

  mov al, 40
  ;telling the master that you should start at 40
  ; why 40 you may ask? well 8 for the master so its normal that we use 40

  out 0xa1, al

  mov al, 0x4
  ; telling the pic where slave is connected
  ; but should not be 0x2 instead of 0x4 well its. its just different encoding
  ; 0x4 = 100 what is happening is that we should set the right bit which is bit at index 2
  out 0x21, al

  mov al, 0x2
  ; telling the slave where the master is connected with you
  out 0xa1, al

  ; telling the master and slave that we are working with x86
  mov al, 0x1
  out 0x21, al
  out 0xa1, al

  ; enable all irqs
  mov al, 0x0
  out 0x21, al
  out 0xa1, al

  ret

load_idt:
  lidt [idtr - start]
  ret

load_gdt:
  cli
  lgdt [gdtr - start]
  ret


switch_to_protected_mode:
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  ret

init_video_mode:
  mov ah, 0x0
  mov al, 0x03
  int 0x10

  mov ah, 0x01
  mov cx, 0x2000
  int 0x10

  ret


bits 32
start_kernel:
  mov eax, 0x10
  mov ds, eax
  mov ss, eax

  mov eax, 0x0
  mov es, eax
  mov fs, eax
  mov gs, eax

  sti

  call kernel_entry

starter_str db 'we are in starter code', 0
%include 'gdt.s'
%include `idt.s`
