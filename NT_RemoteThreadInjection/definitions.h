#pragma once
#include <Windows.h>

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

typedef struct _INITIAL_TEB {
	PVOID StackBase;
	PVOID StackLimit;
	PVOID StackCommit;
	PVOID StackCommitMax;
	PVOID StackReserved;

} INITIAL_TEB, * PINITIAL_TEB;

typedef struct PS_ATTRIBUTE {
	ULONG Attribute;
	SIZE_T Size;
	union
	{
		ULONG Value;
		PVOID ValuePtr;
	} ul;
	PSIZE_T ReturnLength;
} PS_ATTRIBUTE, * PPS_ATTRIBUTE;

typedef struct _PS_ATTRIBUTE_LIST {
	SIZE_T TotalLength;
	PS_ATTRIBUTE Attribute[1];
} PS_ATTRIBUTE_LIST, * PPS_ATTRIBUTE_LIST;

typedef NTSTATUS(NTAPI* _NTOpenProc)(PHANDLE ProcessHandle,
	ACCESS_MASK AccessMask,
	POBJECT_ATTRIBUTES
	ObjectAttributes,
	PCLIENT_ID ClientID);
typedef NTSTATUS(NTAPI* _NTAllocVM)(HANDLE, PVOID, ULONG, PULONG, ULONG, ULONG);
typedef NTSTATUS(NTAPI* _NTWriteVM)(HANDLE, PVOID, PVOID, ULONG, PULONG);
typedef NTSTATUS(NTAPI* _NtCT)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE, PVOID, PVOID, ULONG, SIZE_T, SIZE_T, SIZE_T, PPS_ATTRIBUTE_LIST);

#define InitializeObjectAttributes(p, n, a, r, s) \
{\
(p)->Length = sizeof(OBJECT_ATTRIBUTES); \
(p)->RootDirectory = r; \
(p)->Attributes = a; \
(p)->ObjectName = n; \
(p)->SecurityDescriptor = s; \
(p)->SecurityQualityOfService = NULL; \
}

