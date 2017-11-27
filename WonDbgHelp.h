/* WonDbgHelp.h --- Wonders ImageDirectoryEntryToData */
/**************************************************************************/

#ifndef WONDBGHELP_H_
#define WONDBGHELP_H_     1   /* Version 1 */

#include "wonnt.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/

typedef LONG NTSTATUS;

#ifndef STATUS_SUCCESS
    #define STATUS_SUCCESS ((NTSTATUS)0x00000000)
#endif
#ifndef STATUS_INVALID_PARAMETER
    #define STATUS_INVALID_PARAMETER ((NTSTATUS)0xC000000D)
#endif
#ifndef STATUS_INVALID_IMAGE_FORMAT
    #define STATUS_INVALID_IMAGE_FORMAT ((NTSTATUS)0xC000007B)
#endif

/**************************************************************************/

#ifndef RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK
    #define RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK 0x00000001
#endif

IMAGE_NT_HEADERS *NTAPI WonImageNtHeader(void *base);

NTSTATUS NTAPI WonImageNtHeaderEx(
    ULONG flags,
    void *base,
    ULONGLONG size,
    IMAGE_NT_HEADERS **out_nt);

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
