#ifndef __DRIVER_H
#define __DRIVER_H

class Driver
{
public:
    Driver();

    virtual void Activate();
    virtual int Reset();
    virtual void Deactivate();
};

#endif