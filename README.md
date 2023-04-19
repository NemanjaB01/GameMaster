Task: Interprocess Communication (IPC)

This exercise should teach you what interprocess communication is for and how you can realize it.
Main Idea

Everybody of us has used interprocess communication already. Mostly unintentionally at this point in your studies. This is why we wanted to take this specific topic into this semester’s course.

To understand the concept of IPC, some major concepts must be learned and understood beforehand.

    Virtual Memory

    Process vs. Thread

    Shared Resources

    Locking

Some of those terms are already familiar to you, others not. I will not entirely go into details for this assignment, but it’s always useful looking up information based on those keywords.
Implementation details: Magic Conch Shell (Magische Miesmuschel)

You MUST NOT change predefined function signatures, sequences of checks or similar. Exploits will automatically result in deductions! Do not remove or add any usleep or assert statements!

The idea of this assignment is that the User can ask the MagicConchShell questions, and the MagicConchShell uses predefined answers to reply. The answers differ depending on the motivation of the MagicConchShell.
You start the User-process, which then has to start the MagicConchShell-process. These two processes shall communicate using shared memory. The User-process reads questions from STDIN (terminal input) and sends them to the MagicConchShell (there are two exceptions, described in Command Handling). The MagicConchShell-process replies with the appropriate answer, depending on the current state of motivation.

User.c
:

    ”initSharedMemoriesUser()”
        Initialize the shared memory objects.
        Make sure only to set the permissions the process needs!
    ”initMmapingsUser()”
        Map those objects to virtual memory.
        The permissions need to match the ones you used for the SHM objects!
        Make sure not to use this function to do anything else (like initializations).
    ”initLocks()”
        Initialize any locks you might need.
    ”initProcess()”
        Launch the MagicConchShell process.
        Ensure you launch a new process.
        Ensure you execute the right executable.
    has to be synchronized properly
    has to be cleaned up properly (also stop and clean up everything after getCommand() returns 0)

The User asks for the input, publishes it on the shared memory, and makes it available for the MagicConchShell-process.
For initializing the shared memory, CAREFULLY take a look at the defines!
The names of those objects are predefined and can be found in the list of defines in util.h
:.

DO NOT remove or relocate code for checking your approach. Otherwise, we will deduct points.

MagicConchShell.c
:

    ”initSharedMemoriesMagicConchShell()”
        open the shared memory objects.
        Make sure only to set the permissions the process needs!
    ”initMmapingsMagicConchShell()”
        map those objects to virtual memory.
        The permissions need to match the ones you used for the SHM objects!
    ”dispatcher()”
        dispatch the messages received in the request object and return the appropriate answer in the response object.
    has to be synchronized properly as well
    has to be cleaned up properly
    make sure to use the provided functions

Command handling
:

    motivation/unmotivation: these commands should be handled by the User-process, before sending them to the MagicConchShell. “motivation” sets the motivated variable in the shmstate struct to 1, “unmotivation” sets it to 0. The MagicConchShell should treat this as a question it has no answer for. Initially, the MagicConchShell is in motivated state.
    <predefined question>: depending on the motivated state, there are predefined answers to each question. You can find those listed in MagicConchShell.c.
    bye: the MagicConchShell responds with “bye” and both processes are terminated.
    any other input or questions without predefined answers should be answered with “I do not know.”.

DO NOT remove or relocate code for checking your approach. Otherwise, we will deduct points.

util.c
:

    You can add checks in the functions provided, but when tested, all changes made will be ignored.

util.h
:

    The only change allowed in this file is adding synchronization primitives to the shmlocks struct. All other changes made in this file will make your program not testable. Do not change anything but carefully examine the extern variables, defines, and structs.

IPC in a (way too oversimplified) nutshell

Those simplified explanations should not replace your attendance and attention in the lecture nor serve you the detailed solution to this assignment. It should help you to understand the central concept briefly to make research more accessible.

Virtual Memory: Virtual memory is part of the concept of modern operating systems. As you may suspect, there is a difference between physical and virtual memory. The physical memory provided by, e.g. (SO-)DIMMs
has to be managed by the operating system that uses virtual addresses for locating data. So there has to be a sort of translation between the physical and virtual memory addresses. If you are interested in this topic, you can look up this and related articles [1].

Process vs. Threads: As many of you already know from previous assignments, processes are treated differently in contrast to threads – well, kind of. As you discovered, a thread can operate on the whole memory the program has mapped. This means, e.g., the heap-allocated by one thread can be used with any other thread the program launches. It is possible since they share the same virtual address space, as they are all used in one program launched before. So we can deduce that each process has its own virtual memory space. Those virtual memory spaces are strictly separated from each other.
Thus, sharing resources ”between” two processes is realized differently from sharing resources ”in” a process. So we have to share memory between two processes. This memory is surprisingly called shared memory, and it is a central part of IPC [2].

Shared resources: Shared resources are “saved” in files, as the file system (FS) is accessible in both processes – well, kind of again. But besides some exceptions, all files saved on the FS can be accessed depending on the rights a user has. But some of you may wonder, how to access this data? Do we have to use ”read()” and ”write()” for sharing information? Well, this is possible with one big limitation. This limitation is that we cannot use virtual addresses to access data easily and fast. Well, there is a solution called ”mmap()”, which takes a file descriptor (fd) that maps the file (full size) to our virtual memory space of the program. That’s it, well, kind of. How this mapping works in the kernel will be taught in the following course called operating systems.

Locking: When we use IPC, mostly, some locking mechanisms have to be used. In our case, semaphores
and mutexes
are the way to go [3].
Holding a lock while going to sleep will lead to point deductions.
Some further hints

What to do before you start?

    Pull from upstream!
    Try to understand the program structure.
    Carefully read the Manpages and/or the POSIX standard on the needed functions.
    Only begin, if you understand the basic concept of processes, virtual memory, shared resources as well as mapping them. Bruteforcing will lead to a severe amount of wasted time.
    User.h
    and MagicConchShell.h
    MUST NOT be changed,
    You are allowed to add code everywhere in the program and extend structs as you like. But don’t remove or move around existing code ever (this would lead to point deductions).
    Do not push binary files or any other junk files.
    Carefully read the TODOs. Some contain crucial information!

Submission

Modify the files in your git repository. You can find this file in the directory A4
. Tag the submission with A4
and push it to the server.
LINKS and Further reading

[1] https://wiki.osdev.org/Memory_management

[2] https://wiki.osdev.org/Processes_and_Threads

[3] https://wiki.osdev.org/Semaphores#Semaphores

as well as the slides
Assignment Tutor
If you have any questions regarding this assignment, go to Discord and read through the SLP channels. The probability that your question was already answered or some discussions will lead you in the right direction is quite high. If not so, just ask in the corresponding channel.
If you have a more direct question regarding your specific solution, you can also ask the tutor who organizes this assignment.

Linda Pirker, linda.pirker@student.tugraz.at
