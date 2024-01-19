#include "ProcessOperator.h"
#include "MemLoadLibrary2.h"
#include <Psapi.h>
#include "Utils.h"

typedef struct _PARAMX
{
    PVOID lpFileData;
    DWORD DataLength;
    PVOID LdrGetProcedureAddress;
    PVOID pLdrLoadDll;
    PVOID RtlInitAnsiString;
    PVOID RtlAnsiStringToUnicodeString;
    PVOID RtlFreeUnicodeString;
    PVOID pMemoryAddress;
}PARAMX, * PPARAMX;
ProcessOperator::ProcessOperator(DWORD pid) :PID(pid)
{
}
BOOL ProcessOperator::Open()
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    InitializeObjectAttributes(&ObjectAttributes, NULL, 0, NULL, NULL);
    CLIENT_ID ClientId = { (HANDLE)this->PID, NULL };
    return NT_SUCCESS(NtOpenProcess(&Handle, PROCESS_ALL_ACCESS, &ObjectAttributes, &ClientId));
}
BOOL ProcessOperator::Close()
{
	return NtClose(this->Handle);
}
BOOL ProcessOperator::Read(ULONG64 addr, PVOID buffer, SIZE_T size)
{
    return NT_SUCCESS(NtReadVirtualMemory(this->Handle, PVOID(addr), buffer, size, NULL));
}
BOOL ProcessOperator::Write(ULONG64 addr, PVOID buffer, SIZE_T size)
{
    return NT_SUCCESS(NtWriteVirtualMemory(this->Handle, PVOID(addr), buffer, size, NULL));
}

long long ProcessOperator::VirtualMemorySize()
{
	PROCESS_MEMORY_COUNTERS pmc;
    K32GetProcessMemoryInfo(this->Handle, &pmc, sizeof(pmc));
	return pmc.WorkingSetSize;
}
std::wstring ProcessOperator::CommandLine()
{
    PROCESS_BASIC_INFORMATION pbi;
    NTSTATUS isok = NtQueryInformationProcess(this->Handle, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
    if (NT_SUCCESS(isok))
    {
        PEB peb = Read<PEB>((ULONG64)pbi.PebBaseAddress);
        if (peb.ProcessParameters)
        {
            RTL_USER_PROCESS_PARAMETERS upps = Read<RTL_USER_PROCESS_PARAMETERS>((ULONG64)peb.ProcessParameters);
            if (upps.CommandLine.Buffer)
            {
                std::wstring result = L"";
                result.resize(upps.CommandLine.Length);
                if (Read((ULONG64)upps.CommandLine.Buffer, &result[0], upps.CommandLine.Length))
                {
                    return result;
                }
            }
        }
    }
    return L"";
}

ULONG64 ProcessOperator::AllocateMemory(SIZE_T size, DWORD protect, DWORD type,ULONG64 baseAddr)
{
    if(NT_SUCCESS(NtAllocateVirtualMemory(this->Handle, (PVOID*)&baseAddr, 0, &size, type, protect)))
        return baseAddr;
    return NULL;
}

BOOL ProcessOperator::FreeMemory(ULONG64 addr, SIZE_T size, DWORD freeType)
{
    return NT_SUCCESS(NtFreeVirtualMemory(this->Handle, (PVOID*)&addr, &size, freeType));
}
ULONG64 ProcessOperator::CallRemote(ULONG64 func,ULONG64 rcx,ULONG64 rdx,ULONG64 r8,ULONG64 r9, ULONG64 rsp20, ULONG64 rsp28, ULONG64 rsp30, ULONG64 rsp38)
{
    BYTE code[] =
    {
        0x55,                                           //push rbp
        0x48,0x8B,0xEC,                                 //mov rbp,rsp
        0x48,0x83,0xEC,0x50,                            //sub rsp,50
        0x48,0xB9,0,0,0,0,0,0,0,0,                      //mov rcx,0000000000000000
        0x48,0xBA,0,0,0,0,0,0,0,0,                      //mov rdx,0000000000000000
        0x49,0xB8,0,0,0,0,0,0,0,0,                      //mov r8,0000000000000000
        0x49,0xB9,0,0,0,0,0,0,0,0,                      //mov r9,0000000000000000
        0x48,0xB8,0,0,0,0,0,0,0,0,                      //mov rax,0000000000000000
        0x48,0x89,0x44,0x24,0x20,                       //mov [rsp+20],rax
        0x48,0xB8,0,0,0,0,0,0,0,0,                      //mov rax,0000000000000000
        0x48,0x89,0x44,0x24,0x28,                       //mov [rsp+28],rax
        0x48,0xB8,0,0,0,0,0,0,0,0,                      //mov rax,0000000000000000
        0x48,0x89,0x44,0x24,0x30,                       //mov [rsp+30],rax
        0x48,0xB8,0,0,0,0,0,0,0,0,                      //mov rax,0000000000000000
        0x48,0x89,0x44,0x24,0x38,                       //mov [rsp+38],rax
        0x48,0xB8,0,0,0,0,0,0,0,0,                      //mov rax,0000000000000000
        0xFF,0xD0,									    //call rax        
        0x48,0xA3,0,0,0,0,0,0,0,0,                      //mov [0000000000000000],rax
        0x48,0x83,0xC4,0x50,                            //add rsp,50
        0x48,0x8B,0xE5,                                 //mov rsp,rbp
        0x5D,                                           //pop rbp
        0xC3,                                           //ret
        0,0,0,0,0,0,0,0                                 //result
    };
    ULONG64* p_rcx = (ULONG64*)(code + 10);
    ULONG64* p_rdx = (ULONG64*)(code + 20);
    ULONG64* p_r8 = (ULONG64*)(code + 30);
    ULONG64* p_r9 = (ULONG64*)(code + 40);
    ULONG64* p_rsp20 = (ULONG64*)(code + 50);
    ULONG64* p_rsp28 = (ULONG64*)(code + 65);
    ULONG64* p_rsp30 = (ULONG64*)(code + 80);
    ULONG64* p_rsp38 = (ULONG64*)(code + 95);


    ULONG64* p_call = (ULONG64*)(code + 110);
    ULONG64* p_rax = (ULONG64*)(code + 122);
    *p_rcx = rcx;
    *p_rdx = rdx;
    *p_r8 = r8;
    *p_r9 = r9;
    *p_call = func;
    *p_rsp20 = rsp20;
    *p_rsp28 = rsp28;
    *p_rsp30 = rsp30;
    *p_rsp38 = rsp38;
    ULONG64 membase = this->AllocateMemory(sizeof(code), PAGE_EXECUTE_READWRITE, MEM_COMMIT, NULL);
    *p_rax = ((ULONG64)membase + (sizeof(code) - 8));
    if (membase)
    {
        if (this->Write(membase, code, sizeof(code)))
        {
            ULONG64 result = NULL;
            HANDLE tHadnle = CreateRemoteThread(this->Handle, NULL, 0x400, (LPTHREAD_START_ROUTINE)membase, 0, 0, 0);
            if (tHadnle)
            {
                if(WaitForSingleObject(tHadnle, INFINITE)!= WAIT_FAILED)
                    result = this->Read<ULONG64>(((ULONG64)membase + 67));
            }
            this->FreeMemory(membase, sizeof(code));
            return result;
        }
        this->FreeMemory(membase, sizeof(code));
    }
    return NULL;
}
ULONG64 ProcessOperator::InjectDll(BYTE* buffer,SIZE_T size)
{
    HMODULE NtBase = GetModuleHandleA("ntdll.dll");
    LPVOID dllbase = VirtualAllocEx(this->Handle, NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (dllbase)
    {
        SIZE_T tmpSize = 0;
        WriteProcessMemory(this->Handle, dllbase, buffer, size, &tmpSize);
        PARAMX MemLoadLibrary2Param = {};
        MemLoadLibrary2Param.pLdrLoadDll = GetProcAddress(NtBase, "LdrLoadDll");
        MemLoadLibrary2Param.RtlInitAnsiString = GetProcAddress(NtBase, "RtlInitAnsiString");
        MemLoadLibrary2Param.RtlAnsiStringToUnicodeString = GetProcAddress(NtBase, "RtlAnsiStringToUnicodeString");
        MemLoadLibrary2Param.RtlFreeUnicodeString = GetProcAddress(NtBase, "RtlFreeUnicodeString");
        MemLoadLibrary2Param.LdrGetProcedureAddress = GetProcAddress(NtBase, "LdrGetProcedureAddress");
        MemLoadLibrary2Param.lpFileData = dllbase;
        MemLoadLibrary2Param.DataLength = size;
        MemLoadLibrary2Param.pMemoryAddress = VirtualAllocEx(this->Handle, NULL, GetSectionSize((PVOID)buffer), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        auto RemoteMemLoadLibrary2Param = VirtualAllocEx(this->Handle, NULL, sizeof(PARAMX), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (RemoteMemLoadLibrary2Param)
        {
            WriteProcessMemory(this->Handle, RemoteMemLoadLibrary2Param, &MemLoadLibrary2Param, sizeof(PARAMX), &tmpSize);
            PVOID MemLoadLibrary2Address = VirtualAllocEx(this->Handle, NULL, sizeof(MemLoadLibrary2), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
            if (MemLoadLibrary2Address)
            {
                WriteProcessMemory(this->Handle, MemLoadLibrary2Address, &MemLoadLibrary2, sizeof(MemLoadLibrary2), &tmpSize);
                BYTE shellcode[] =
                {
                    0x90,
                    0xC6,0x05,0xF8,0xFF,0xFF,0xFF,0xC3, //mov [^],0xC3
                    0x48,0xB9,0,0,0,0,0,0,0,0,			//mov rcx,0
                    0xFF,0x25,0,0,0,0,0,0,0,0,0,0,0,0,	//jmp 0
                    0xC3								//ret
                };
                *(ULONG64*)(shellcode + 10) = (ULONG64)RemoteMemLoadLibrary2Param;
                *(ULONG64*)(shellcode + 24) = (ULONG64)MemLoadLibrary2Address;
                ULONG64 jmpmem = (ULONG64)VirtualAllocEx(this->Handle, NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                if (!jmpmem)
                    return NULL;
                WriteProcessMemory(this->Handle, (PVOID)jmpmem, (PVOID)shellcode, sizeof(shellcode), &tmpSize);
                HANDLE tHadnle = CreateRemoteThread(this->Handle, NULL, 0, (LPTHREAD_START_ROUTINE)jmpmem, 0, 0, 0);
                WaitForSingleObject(tHadnle, INFINITE);
                DWORD exitCode;
                GetExitCodeThread(tHadnle, (PDWORD)&exitCode);
            }
        }
        return (ULONG64)MemLoadLibrary2Param.pMemoryAddress;
    }
    return NULL;
}