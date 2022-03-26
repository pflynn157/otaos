## Ota OS

My first working attempt at making an operating system. Compared to many other non-real-world OSes out there, this is not very impressive, but for my first somewhat working hobby OS, I'm happy with it :)

Features
* Intel x86-32 bit
* Boots with custom bootloader (only legacy BIOS)
* Loads global descriptor table (GDT)
* Loads interrupt descriptor table (IDT)
* Basic keyboard interrupts with keyboard driver
* Basic software interrupts
* ATA PIO polling driver for hard disk reads (read/write supported)
* Basic filesystem layer with custom filesystem (sort of...)
* Ability to load small program from disk

And yes, I did make that sound better than it might actually be :) But we have to start somewhere.

### Why?

See my blog for more information. But in a nutshell, I did this for learning purposes. The time has passed when you can go out and create some general purpose OS and expect it to go anywhere. But that said, it still important for newer generations of programmers (of which I'm part of...) to understand how OSes work so we can maintain the existing ones. And new OS development is far from dead, even if it exists in a different context.

So in other words: for learning purposes.

### Why x86-32 (i386)?

I tried to do a UEFI-supported x86-64 kernel, but I was having tons of trouble, both getting it to work and finding resources. The most I was able to do was get a simple kernel with a display driver to boot on my computers. Even though its quite dated, there are more resources and examples for x86-32 out there, so I decided to just stick with this.

### Future Plans

None at the moment. Eventually, I want to do something more in-depth, but I'm happy that I got this much working. And I learned a lot.

