/*
File:    extract.cpp
Project: Final Project
Author:  Ben Marshall
Date:    12/06/20
Section: 04
E-mail:  bmarsha1@umbc.edu

A program for extracting messages from binaries
Requires a keyfile, original binary, and modified binary. 
Prints the message to stdout.

*/
#include <iostream>

#include "BinarySteganography.h"

using std::cout;
using std::endl;
using std::exception;

//Print correct usage
void usage(const char* executableName)
{
    cout << "Usage: ";
    cout << executableName;
    cout << " <keyfile> <input binary> <output binary>" << endl;
}

//main
int main(int argc, char** argv)
{
    //Check for correct number of arguments
    if(argc != 4)
    {
        usage(argv[0]);
        return 1;
    }

    BinarySteganography bs(argv[1], argv[2], argv[3]);

    try
    {
        cout << bs.extract() << endl;
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return 1;
    }


    return 0;
}
