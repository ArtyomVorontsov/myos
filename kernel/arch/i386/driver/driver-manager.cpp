#include <driver/driver-manager.hpp>

DriverManager::DriverManager()
{
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver *drv)
{
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager::ActivateAll()
{
    for (int i = 0; i < numDrivers; i++)
    {
        drivers[i]->Activate();
    }
}