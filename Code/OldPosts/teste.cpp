// opcodes to run a jump to the function AntiAttachAbort
BYTE jmpToAntiAttachAbort[] =
{ 0xB8, 0xCC, 0xCC, 0xCC, 0xCC,   // mov eax, 0xCCCCCCCC
0xFF, 0xE0 };                     // jmp eax

// � � � � � � � �
*reinterpret_cast<PVOID*>(&jmpToAntiAttachAbort[1]) = AntiAttachAbort; 
