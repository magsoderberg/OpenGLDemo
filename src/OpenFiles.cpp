/******************************************************
*   OpenGL demo
*
*   Class for opening file dialog in Windows.
* 
*	Author: Magnus Söderberg
*
********************************************************/


#include "OpenFiles.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <algorithm>

std::string OpenFiles::OpenDialog()
{
    OPENFILENAMEA ofn;

    char szFile[260] = { 0 };
    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Display the Open dialog box. 
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;

    }
    return std::string();
 
}