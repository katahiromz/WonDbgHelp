/* WonDbgHelp.h --- Wonders ImageDirectoryEntryToData */
/**************************************************************************/

#ifndef WONDBGHELP_H_
#define WONDBGHELP_H_     1   /* Version 1 */

#include "wonnt.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/

IMAGE_NT_HEADERS *NTAPI WonRtlImageNtHeader(void *base);

IMAGE_SECTION_HEADER *NTAPI
WonImageRvaToSection(IMAGE_NT_HEADERS *nt, void *base, ULONG rva);

void *NTAPI WonImageRvaToVa(
    IMAGE_NT_HEADERS *nt,
    void *base,
    ULONG rva,
    IMAGE_SECTION_HEADER **ppSection);

void *WINAPI
WonImageDirectoryEntryToData(
    void *base,
    BOOLEAN image,
    USHORT dir,
    ULONG *size);

void *WINAPI
WonImageDirectoryEntryToDataEx(
    void *base,
    BOOLEAN image,
    USHORT dir,
    ULONG *size,
    IMAGE_SECTION_HEADER **section);

/**************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* ndef WONDBGHELP_H_ */
