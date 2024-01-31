#include <stdio.h>
#include <string>
#include "pin.H"
#include "inverse.h"

// Command line args
KNOB<std::string> pwfile_name(KNOB_MODE_WRITEONCE, "pintool", "o", "password.txt", "File to save password in");
FILE *pwfile;

// Called before each syscall instruction
VOID syscall_before(ADDRINT rip, ADDRINT call_num, CONTEXT* ctxt)
{
    if (call_num == 0)  // sys_read
    {
        PIN_SetContextReg(ctxt, REG_GAX, 16);           // Set return value
        PIN_SetContextReg(ctxt, REG_INST_PTR, rip + 2); // Set RIP to address of next instruction
        PIN_ExecuteAt(ctxt);                            // Skip the syscall
    }
}

// Called before aesenc and aesdec instructions involved in comparisons
VOID aes_before(ADDRINT eip, VOID* rsp, UINT8* xmm0, UINT8* xmm1, UINT8* xmm2, CONTEXT* ctxt, bool enc)
{
    // Copy xmm0 to xmm2
    if (PIN_SafeCopy((VOID*)xmm2, (VOID*)xmm0, 16) != 16)
    {
        printf("[%lx] Error copying xmm0 to xmm2!", eip);
        PIN_ExitApplication(-1);
    }

    // Calculate correct value for xmm2
    if (enc)
        inv_aesenc(xmm2, xmm1);
    else
        inv_aesdec(xmm2, xmm1);
    
    // Update buffer with calculated value
    if (PIN_SafeCopy(rsp, xmm2, 16) != 16)
    {
        printf("[%lx] Error copying calculated input to [rsp]!", eip);
        PIN_ExitApplication(-1);
    }

    // Print calculated value
	PIN_LockClient();
    printf("[%lx] \"", eip);

    for (size_t i = 0; i < 16; i++)
        printf("\\x%.2x", xmm2[i] & 0xff);

    puts("\"");
	PIN_UnlockClient();
	
	// Add bytes to password file
	fwrite(xmm2, 16, 1, pwfile);
}

VOID insInstrumentation(INS ins, VOID* v)
{
    switch (INS_Opcode(ins))
    {
    case XED_ICLASS_SYSCALL:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)syscall_before,
            IARG_INST_PTR,
            IARG_REG_VALUE, REG_EAX,
            IARG_CONTEXT,
            IARG_END);
        break;

    case XED_ICLASS_AESENC:
		// Only instrument operations used for comparisons,
        // not the ones in the actual decryption loops
        if (INS_OperandReg(ins, 0) == REG_XMM2)
        {
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)aes_before,
                IARG_INST_PTR,
                IARG_REG_VALUE, REG_STACK_PTR,
                IARG_REG_REFERENCE, REG_XMM0,
                IARG_REG_REFERENCE, REG_XMM1,
                IARG_REG_REFERENCE, REG_XMM2,
                IARG_CONTEXT,
                IARG_BOOL, true,    // aesenc
                IARG_END);
        }
        break;

    case XED_ICLASS_AESDEC:
        if (INS_OperandReg(ins, 0) == REG_XMM2)
        {
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)aes_before,
                IARG_INST_PTR,
                IARG_REG_VALUE, REG_STACK_PTR,
                IARG_REG_REFERENCE, REG_XMM0,
                IARG_REG_REFERENCE, REG_XMM1,
                IARG_REG_REFERENCE, REG_XMM2,
                IARG_CONTEXT,
                IARG_BOOL, false,    // aesdec
                IARG_END);
        }
        break;
    }
}
    
VOID finished(INT32 code, VOID* v)
{
    printf("Finished");
}

int main(int argc, char* argv[])
{
    // Init PIN
    PIN_InitSymbols();

    // Parse command line
    if (PIN_Init(argc, argv))
        return -1;

    // Open password file
    if ( !(pwfile = fopen(pwfile_name.Value().c_str(), "wb")) )
    {
        printf("Unable to open file to write password: %s\n", pwfile_name.Value().c_str());
        return -1;
    }

    // Setup PIN instrumentation callbacks
    INS_AddInstrumentFunction(insInstrumentation, 0);
    PIN_AddFiniFunction(finished, 0);

    PIN_StartProgram();
    return 0;
}
