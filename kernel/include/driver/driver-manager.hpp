#ifndef __DRIVER_MANAGER_H
#define __DRIVER_MANAGER_H
#include <driver/driver.hpp>

class DriverManager
{

private:
    int numDrivers;

public:
    Driver *drivers[255];

    DriverManager();
    void AddDriver(Driver *);
    void ActivateAll();
};

#endif