/*
File:    BinarySteganographyKey.h
Project: Final Project
Author:  Ben Marshall
Date:    12/06/20
Section: 04
E-mail:  bmarsha1@umbc.edu

Definition of the BinarySteganographyKey struct
Holds two BinarySteganographyInstructions, one for each
part of the pair

*/
#ifndef BINARYSTEGANOGRAPHYKEY_H
#define BINARYSTEGANOGRAPHYKEY_H

#include "BinarySteganographyInstruction.h"

struct BinarySteganographyKey
{
    BinarySteganographyInstruction first, second;
};

#endif
