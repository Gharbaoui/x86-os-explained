#### Part-1

this is not intended to explain all the details of the os, what I want is to make something easy to understand, is it going to be perfect? no, when I have the choice to make something good techinically or understandable  I'll choose understandable this readme's are intended to be read while you are reading  the source code.

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
"I said computer loades bootloader from disk to ram"
- which disk?
- where does it end up in the ram?

>which disk well there's no spefic disk for example some specialized disk, what really happens is that
the software that is trying to locate the bootloader is looking for some signature, to be exact it checks every disk and read the first sector of 512 bytes and if that sector has `0xaaff` that is the bootloader


##### Disk structure
here's [explainatin](https://www.youtube.com/watch?v=wtdnatmVdIg)
and here's what you need for now


##### Source Code Explained

