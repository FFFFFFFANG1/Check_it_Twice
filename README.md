# Check it Twice
  A light-weight, system-level program to against screenshot malware in Windows

## Requirement & Versions
1. Detours (latest release), available at https://github.com/microsoft/Detours
2. windows.h (this header file belongs to Microsoft SDK, ask GPT to install it and add it to environment path)
3. Visual Studio 2022

## Build

### Build DLL (x64)
The hook will be compiled as a dynamic load library. 
1. Open a new project in VS2022. Add hbb.h to header files, DLLmain.cpp and hbb.cpp to source files.
2. Click "generate" on top tool bar, 
3. Right click project name on the left bar and choose "property"
   ![image](https://github.com/user-attachments/assets/da1599a5-2ce8-4432-ac78-3c88ebd0e087)
4. Modify:
     a. 配置类型-常规：dll
   ![image](https://github.com/user-attachments/assets/f2fb17f4-346a-4275-9d67-0bd6c79d8c9f)

     b. VC++目录-包含目录：file path to detours/include
     c. VC++目录-库目录： detours/lib_x64
     ![image](https://github.com/user-attachments/assets/633ffaac-7d09-4699-a057-ef8f64226b7b)
     d. 链接器-输入-附加依赖项：detours/lib_x64/detours.lib
   ![image](https://github.com/user-attachments/assets/c34f9fbe-e1ae-4715-8741-b4847bb39304)


###

   
