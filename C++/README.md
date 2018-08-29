# Notes

 - This library has been written using VS 2017 **without** the inclusion of the stdafx header.
 - The sources can be used with every compiler that supports C++11 (or higher).
 - The Qt plugin has been created with Qt Creator 4.7.0 and Qt 5.11.1 (more info in the Qt folder)
 - Installation, documentation and examples can be found in the Source folder

While creating the library the C++ language standard was `/std:c++14` but if your compiler supports only C++11 you should be able to compile the files anwyay. If you use Visual Studio like me I recommend you to setup `/std:c++14` (or higher like `/std:c++17`).
