/*
File:    BinarySteganography.h
Project: Final Project
Author:  Ben Marshall
Date:    12/06/20
Section: 04
E-mail:  bmarsha1@umbc.edu

Definition of the BinarySteganography class
Takes parameters for the keyfile, original binary, and
modified binary. Can conceal message in modified binary
and extract message from modified binary.

*/
#ifndef BINARYSTEGANOGRAPHY_H
#define BINARYSTEGANOGRAPHY_H

#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include "BinarySteganographyKey.h"

using std::string;
using std::vector;
using std::istreambuf_iterator;
using std::exception;
using std::range_error;

class BinarySteganography
{

    private:
        //Maximum size that can be stored in the binary
        size_t m_maxBytes;
        //Holds the bytes from the original binary
        vector<char> m_origBinBytes;
        //Holds the bytes for the new modified binary
        vector<char> m_newBinBytes;
        //Holds the contents of the keyfile
        vector<BinarySteganographyKey> m_keys;
        //Path to the key file
        string m_keyFilePath;
        //Path to the original binary
        string m_origBinPath;
        //Path to the new modified bin
        string m_newBinPath;

        //Loads the keys into memory from file
        void loadKeyFile();
        //Loads a binary into memory from file
        void loadBin(string paht, vector<char> &arr);
        //Writes modified binary to disk
        void writeModifiedBin();
        //Encodes a bit into modified file byte array using key at specified index
        void encodeBit(size_t index, bool bit);
        //Decodes a but from modified file byte array using key at specified index
        bool decodeBit(size_t index);

    public:
        //Constructor
        BinarySteganography(string keyFilePath, string originalBinPath, 
                string newBinPath);

        //Conceal
        //Conceals message in binary
        void conceal(string message);

        //Extract
        //Extracts message from binary
        string extract();

};

#endif
