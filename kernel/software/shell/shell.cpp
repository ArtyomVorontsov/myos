#include <software/shell/shell.hpp>
#include <software/cat/cat.hpp>
#include <software/cd/cd.hpp>
#include <software/ls/ls.hpp>
#include <software/write/write.hpp>
#include <software/pwd/pwd.hpp>
#include <stdio.h>
#include <string.h>

Shell::Shell(KeyboardDriver *keyboardDriver, FATVFS *fatvfs)
{
    this->keyboardDriver = keyboardDriver;
    this->fatvfs = fatvfs;
    this->shellTextBufferPointer = 0;
}

void Shell::run()
{
    while (true)
    {

        uint8_t bufferPendingAmount = keyboardDriver->getBufferPendingAmount();
        uint8_t lastBufferChar = keyboardDriver->getLastBufferChar();
        char ENTER_CHAR = '\n';

        if (bufferPendingAmount && lastBufferChar == ENTER_CHAR)
        {

            // Get command

            // pop new line
            keyboardDriver->bufferPop();
            bufferPendingAmount = keyboardDriver->getBufferPendingAmount();

            uint8_t skipNextChar = 0;
            uint8_t skipNextCharTotal = 0;

            int i = 0;
            int j = bufferPendingAmount - 1;
            for (; j >= 0; j--)
            {
                // fill buffer in reverse
                char c = keyboardDriver->bufferPop();

                if (c == '\b')
                {
                    skipNextChar++;
                    skipNextCharTotal++;
                    continue;
                }

                if (skipNextChar)
                {
                    skipNextChar--;
                    continue;
                }

                this->shellTextBufferReverse[i] = c;
                this->shellTextBufferPointer++;

                i++;
            }

            for (int i = 0; i < this->shellTextBufferPointer; i++)
            {
                this->shellTextBuffer[i] = this->shellTextBufferReverse[(this->shellTextBufferPointer - 1) - i];
            }
            this->shellTextBuffer[this->shellTextBufferPointer] = '\0';

            // parse command and argument
            // TODO: implement normal parsing, probably as scripting language
            char command[10] = {0};
            char argument1[10] = {0};
            char argument2[20] = {0};

            bool cmdWasProcessed = false;
            bool argument1WasProcessed = false;
            for (int j = 0, i = 0; i < this->shellTextBufferPointer; i++)
            {

                if (argument1WasProcessed)
                {
                    argument2[j] = this->shellTextBuffer[i];
                    j++;
                }
                else if (cmdWasProcessed)
                {
                    if (this->shellTextBuffer[i] != ' ')
                    {
                        argument1[j] = this->shellTextBuffer[i];
                        j++;
                    }
                }
                else
                {
                    if (this->shellTextBuffer[i] != ' ')
                    {
                        command[j] = this->shellTextBuffer[i];
                        j++;
                    }
                }

                if (this->shellTextBuffer[i] == ' ' && cmdWasProcessed)
                {
                    argument1WasProcessed = true;
                    j = 0;
                }

                if (this->shellTextBuffer[i] == ' ')
                {
                    cmdWasProcessed = true;
                    j = 0;
                }
            }

            this->shellTextBufferPointer = 0;

            // Execute command

            if (strcmp("ls", command) == 0)
            {
                Ls ls(fatvfs);

                ls.run(argument1);
            }

            if (strcmp("cat", command) == 0)
            {
                Cat cat(fatvfs);

                cat.run(argument1);
            }

            if (strcmp("cd", command) == 0)
            {
                Cd cd(fatvfs);

                cd.run(argument1);
            }

            if (strcmp("write", command) == 0)
            {
                Write write(fatvfs);

                write.run(argument1, argument2);
            }

            if (strcmp("pwd", command) == 0)
            {
                Pwd pwd(fatvfs);

                pwd.run(argument1);
            }
        }
    }
}