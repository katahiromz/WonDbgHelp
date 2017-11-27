/* WonDbgHelp.c --- Wonders ImageDirectoryEntryToData */

#include "WonDbgHelp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/

IMAGE_NT_HEADERS *NTAPI WonImageNtHeader(void *base)
{
    IMAGE_DOS_HEADER *dos;
    IMAGE_NT_HEADERS *nt;
    DWORD nt_offset = 0;

    if (!base || base == (void *)-1)
        return NULL;

    dos = (IMAGE_DOS_HEADER *)base;
    if (dos->e_magic == IMAGE_DOS_SIGNATURE)
        nt_offset = dos->e_lfanew;

    if (nt_offset >= 256 * 1024 * 1024)
        return NULL;

    nt = (IMAGE_NT_HEADERS *)((ULONG_PTR)base + nt_offset);
    if (nt->Signature != IMAGE_NT_SIGNATURE)
        return NULL;

    return nt;
}

IMAGE_SECTION_HEADER *NTAPI
WonImageRvaToSection(IMAGE_NT_HEADERS *nt, void *base, ULONG rva)
{
    IMAGE_SECTION_HEADER *section;
    ULONG va;
    ULONG count;

    count = nt->FileHeader.NumberOfSections;
    section = IMAGE_FIRST_SECTION(nt);

    while (count--)
    {
        va = section->VirtualAddress;
        if (va <= rva && rva < va + section->SizeOfRawData)
            return section;
        section++;
    }

    return NULL;
}

void *NTAPI WonImageRvaToVa(
    IMAGE_NT_HEADERS *nt,
    void *base,
    ULONG rva,
    IMAGE_SECTION_HEADER **ppSection)
{
    IMAGE_SECTION_HEADER *pSection = NULL;

    if (ppSection)
        pSection = *ppSection;

    if (pSection == NULL ||
        rva < pSection->VirtualAddress ||
        rva >= pSection->VirtualAddress + pSection->SizeOfRawData)
    {
        pSection = WonImageRvaToSection(nt, base, rva);
        if (pSection == NULL)
            return NULL;

        if (ppSection)
            *ppSection = pSection;
    }

    return (void *)((ULONG_PTR)base + rva +
                    (ULONG_PTR)pSection->PointerToRawData -
                    (ULONG_PTR)pSection->VirtualAddress);
}

/**************************************************************************/

void *WINAPI
WonImageDirectoryEntryToDataEx(
    void *base,
    BOOLEAN image,
    USHORT dir,
    ULONG *size,
    IMAGE_SECTION_HEADER **section)
{
    IMAGE_NT_HEADERS *nt;
    DWORD addr;

    *size = 0;
    if (section)
        *section = NULL;

    if (!(nt = WonImageNtHeader(base)))
        return NULL;
    if (dir >= nt->OptionalHeader.NumberOfRvaAndSizes)
        return NULL;
    if (!(addr = nt->OptionalHeader.DataDirectory[dir].VirtualAddress))
        return NULL;

    *size = nt->OptionalHeader.DataDirectory[dir].Size;
    if (image || addr < nt->OptionalHeader.SizeOfHeaders)
        return (BYTE *)base + addr;

    return WonImageRvaToVa(nt, base, addr, section);
}

void *WINAPI
WonImageDirectoryEntryToData(
    void *base,
    BOOLEAN image,
    USHORT dir,
    ULONG *size)
{
    return WonImageDirectoryEntryToDataEx(base, image, dir, size, NULL);
}

/**************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif
