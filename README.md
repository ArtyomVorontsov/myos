## High level plan


## Languages 
* C++
* x86 ASM

## **Phase 1: Bootstrapping & Setup**

1. **Write your own OS in 1 hour (54:56)**

   * Build a *very minimal* bootable OS (bootloader + kernel skeleton).
   * Likely prints something to the screen (“Hello OS”) in text mode.

2. **1-B: Addendum (8:43)**

   * Fixes, small improvements, or corrections to the first build.

3. **2: Install your OS in a Virtual Machine (18:00)**

   * Learn how to package your OS so it runs under QEMU/VMware/VirtualBox.
   * Covers disk images, booting, and debugging inside a VM.

---

## **Phase 2: Core OS Fundamentals**

4. **3: Memory Segments, Global Descriptor Table (47:53)**

   * Move from real mode into *protected mode*.
   * Set up segmentation and GDT, foundation for memory protection.

5. **4: Hardware Communication / Ports (28:06)**

   * Introduce **port I/O** (in/out instructions).
   * How to communicate with hardware at the register level.

6. **5: Interrupts (1:04:35)**

   * Build an **Interrupt Descriptor Table (IDT)**.
   * Handle CPU exceptions and hardware interrupts.

---

## **Phase 3: Device Input**

7. **6: Keyboard (56:33)**

   * Write a **keyboard driver** using interrupts.
   * Translate scan codes into characters.

8. **7: Mouse (33:13)**

   * Write a **mouse driver** (via PS/2).
   * Track cursor movement and clicks.

9. **8: Abstractions for Drivers (49:04)**

   * Introduce a **driver framework** so keyboard/mouse share a common interface.

10. **9: Tidying up (31:18)**

* Refactor code for maintainability.

---

## **Phase 4: Hardware Expansion**

11. **10: PCI (1:06:05)**

* Enumerate and interact with devices via the **PCI bus**.

12. **11: Base Address Registers (52:48)**

* Map device memory regions (BARs).
* Foundation for writing more complex drivers.

---

## **Phase 5: Graphics & GUI**

13. **12: Graphics Mode (VGA) (1:01:26)**

* Switch to VGA graphics mode.
* Draw pixels, lines, and basic shapes.

14. **13: GUI Framework Basics (57:59)**

* Implement primitive GUI widgets: buttons, windows, etc.

15. **14: Desktop and Windows (1:09:23)**

* Build a **desktop environment** inside the OS.
* Multiple draggable windows, event handling.

---

## **Phase 6: Multitasking & Memory**

16. **15: Multitasking (52:13)**

* Introduce a **task scheduler**.
* Implement context switching between processes.

17. **16: Dynamic Memory Management / Heap (1:10:47)**

* Implement a **kernel heap allocator** (`malloc`/`free`).

---

## **Phase 7: Networking**

18. **17: Networking (1:07:05)**
19. **18: Network continued (44:19)**

* Basics of **network stacks**.
* Ethernet driver and sending/receiving frames.

20. **19: Hard Drives (1:08:52)**

* Write **ATA driver**.
* Read and write disk sectors.

21. **20: System Calls, POSIX Compliance (31:49)**

* Define a **system call interface**.
* Begin aligning with POSIX API (e.g., `read`, `write`).

---

## **Phase 8: Networking Protocols (Advanced)**

22. **A01: Ethernet Frames (53:50)**
23. **A02: ARP (1:15:11)**
24. **A03: IPv4 (1:32:02)**
25. **A04: ICMP (30:51)**
26. **A05: UDP (1:28:56)**
27. **A06–A09: TCP + HTTP (50–60 min each)**

* Build a **network stack** step by step: Ethernet → ARP → IP → ICMP → UDP → TCP.
* End with a **minimal HTTP server** running on your OS.

---

## **Phase 9: File Systems**

31. **B01: Partition Table (40:43)**

* Parse disk partition tables (MBR).

32–33. **B02–B03: FAT32 (1:05:17 + 48:10)**

* Implement a **FAT32 filesystem driver**.
* List files, read/write files.

---

## ✅ End Result

By the end of the course:

* A **bootable operating system** that runs in a VM.
* **Keyboard, mouse, disk, and PCI drivers**.
* A **GUI desktop environment** with windows.
* **Multitasking and heap memory management**.
* A **networking stack up to TCP + HTTP server**.
* A **FAT32 filesystem driver** (read/write files from disk).
* A **POSIX-like system call interface** for running user programs.

This is almost a **miniature Linux/Windows hybrid**, built from scratch in C/ASM. 🚀

---


