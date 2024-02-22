#### Part-1

this is not intended to explain all the details of the os, what I want is to make something easy to understand, is it going to be perfect? no, when I have the choice to make something good technically or understandable  I'll choose understandable this readme's are intended to be read while you are reading  the source code.

simplest os possible!!!

####  First
when a computer powers on, a piece of code named [Bootloader](#Bootloader) loaded and takes the control of the computer


###### Bootloader
>the goal of the bootloader is loading the [Kernel](#Kernel) of an os from disk to main memory

###### Kernel
>heart of our os


#### Apply
with  this info what I'm going to do is write a [Bootloader](#Bootloader) that will print something, and loads a simple [Kernel](#Kernel) that itself just print some other stuff


#### Questions
"I said computer loads bootloader from disk to ram"
1. which disk?
2. where does it end up in the ram?

>which disk well there's no specific disk for example some specialized disk, what really happens is that
the software that is trying to locate the bootloader is looking for some signature, to be exact it checks every disk and read the first sector of 512 bytes and if that sector has `0xaaff` that is the bootloader so what we need is to put our bootloader in the first sector (sector 0) and put the value `0xaa55` look [here](#signature)

>well it will endup in the location `0x7C00`

##### Disk structure
here's [explanation](https://www.youtube.com/watch?v=wtdnatmVdIg)

and here's what you need for now
![](./pics/hard_disk_platter.png)

##### Real Mode
x86 architecture supports different operating modes that determine the processor's behavior and the software it can execute. one of them is real mode, and every mode has some constraints and some features, and every x86 os starts in real mode the most basic mode, why you asking I believe for compatibility reasons, for now don't worry if you didn't understand what is going on yet


##### memory segmentation in real mode
>we view memory as *addressable array of bytes* and that is usually how the *physical memory* look like, on top of this
>view, there's *logical view* in simple words just another way
>of seeing the same memory and this different way is called *x86 segmentation*, why you asking? think of scenario where you have two programmers writing two different programmes and
>if we used *physical* view of the memory one programmer could decide to write into location `0x89`, and other programmer also tries to write something there they will conflict each other and another problem like there's no separation between programme's so any program can read or write any location which leads to no security and a lot of stuff, but for now
>think of *x86 segmentation* just another way of seeing the memory, what do I mean by "seeing"? just different way to write/read memory.

**Details**

>In x86 segmentation the main memory is viewed as separated parts called segments and each segment stores a bunch of related data

![](./pics/memory_x86_segmentation_1.png)

>To access data inside a segment, each byte can be referred to by its own offset

![](./pics/memory_x86_segmentation_2.png)

>The running program can be separated into three possible types of segments in x86, these types are: code segment which stores the code of the program under execution, data segments which store the data of the program and the stack segment which stores the data of program’s stack

>okay now we have this logical view of memory but at the end of day we need to access the ram, so we need some kind of mapping/translation between logical and physical, and this process can be done either by software or hardware in this case it's done automatically by cpu so yes it's done by hardware

>RAM hardware sees the data as a big array of bytes. Therefore, we need some tools to implement (map) the logical view of segmentation on top the actual hardware. for this purpose, special registers named segment registers are presented in x86, the size of each segment register is 16 bits and they are: CS which is used to define the code segment. SS which is used to define the stack segment. DS, and others.

>Each segment register stores the starting memory address of a segment

>assume that we have a code of some program loaded into the memory and its starting physical memory address is 100d, that is, the first instruction of this program is stored in this address

![](./pics/memory_x86_segmentation_3.png)

>To reach the first byte of this code we use the offset 0, so, the whole physical address of the first byte will be 100:0d

![](./pics/memory_x86_segmentation_4.png)

>To make the processor handle this piece of code as the current code segment then its starting memory address should be loaded into the register CS, that is, setting the value 100d to CS, so, we can say in other words that CS contains the starting memory address of currently executing code segment

>let’s say it is executing the following jmp 150d which jumps to the address 150d. What really happens here is that the processor consider the value 150d as an offset instead of a full memory address, the processor is going to retrieve the value of the register CS to know what is the starting memory address of the currently active code segment and append the value 150 to it. Say, the value of CS is 100, then the memory address that the processor is going to jump to is 100:150d.


##### Source Code Explained
you maybe wondering why is [this](#set-data-segment) well that because x86 [real mode](#real-mode) memory segmentation wait what [memory segmentation](#memory-segmentation-in-real-mode) so when our programme is going to try access data like some string to print cpu will consult with ds register to find it that's why we set it

![](./pics/set_data_segment_1.png)
![](./pics/set_data_segment_2.png)
![](./pics/set_data_segment_3.png)

so if ds=0 cpu will try access 0d:0x9 which has nothing to do with what we are trying to do

##### Source Code Parts

###### signature 
```asm
times 510-($-$$) db 0
dw 0xaa55
```

###### set data segment
```asm
mov ax, 0x7c00
mov ds, ax
```