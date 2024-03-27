SECTIONS
{
  .text 0x04000 :
  {
    code = .; _code = .; __code = .;
    *(.text)
  }

  .data :
  {
    data = .; _data = .; __data = .;
    *(.data)
    *(.rodata)
  }

  .bss :
  {
    bss = .; _bss = .; __bss = .;
  }

  end = .; _end = .; __end = .;
}
