#### Protected mode

what is protected mode? well we have said in `part-1`, that in x86 architecture that the cpu has modes
like BMW Comfort Mode vs Sport Mode, some provide feature that the other lacks and vise versa, so
before we  have used `real mode` that has the consequence of how are we gonna use it because in that
mode you can use only 16-bit registers, and there's no such thing as memory protection, in Modern OS
we need some advanced features like memory protection, virtual memory multitasking ...
that is why we have `Protected Mode` so if our cpu is running in this mode, we can use these features
(memory protection...), the question now is how  do we switch from `real mode` to `protected mode`?
well can't we just start in `protected mode` unfortunately we can not because every x86 cpu start's 
in real mode for backwards compatibility, so yes we need somehow to switch from real-mode to protected-mode

###### Keep In Mind

###### Memory Protection
well In `protected mode` every `process` aka running program should be isolated from other processes
it could lead to `processes` corrupt each other memory which is bad and keep in mind that of the processes
that is running is your `kernel`, and you are not responsible for what it's going to run under it
for example `chrome` so I don't want `chrome` to jump for example to `kernel` code so we need some
mechanism to enforce it. this is when it comes `privilege levels`

###### Privilege Levels
read the part about [memory protection](#memory-protection).
The academic literature of operating systems separate the system environment into two modes,
kernel-mode and user-mode, at a given time, the system may run on one of these modes and not both of them
the kernel is runs on kernel-mode and has the privilege to do anything (access any resource of the system)
while the user-application run on user-mode which is restricted environment aka can't perform sensitive actions
x86 protected has feature that helps implement this which is called `privilege levels` and specifically
we **Four Privilege Levels** numbered from 0 to 3 being that 0 is the most privilege and 3 the lowest.
and by the way this `privilege levels` not just prevents user-mode application to not interfere with 
kernel, it also prevents user-mode from executing a sensitive instructions

###### Global Descriptor Table
is table that is stored in memory and it's starting memory address is stored in register called GDTR
**global descriptor table (GDT)**.
each entry in this table is called *segment descriptor* which has the size of 8 bytes and we can refer
to them by index number called *segment selector*
every segment descriptor defines a segment of any type (code segment, stack segment ...) and has enough
information that the processor needs to deal with the segment, for example the starting memory address
of the segment and so on, the segment selector of the currently running program should be stored in the
corresponding segment register for example if the segment descriptor is stored in the second raw of GDT
defines a code segment, `cs` should be 8 why 8 and not 1? well that because each segment descriptor
is 8 bytes so address of the second raw is 8 bytes from the start of GDT, let's dive deeper into the
[*segment descriptor*](#segment-descriptor-structure)

###### Segment Descriptor Structure
A segment descriptor is an 8 bytes entry of global descriptor table which stores multiple fields
and flags that describe the properties of a specific segment in the memory
![](./pics/empty_segment_descriptor.png)
one of the most important information about a segment is its starting memory address, which is called
the *base address* of a segment, when code refers to memory to read from it or write to it. it is actually
referring to specific segment, we call this memory address a generated memory address since it's not
referring to physical memory address, it should pass some translation like before `part-1`.
The logical memory address in x86 may pass two translation processes instead of one in order to
obtain the physical memory address. The first address translation is performed on a logical
memory address to obtain a *linear memory address* which is another not real and not physical
memory address which is there in x86 architecture because of paging feature. If paging is enabled
a second translation is needed that it takes *linear memory address* and it give us *physical memory address*
in our case I'm not going to use paging so one translation is enough. and here's how the translation
is performed
![](./pics/generated_to_physical_memory_address.png)