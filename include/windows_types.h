#ifndef WINDOWS_TYPES_H
#define WINDOWS_TYPES_H

extern "C" {
	unsigned char* __stdcall GetModuleHandleA(
		const char* moduleName
	);
	void* __stdcall LoadLibraryA(
		const char* lpLibFileName
	);
	void* __stdcall GetProcAddress(
		void* hModule,
		const char* lpProcName
	);
	void __stdcall EnterCriticalSection(
		void* lpCriticalSection
	);
	void __stdcall LeaveCriticalSection(
		void* lpCriticalSection
	);
	inline void* _InlineInterlockedExchangePointer(
		void* volatile*,
		void*
	);
}
constexpr unsigned int IMAGE_NUMBEROF_DIRECTORY_ENTRIES = 16;
constexpr unsigned short IMAGE_DOS_SIGNATURE = 0x5A4D;
constexpr unsigned int IMAGE_NT_SIGNATURE = 0x00004550;

typedef struct _IMAGE_DOS_HEADER {
	unsigned short   e_magic;
	unsigned short   e_cblp;
	unsigned short   e_cp;
	unsigned short   e_crlc;
	unsigned short   e_cparhdr;
	unsigned short   e_minalloc;
	unsigned short   e_maxalloc;
	unsigned short   e_ss;
	unsigned short   e_sp;
	unsigned short   e_csum;
	unsigned short   e_ip;
	unsigned short   e_cs;
	unsigned short   e_lfarlc;
	unsigned short   e_ovno;
	unsigned short   e_res[4];
	unsigned short   e_oemid;
	unsigned short   e_oeminfo;
	unsigned short   e_res2[10];
	int	             e_lfanew;
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;
typedef struct _IMAGE_FILE_HEADER {
	unsigned short   Machine;
	unsigned short   NumberOfSections;
	unsigned int     TimeDateStamp;
	unsigned int     PointerToSymbolTable;
	unsigned int     NumberOfSymbols;
	unsigned short   SizeOfOptionalHeader;
	unsigned short   Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;
typedef struct _IMAGE_DATA_DIRECTORY {
	unsigned int   VirtualAddress;
	unsigned int   Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER {
	unsigned short   Magic;
	unsigned char    MajorLinkerVersion;
	unsigned char    MinorLinkerVersion;
	unsigned int     SizeOfCode;
	unsigned int     SizeOfInitializedData;
	unsigned int     SizeOfUninitializedData;
	unsigned int     AddressOfEntryPoint;
	unsigned int     BaseOfCode;
	unsigned int     BaseOfData;
	unsigned int     ImageBase;
	unsigned int     SectionAlignment;
	unsigned int     FileAlignment;
	unsigned short   MajorOperatingSystemVersion;
	unsigned short   MinorOperatingSystemVersion;
	unsigned short   MajorImageVersion;
	unsigned short   MinorImageVersion;
	unsigned short   MajorSubsystemVersion;
	unsigned short   MinorSubsystemVersion;
	unsigned int     Win32VersionValue;
	unsigned int     SizeOfImage;
	unsigned int     SizeOfHeaders;
	unsigned int     CheckSum;
	unsigned short   Subsystem;
	unsigned short   DllCharacteristics;
	unsigned int     SizeOfStackReserve;
	unsigned int     SizeOfStackCommit;
	unsigned int     SizeOfHeapReserve;
	unsigned int     SizeOfHeapCommit;
	unsigned int     LoaderFlags;
	unsigned int     NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, * PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_NT_HEADERS {
	unsigned int Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERint, * PIMAGE_NT_HEADERint, * PIMAGE_NT_HEADERS;

#endif // WINDOWS_TYPES_H