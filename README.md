# Check it Twice
  A light-weight, system-level program to against screenshot malware in Windows

## Requirement & Versions
1. Detours (latest release), available at https://github.com/microsoft/Detours
2. windows.h (this header file belongs to Microsoft SDK, ask GPT to install it and add it to environment path)
3. Visual Studio 2022

## Build
1. DLL: The hook will be compiled as a dynamic load library. Open a new project in VS2022, add 
