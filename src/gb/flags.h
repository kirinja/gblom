struct Flags {
    unsigned char Z; // ZERO FLAG; set to 1 if current operation results in Zero, or two values match on a CMP operation
    unsigned char N; // SUBTRACT FLAG; set to 1 if a subtraction was performed
    unsigned char H; // HALF CARRY FLAG; set to 1 if a carry occured from the lower nibble in the last operation
    unsigned char C; // CARRY FLAG; set to 1 if a carry occured in the last operation or if A is the smaller value on CP instruction
    bool HALT;
};
