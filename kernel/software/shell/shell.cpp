#include <software/shell/shell.hpp>
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

            printf("Command: ");
            for (int i = 0; i < this->shellTextBufferPointer; i++)
            {
                this->shellTextBuffer[i] = this->shellTextBufferReverse[(this->shellTextBufferPointer - 1) - i];

            }
            this->shellTextBuffer[this->shellTextBufferPointer] = '\0';
            printf("%s", this->shellTextBuffer);
            printf("\n");

            this->shellTextBufferPointer = 0;


            // Execute command

            // add strcmp for correct comparison
            if(memcmp("ls", shellTextBuffer, 2) == 0){
                printf("Execute ls\n");
            }

            if(memcmp("cat", shellTextBuffer, 2) == 0){
                printf("Execute cat\n");
            }

            if(memcmp("cd", shellTextBuffer, 2) == 0){
                printf("Execute cd\n");
            }

            if(memcmp("write", shellTextBuffer, 2) == 0){
                printf("Execute write\n");
            }
        }
    }
}