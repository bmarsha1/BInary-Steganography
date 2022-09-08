/*
File:    BinarySteganography.cpp
Project: Final Project
Author:  Ben Marshall
Date:    12/06/20
Section: 04
E-mail:  bmarsha1@umbc.edu

Implementation of the BinarySteganography class

*/
#include "BinarySteganography.h"

#include <sstream>

using std::ios_base;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::istreambuf_iterator;
using std::getline;
using std::istringstream;
using std::stoul;
using std::stoi;
using std::to_string;

void BinarySteganography::loadKeyFile()
{
    //Open file
    ifstream infile(m_keyFilePath);

    //Read data from file
    while(infile.peek() != EOF)
    {
        //Holds lines from the text file
        string line;

        //Get a line
        getline(infile, line);

        //Process line
        BinarySteganographyKey key;
        istringstream sstream(line);
        string buff;
        
        //Get first offset
        getline(sstream, buff, ',');
        key.first.offset = stoul(buff);

        //Get first size
        getline(sstream, buff, ',');
        key.first.size = stoi(buff);

        //Get second offset
        getline(sstream, buff, ',');
        key.second.offset = stoul(buff);

        //Get second size
        getline(sstream, buff);
        key.second.size = stoi(buff);

        //Add to list
        m_keys.push_back(key);
    }

    //Maximum number of bytes we can store
    m_maxBytes = m_keys.size() / 8;
}

void BinarySteganography::loadBin(string path, vector<char> &arr)
{
    //Open file and load bytes
    ifstream infile(path, ios_base::binary);
    arr = vector<char>(istreambuf_iterator<char>(infile), istreambuf_iterator<char>());
}

void BinarySteganography::writeModifiedBin()
{
    ofstream outfile(m_newBinPath, ios::out | ios::binary);
    outfile.write(&m_newBinBytes[0], m_newBinBytes.size());
}

void BinarySteganography::encodeBit(size_t index, bool bit)
{
    //We only have to make a swap in the case of a 1
    if(bit)
    {
        //Get the pair
        BinarySteganographyKey key = m_keys[index];
        
        //Buffers to hold the instructions
        char *first = new char[key.first.size];
        char *second = new char[key.second.size];
        
        //Read instructions
        for(size_t i = 0; i < key.first.size; ++i)
        {
            first[i] = m_origBinBytes[i + key.first.offset];
        }
        for(size_t i = 0; i < key.second.size; ++i)
        {
            second[i] = m_origBinBytes[i + key.second.offset];
        }

        //Write second instruction first
        for(size_t i = 0; i < key.second.size; ++i)
        {
            m_newBinBytes[i + key.first.offset] = second[i];
        }
        //Write first instruction second
        for(size_t i = 0; i < key.first.size; ++i)
        {
            m_newBinBytes[i + key.first.offset + key.second.size] = first[i];
        }

        //Done with the buffers
        delete[] first;
        delete[] second;
    }
}

bool BinarySteganography::decodeBit(size_t index)
{
    //Get the pair
    BinarySteganographyKey key = m_keys[index];
    for(size_t i = 0; i < key.first.size + key.second.size; ++i)
    {
        //Get bytes from both files
        char origByte = m_origBinBytes[i + key.first.offset];
        char modifiedByte = m_newBinBytes[i + key.first.offset];

        //If they do not match, the instructions have been swapped
        if(origByte != modifiedByte)
            return 1;
    }

    //All bits must have matched
    return 0;
}

BinarySteganography::BinarySteganography(string keyFilePath, string originalBinPath, 
        string newBinPath)
{
    m_keyFilePath = keyFilePath;
    m_origBinPath = originalBinPath;
    m_newBinPath = newBinPath;
}

void BinarySteganography::conceal(string message)
{
    //Read info from files
    loadKeyFile();
    loadBin(m_origBinPath, m_origBinBytes);
    
    //Make sure we can fit the string in the binary
    if(message.size() > m_maxBytes)
    {
        throw range_error("Input size " + to_string(message.size()) + " > max size " + to_string(m_maxBytes));
    }

    //keep track of the current swap
    size_t currSwap = 0;

    //start with the original binary
    m_newBinBytes = m_origBinBytes;

    //iterate over entire string
    for(size_t i = 0; i < message.size(); ++i)
    {
        char character = message[i];
        for(int j = 0; j < 8; j++)
        {
            //Get the bits from the character
            bool bit = (character & (0b10000000 >> j)) >> (7 - j);
            //Encode the bit in the message
            encodeBit(currSwap, bit);
             
            //Increment currSwap
            ++currSwap;
        }
    }

    //Check if we still have space left
    if(m_maxBytes * 8 - currSwap > 8)
    {
        //Write a null terminator at the end (8 zeros)
        for(int i = 0; i < 8; ++i)
        {
            encodeBit(currSwap, 0);
            ++currSwap;
        }
    }

    writeModifiedBin();
}

string BinarySteganography::extract()
{
    //Read info from files
    loadKeyFile();
    loadBin(m_origBinPath, m_origBinBytes);
    loadBin(m_newBinPath, m_newBinBytes);

    string message = "";

    //Keep track of the current swap
    size_t currSwap = 0;

    for(size_t byte = 0; byte < m_maxBytes; ++byte)
    {
        char character = 0;

        //Assemble character bit by bit
        for(int i = 0; i < 8; ++i)
        {
            //Get next bit
            char bit = decodeBit(currSwap);
            //Put the bit on the character
            character |= bit << (7 - i);

            //Increment currSwap
            ++currSwap;
        }

        //Break if there is a null terminator
        if(character == 0)
            break;

        //Tack the character on the end of the string
        message += (char) character;
    }

    return message;
}
