#pragma once
#define MEMORY_SLOT_SIZE 64
VOID   UninitializeBuffer(VOID);
LPVOID AllocateBuffer(LPVOID pOrigin);
VOID   FreeBuffer(LPVOID pBuffer);
BOOL   IsExecutableAddress(LPVOID pAddress);
