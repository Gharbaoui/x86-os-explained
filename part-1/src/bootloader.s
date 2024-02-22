start:
  mov ax, 0x7c00
  mov ds, ax

  mov si, title_str
  hlt



title_str db 'bootloader', 0
times 510-($-$$) db 0
dw 0xaa55
