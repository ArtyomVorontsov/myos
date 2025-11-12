# 🧠 Custom Operating System Project  

## Overview  
A custom **Operating System** written entirely from scratch in **C++** and **x86 Assembly**, built for learning and experimentation in low-level systems programming.  
The project is fully functional up to a graphical desktop environment and has been thoroughly tested in virtualized environments.  

🧩 **Inspired by:**  
- [Write Your Own Operating System (YouTube)](https://www.youtube.com/@writeyourownoperatingsystem)  
- [OSDev Wiki](https://wiki.osdev.org)  

---

## 🛠️ Completed Features  

- Minimal **bootable OS** with bootloader and kernel skeleton  
- Text output (“Hello OS”) in **VGA text mode**  
- Transition from **real mode** to **protected mode**  
- **Global Descriptor Table (GDT)** for memory segmentation  
- **Interrupt Descriptor Table (IDT)** with CPU exception handling  
- **Hardware interrupts** and ISR management  
- **Port I/O** operations for hardware communication  
- **Keyboard driver** (interrupt-based, scan code → ASCII translation)  
- **Mouse driver** (PS/2 interface, cursor and button handling)  
- **Driver abstraction layer** for modular input handling  
- **PCI bus** enumeration and device detection  
- **Base Address Register (BAR)** mapping for device memory regions  
- **VGA graphics mode (320x200)** with low-level pixel and shape rendering  
- **GUI framework** with windows, cursor, and event handling  
- **Desktop environment** supporting multiple draggable windows and text  
- **Bootable disk image** compatible with **QEMU**, **VirtualBox**, and **VMware**  

---

## 🧩 Technologies  
- **Languages:** C++, x86 Assembly  
- **Tools:** NASM, GCC cross-compiler, QEMU, VirtualBox, VMware, Makefile  

---

## 🧪 Testing  
- Verified under **QEMU**
- Confirmed stable input/output, interrupt handling, and graphics rendering  

---

## ✅ Summary  
The project demonstrates the journey from bare-metal boot code to a simple desktop OS. 

---

## 🧱 TODO  

### **Phase 6: Multitasking & Memory**
- [ ] Implement **task scheduler** with context switching between processes  
- [ ] Add **dynamic memory management** and **kernel heap allocator** (`malloc`/`free`)  

### **Phase 7: Networking**
- [ ] Develop **basic networking stack** (Ethernet driver, frame transmission)  
- [ ] Implement **ATA driver** for hard drive read/write access  
- [ ] Add **system call interface** and start **POSIX API** alignment (`read`, `write`, etc.)  

### **Phase 8: Networking Protocols (Advanced)**
- [ ] Build full **network stack**: Ethernet → ARP → IPv4 → ICMP → UDP → TCP  
- [ ] Implement **minimal HTTP server** running natively on the OS  

### **Phase 9: File Systems**
- [ ] Parse **partition tables (MBR)**  
- [ ] Implement **FAT32 filesystem driver** for file listing, reading, and writing  

