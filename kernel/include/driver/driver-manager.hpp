#ifndef __DRIVER_MANAGER_H
#define __DRIVER_MANAGER_H
#include <driver/driver.hpp>

class DriverManager
{

private:
    Driver *drivers[255];
    int numDrivers;

public:
    DriverManager();
    void AddDriver(Driver *);
    void ActivateAll();
};

#endif