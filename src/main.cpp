#include "application.h"

#include <iostream>

int main()
{
    try
    {
        minecpp::Application application;
        application.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "fuck you: " << ex.what() << std::endl;
    }

    return 0;
}
