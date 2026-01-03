#ifndef __MY_OS__SOFTWARE__SHELL_H
#define __MY_OS__SOFTWARE__SHELL_H

#include <driver/keyboard.hpp>
#include <filesystem/fat-vfs.hpp>

class Shell
{
public:
    // TODO: later should use syscalls instead of direct call to driver
    // TODO: later should use syscalls instead of direct call to FATVFS
    Shell(KeyboardDriver *keyboardDriver, FATVFS *fatvfs);
    void run();

private:
    KeyboardDriver *keyboardDriver;
    FATVFS *fatvfs;
    char shellTextBuffer[128];
    char shellTextBufferReverse[128];
    uint8_t shellTextBufferPointer;
};

#endif