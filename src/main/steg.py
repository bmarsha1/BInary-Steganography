#Get a list of all of the instructions in the binary
minAddress = currentProgram.getMinAddress()
listing = currentProgram.getListing()
ins_list = listing.getInstructions(minAddress, 1)

#Keep track of how much data we can store
bits = 0

#Class for holding data
class InstrPair:
    ins1 = 0
    ins1_size = 0
    ins2 = 0
    ins2_size = 0

#Array of swappable instuction pairs
instr_pairs = []

#Iterate over all instructions
while ins_list.hasNext():
    #Get two instructions, and only continue if both instructions are
    #fallthrough instructions
    ins1 = ins_list.next()
    if not "MOV " in ins1.toString() or ins1.toString() == "MOV RBP,RSP":
        continue
    if ins_list.hasNext():
        ins2 = ins_list.next()
        if not "MOV " in ins2.toString() or ins2.toString() == "MOV RBP,RSP" or ins2.getLabel() != None:
            continue
        
        #Get inputs and outputs for both functions
        ins1_input = ins1.getInputObjects()
        ins1_output = ins1.getResultObjects()
        ins2_input = ins2.getInputObjects()
        ins2_output = ins2.getResultObjects()
        
        #Check if any instructions have no input/output
        if len(ins1_input) == 0:
            continue
        if len(ins1_output) == 0:
            continue
        if len(ins2_input) == 0:
            continue
        if len(ins2_output) == 0:
            continue
        
        valid = True
        
        #Check to make sure that the inputs/outputs are not addresses
        #This code cannot handle relative addresses based on RIP.
        #They are lost in translation to sleigh
        for input in ins1_input:
            if isinstance(input, ghidra.program.model.address.Address):
                valid = False
                break
        
        if not valid:
            continue
            
        for input in ins2_input:
            if isinstance(input, ghidra.program.model.address.Address):
                valid = False
                break
        
        if not valid:
            continue
            
        for output in ins1_output:
            if isinstance(output, ghidra.program.model.address.Address):
                valid = False
                break
        
        if not valid:
            continue
            
        for output in ins2_output:
            if isinstance(output, ghidra.program.model.address.Address):
                valid = False
                break
        
        if not valid:
            continue
        

        #Check to make sure that the input of ins2 does not
        #depend on the output of ins1
        for output in ins1_output:
            if output in ins2_input:
                valid = False
                break
            if isinstance(output, ghidra.program.model.lang.Register):
                for input in ins2_input:
                    if isinstance(input, ghidra.program.model.lang.Register):
                        if output.contains(input) or input.contains(output):
                            valid = False
                            break

        if not valid:
            continue

        #Check to maks sure that the input of ins1 does not
        #depend on the output of ins2
        for output in ins2_output:
            if output in ins1_input:
                valid = False
                break
            if isinstance(output, ghidra.program.model.lang.Register):
                for input in ins1_input:
                    if isinstance(input, ghidra.program.model.lang.Register):
                        if output.contains(input) or input.contains(output):
                            valid = False
                            break

        if not valid:
            continue
            
        #Check to maks sure that the output of ins1 does not
        #match that of ins2
        #If they produce the same output, flipping the order will matter
        for out1 in ins1_output:
            if out1 in ins2_output:
                valid = False
                break
            if isinstance(out1, ghidra.program.model.lang.Register):
                for out2 in ins2_output:
                    if isinstance(out2, ghidra.program.model.lang.Register):
                        if out1.contains(out2) or out2.contains(out1):
                            valid = False
                            break

        if not valid:
            continue

        #Each swap is another bit
        bits += 1

        #Add entry to list
        pair = InstrPair()
        pair.ins1 = ins1.getAddress().getOffset() - minAddress.getOffset()
        pair.ins1_size = ins1.getLength()
        pair.ins2 = ins2.getAddress().getOffset() - minAddress.getOffset()
        pair.ins2_size = ins2.getLength()
        instr_pairs.append(pair)
        
#Print size info
print ("Bits",bits)
print ("Bytes",bits / 8)

#Write results to file
file1 = open(askFile("FILE", "Save results").getAbsolutePath(), "w")

for pair in instr_pairs:
    file1.write(str(pair.ins1))
    file1.write(str(", "))
    file1.write(str(pair.ins1_size))
    file1.write(str(", "))
    file1.write(str(pair.ins2))
    file1.write(str(", "))
    file1.write(str(pair.ins2_size))
    file1.write('\n')
