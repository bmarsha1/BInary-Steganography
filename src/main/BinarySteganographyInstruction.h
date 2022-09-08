/*
File:    BinarySteganographyInstruction.h
Project: Final Project
Author:  Ben Marshall
Date:    12/06/20
Section: 04
E-mail:  bmarsha1@umbc.edu

Definition of the BinarySteganographyInstruction struct
holds the instruction's offset and its size

*/
#ifndef BINARYSTEGANOGRAPHYINSTRUCTION_H
#define BINARYSTEGANOGRAPHYINSTRUCTION_H

struct BinarySteganographyInstruction
{
    unsigned long offset;
    size_t size;
};

#endif
