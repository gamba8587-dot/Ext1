#include <windows.h>
#ifndef ARRAYSIZE
    #define ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))
#endif
#include "./hde/hde.h"
typedef hde64s HDE;
#define HDE_DISASM(code, hs) hde64_disasm(code, hs)
#include "trampoline.h"
#include "buffer.h"
#define TRAMPOLINE_MAX_SIZE (MEMORY_SLOT_SIZE - sizeof(JMP_ABS))
static BOOL IsCodePadding(LPBYTE pInst, UINT size)
{
    UINT i;
    if (pInst[0] != 0x00 && pInst[0] != 0x90 && pInst[0] != 0xCC)
        return FALSE;
    for (i = 1; i < size; ++i)
    {
        if (pInst[i] != pInst[0])
            return FALSE;
    }
    return TRUE;
}
BOOL CreateTrampolineFunction(PTRAMPOLINE ct)
{
    CALL_ABS call = {
        0xFF, 0x15, 0x00000002,
        0xEB, 0x08,
        0x0000000000000000ULL
    };
    JMP_ABS jmp = {
        0xFF, 0x25, 0x00000000,
        0x0000000000000000ULL
    };
    JCC_ABS jcc = {
        0x70, 0x0E,
        0xFF, 0x25, 0x00000000,
        0x0000000000000000ULL
    };
    UINT8     oldPos   = 0;
    UINT8     newPos   = 0;
    ULONG_PTR jmpDest  = 0;
    BOOL      finished = FALSE;
    UINT8     instBuf[16];
    ct->patchAbove = FALSE;
    ct->nIP        = 0;
    do
    {
        HDE       hs;
        UINT      copySize;
        LPVOID    pCopySrc;
        ULONG_PTR pOldInst = (ULONG_PTR)ct->pTarget     + oldPos;
        ULONG_PTR pNewInst = (ULONG_PTR)ct->pTrampoline + newPos;
        copySize = HDE_DISASM((LPVOID)pOldInst, &hs);
        if (hs.flags & F_ERROR)
            return FALSE;
        pCopySrc = (LPVOID)pOldInst;
        if (oldPos >= sizeof(JMP_REL))
        {
            jmp.address = pOldInst;
            pCopySrc = &jmp;
            copySize = sizeof(jmp);
            finished = TRUE;
        }
        else if ((hs.modrm & 0xC7) == 0x05)
        {
            PUINT32 pRelAddr;
            memcpy(instBuf, (LPBYTE)pOldInst, copySize);
            pCopySrc = instBuf;
            pRelAddr = (PUINT32)(instBuf + hs.len - ((hs.flags & 0x3C) >> 2) - 4);
            *pRelAddr
                = (UINT32)((pOldInst + hs.len + (INT32)hs.disp.disp32) - (pNewInst + hs.len));
            if (hs.opcode == 0xFF && hs.modrm_reg == 4)
                finished = TRUE;
        }
        else if (hs.opcode == 0xE8)
        {
            ULONG_PTR dest = pOldInst + hs.len + (INT32)hs.imm.imm32;
            call.address = dest;
            pCopySrc = &call;
            copySize = sizeof(call);
        }
        else if ((hs.opcode & 0xFD) == 0xE9)
        {
            ULONG_PTR dest = pOldInst + hs.len;
            if (hs.opcode == 0xEB)
                dest += (INT8)hs.imm.imm8;
            else
                dest += (INT32)hs.imm.imm32;
            if ((ULONG_PTR)ct->pTarget <= dest
                && dest < ((ULONG_PTR)ct->pTarget + sizeof(JMP_REL)))
            {
                if (jmpDest < dest)
                    jmpDest = dest;
            }
            else
            {
                jmp.address = dest;
                pCopySrc = &jmp;
                copySize = sizeof(jmp);
                finished = (pOldInst >= jmpDest);
            }
        }
        else if ((hs.opcode & 0xF0) == 0x70
            || (hs.opcode & 0xFC) == 0xE0
            || (hs.opcode2 & 0xF0) == 0x80)
        {
            ULONG_PTR dest = pOldInst + hs.len;
            if ((hs.opcode & 0xF0) == 0x70
                || (hs.opcode & 0xFC) == 0xE0)
                dest += (INT8)hs.imm.imm8;
            else
                dest += (INT32)hs.imm.imm32;
            if ((ULONG_PTR)ct->pTarget <= dest
                && dest < ((ULONG_PTR)ct->pTarget + sizeof(JMP_REL)))
            {
                if (jmpDest < dest)
                    jmpDest = dest;
            }
            else if ((hs.opcode & 0xFC) == 0xE0)
            {
                return FALSE;
            }
            else
            {
                UINT8 cond = ((hs.opcode != 0x0F ? hs.opcode : hs.opcode2) & 0x0F);
                jcc.opcode  = 0x71 ^ cond;
                jcc.address = dest;
                pCopySrc = &jcc;
                copySize = sizeof(jcc);
            }
        }
        else if ((hs.opcode & 0xFE) == 0xC2)
        {
            finished = (pOldInst >= jmpDest);
        }
        if (pOldInst < jmpDest && copySize != hs.len)
            return FALSE;
        if ((newPos + copySize) > TRAMPOLINE_MAX_SIZE)
            return FALSE;
        if (ct->nIP >= ARRAYSIZE(ct->oldIPs))
            return FALSE;
        ct->oldIPs[ct->nIP] = oldPos;
        ct->newIPs[ct->nIP] = newPos;
        ct->nIP++;
        memcpy((LPBYTE)ct->pTrampoline + newPos, pCopySrc, copySize);
        newPos += copySize;
        oldPos += hs.len;
    }
    while (!finished);
    if (oldPos < sizeof(JMP_REL)
        && !IsCodePadding((LPBYTE)ct->pTarget + oldPos, sizeof(JMP_REL) - oldPos))
    {
        if (oldPos < sizeof(JMP_REL_SHORT)
            && !IsCodePadding((LPBYTE)ct->pTarget + oldPos, sizeof(JMP_REL_SHORT) - oldPos))
        {
            return FALSE;
        }
        if (!IsExecutableAddress((LPBYTE)ct->pTarget - sizeof(JMP_REL)))
            return FALSE;
        if (!IsCodePadding((LPBYTE)ct->pTarget - sizeof(JMP_REL), sizeof(JMP_REL)))
            return FALSE;
        ct->patchAbove = TRUE;
    }
jmp.address = (ULONG_PTR)ct->pDetour;
    ct->pRelay = (LPBYTE)ct->pTrampoline + newPos;
    memcpy(ct->pRelay, &jmp, sizeof(jmp));
    return TRUE;
}
