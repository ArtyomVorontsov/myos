#include <vfs/file-reader.hpp>
#include <vfs/file-writer.hpp>
#include <types.h>

class FileEnumerator
{
public:
    virtual char *GetName();
    virtual FileReader *GetReader();
    virtual FileWriter *GetWriter();
    virtual int GetSize();
    virtual FileEnumerator *Next();
    virtual uint8_t getType(); // 0 - file, 1 - dir
};
