/*
 * PROJECT:   Veil
 * FILE:      Veil.System.IOManager.h
 * PURPOSE:   This file is part of Veil.
 *
 * LICENSE:   MIT License
 *
 * DEVELOPER: MiroKaku (kkmi04@outlook.com)
 */

#pragma once

// Warnings which disabled for compiling
#if _MSC_VER >= 1200
#pragma warning(push)
// nonstandard extension used : nameless struct/union
#pragma warning(disable:4201)
// 'struct_name' : structure was padded due to __declspec(align())
#pragma warning(disable:4324)
// 'enumeration': a forward declaration of an unscoped enumeration must have an
// underlying type (int assumed)
#pragma warning(disable:4471)
#endif

VEIL_BEGIN()

#ifndef _KERNEL_MODE
// Define share access rights to files and directories

#define FILE_SHARE_VALID_FLAGS          0x00000007

// Define the file attributes values

#if (NTDDI_VERSION < NTDDI_WIN8)

#define FILE_ATTRIBUTE_VALID_FLAGS          0x00007fb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS      0x000031a7

#elif (NTDDI_VERSION < NTDDI_WIN10_RS2)

#define FILE_ATTRIBUTE_VALID_FLAGS          0x0002ffb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS      0x000231a7

#elif (NTDDI_VERSION < NTDDI_WIN10_RS3)

#define FILE_ATTRIBUTE_VALID_FLAGS              0x005affb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS          0x001a31a7

#else

#define FILE_ATTRIBUTE_VALID_FLAGS              0x005affb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS          0x001a31a7

//
// This mask describes the set of attributes that kernel-mode callers may set.
//

#define FILE_ATTRIBUTE_VALID_KERNEL_SET_FLAGS   0x005a31a7

#endif

// Create disposition

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

// Create/open flags

#define FILE_DIRECTORY_FILE             0x00000001
#define FILE_WRITE_THROUGH              0x00000002
#define FILE_SEQUENTIAL_ONLY            0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING  0x00000008

#define FILE_SYNCHRONOUS_IO_ALERT       0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT    0x00000020
#define FILE_NON_DIRECTORY_FILE         0x00000040
#define FILE_CREATE_TREE_CONNECTION     0x00000080

#define FILE_COMPLETE_IF_OPLOCKED       0x00000100
#define FILE_NO_EA_KNOWLEDGE            0x00000200
#define FILE_OPEN_FOR_RECOVERY          0x00000400
#define FILE_RANDOM_ACCESS              0x00000800

#define FILE_DELETE_ON_CLOSE            0x00001000
#define FILE_OPEN_BY_FILE_ID            0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT     0x00004000
#define FILE_NO_COMPRESSION             0x00008000
#define FILE_OPEN_REQUIRING_OPLOCK      0x00010000
#define FILE_DISALLOW_EXCLUSIVE         0x00020000
#if (NTDDI_VERSION >= NTDDI_WIN8)
#define FILE_SESSION_AWARE              0x00040000
#endif

//
//  CreateOptions flag to pass in call to CreateFile to allow the write through xro.sys
//

#define FILE_RESERVE_OPFILTER           0x00100000
#define FILE_OPEN_REPARSE_POINT         0x00200000
#define FILE_OPEN_NO_RECALL             0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY  0x00800000


#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10_RS5)

//
// Create options that go with FILE_CREATE_TREE_CONNECTION.
//

#define TREE_CONNECT_NO_CLIENT_BUFFERING          0x00000008  // matches with FILE_NO_INTERMEDIATE_BUFFERING
#define TREE_CONNECT_WRITE_THROUGH                0x00000002  // matches with FILE_WRITE_THROUGH
#endif  // _WIN32_WINNT_WIN10_RS5

//
//  The FILE_VALID_OPTION_FLAGS mask cannot be expanded to include the
//  highest 8 bits of the DWORD because those are used to represent the
//  create disposition in the IO Request Packet when sending information
//  to the file system
//
#define FILE_VALID_OPTION_FLAGS                 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS            0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS        0x00000032
#define FILE_VALID_SET_FLAGS                    0x00000036

//
// While the highest 8 bits of the create options are reserved for the
// create disposition in the IRP, if a create option flag is processed
// prior to IRP creation, we can utilize these bits. These values are
// masked out of the open packet in IopCreateFile prior to the later
// call to IopParseDevice, where we create the IRP.
// Currently this includes the flag to interpet the EABuffer as an
// instance of EXTENDED_CREATE_INFORMATION.
//

#define FILE_CONTAINS_EXTENDED_CREATE_INFORMATION   0x10000000
#define FILE_VALID_EXTENDED_OPTION_FLAGS            0x10000000

#if (NTDDI_VERSION >= NTDDI_WIN10_NI)

//
//================= Extended Create Information ====================
//

//
// This struct can be extended and new fields may be added to the end
// of the struct in the future.
//
typedef struct _EXTENDED_CREATE_INFORMATION
{
    LONGLONG ExtendedCreateFlags;   // extended create flags
    PVOID EaBuffer;                 // EA buffer
    ULONG EaLength;                 // EA buffer length
} EXTENDED_CREATE_INFORMATION, * PEXTENDED_CREATE_INFORMATION;

//
// 32-bit version of EXTENDED_CREATE_INFORMATION struct
//
typedef struct _EXTENDED_CREATE_INFORMATION_32
{
    LONGLONG ExtendedCreateFlags;   // extended create flags
    void* POINTER_32 EaBuffer;      // EA buffer
    ULONG EaLength;                 // EA buffer length
} EXTENDED_CREATE_INFORMATION_32, * PEXTENDED_CREATE_INFORMATION_32;

//
// Define extra create/open option flags. These are passed in through
// the defined ExtendedInformation struct in the EaBuffer.
//
#define EX_CREATE_FLAG_FILE_SOURCE_OPEN_FOR_COPY        0x00000001
#define EX_CREATE_FLAG_FILE_DEST_OPEN_FOR_COPY          0x00000002

#endif // (NTDDI_VERSION >= NTDDI_WIN10_NI)

#define FILE_COPY_STRUCTURED_STORAGE    0x00000041
#define FILE_STRUCTURED_STORAGE         0x00000441

// I/O status information values for NtCreateFile/NtOpenFile

#define FILE_SUPERSEDED                 0x00000000
#define FILE_OPENED                     0x00000001
#define FILE_CREATED                    0x00000002
#define FILE_OVERWRITTEN                0x00000003
#define FILE_EXISTS                     0x00000004
#define FILE_DOES_NOT_EXIST             0x00000005

#if (NTDDI_VERSION >= NTDDI_WIN10_RS3)
//
// Define the QueryFlags values for NtQueryDirectoryFileEx.
//

#define FILE_QUERY_RESTART_SCAN                 0x00000001
#define FILE_QUERY_RETURN_SINGLE_ENTRY          0x00000002
#define FILE_QUERY_INDEX_SPECIFIED              0x00000004
#define FILE_QUERY_RETURN_ON_DISK_ENTRIES_ONLY  0x00000008
#endif
#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
#define FILE_QUERY_NO_CURSOR_UPDATE             0x00000010
#endif

// Special ByteOffset parameters

#define FILE_WRITE_TO_END_OF_FILE       0xffffffff
#define FILE_USE_FILE_POINTER_POSITION  0xfffffffe

// Alignment requirement values

#define FILE_BYTE_ALIGNMENT             0x00000000
#define FILE_WORD_ALIGNMENT             0x00000001
#define FILE_LONG_ALIGNMENT             0x00000003
#define FILE_QUAD_ALIGNMENT             0x00000007
#define FILE_OCTA_ALIGNMENT             0x0000000f
#define FILE_32_BYTE_ALIGNMENT          0x0000001f
#define FILE_64_BYTE_ALIGNMENT          0x0000003f
#define FILE_128_BYTE_ALIGNMENT         0x0000007f
#define FILE_256_BYTE_ALIGNMENT         0x000000ff
#define FILE_512_BYTE_ALIGNMENT         0x000001ff

// Maximum length of a filename string

#define DOS_MAX_COMPONENT_LENGTH 255
#define DOS_MAX_PATH_LENGTH (DOS_MAX_COMPONENT_LENGTH + 5)

#define MAXIMUM_FILENAME_LENGTH         256

// Extended attributes

#define FILE_NEED_EA                    0x00000080

#define FILE_EA_TYPE_BINARY             0xfffe
#define FILE_EA_TYPE_ASCII              0xfffd
#define FILE_EA_TYPE_BITMAP             0xfffb
#define FILE_EA_TYPE_METAFILE           0xfffa
#define FILE_EA_TYPE_ICON               0xfff9
#define FILE_EA_TYPE_EA                 0xffee
#define FILE_EA_TYPE_MVMT               0xffdf
#define FILE_EA_TYPE_MVST               0xffde
#define FILE_EA_TYPE_ASN1               0xffdd
#define FILE_EA_TYPE_FAMILY_IDS         0xff01

// Device characteristics

#define FILE_REMOVABLE_MEDIA            0x00000001
#define FILE_READ_ONLY_DEVICE           0x00000002
#define FILE_FLOPPY_DISKETTE            0x00000004
#define FILE_WRITE_ONCE_MEDIA           0x00000008
#define FILE_REMOTE_DEVICE              0x00000010
#define FILE_DEVICE_IS_MOUNTED          0x00000020
#define FILE_VIRTUAL_VOLUME             0x00000040
#define FILE_AUTOGENERATED_DEVICE_NAME  0x00000080
#define FILE_DEVICE_SECURE_OPEN         0x00000100
#define FILE_CHARACTERISTIC_PNP_DEVICE  0x00000800
#define FILE_CHARACTERISTIC_TS_DEVICE   0x00001000
#define FILE_CHARACTERISTIC_WEBDAV_DEVICE 0x00002000
#define FILE_CHARACTERISTIC_CSV         0x00010000
#define FILE_DEVICE_ALLOW_APPCONTAINER_TRAVERSAL 0x00020000
#define FILE_PORTABLE_DEVICE            0x00040000
#define FILE_REMOTE_DEVICE_VSMB         0x00080000
#define FILE_DEVICE_REQUIRE_SECURITY_CHECK 0x00100000

// Named pipe values

// NamedPipeType for NtCreateNamedPipeFile
#define FILE_PIPE_BYTE_STREAM_TYPE      0x00000000
#define FILE_PIPE_MESSAGE_TYPE          0x00000001
#define FILE_PIPE_ACCEPT_REMOTE_CLIENTS 0x00000000
#define FILE_PIPE_REJECT_REMOTE_CLIENTS 0x00000002
#define FILE_PIPE_TYPE_VALID_MASK       0x00000003

// CompletionMode for NtCreateNamedPipeFile
#define FILE_PIPE_QUEUE_OPERATION       0x00000000
#define FILE_PIPE_COMPLETE_OPERATION    0x00000001

// ReadMode for NtCreateNamedPipeFile
#define FILE_PIPE_BYTE_STREAM_MODE      0x00000000
#define FILE_PIPE_MESSAGE_MODE          0x00000001

// NamedPipeConfiguration for NtQueryInformationFile
#define FILE_PIPE_INBOUND               0x00000000
#define FILE_PIPE_OUTBOUND              0x00000001
#define FILE_PIPE_FULL_DUPLEX           0x00000002

// NamedPipeState for NtQueryInformationFile
#define FILE_PIPE_DISCONNECTED_STATE    0x00000001
#define FILE_PIPE_LISTENING_STATE       0x00000002
#define FILE_PIPE_CONNECTED_STATE       0x00000003
#define FILE_PIPE_CLOSING_STATE         0x00000004

// NamedPipeEnd for NtQueryInformationFile
#define FILE_PIPE_CLIENT_END            0x00000000
#define FILE_PIPE_SERVER_END            0x00000001

#endif // !_KERNEL_MODE

// Win32 pipe instance limit (0xff)
#define FILE_PIPE_UNLIMITED_INSTANCES   0xffffffff 

// Mailslot values

#define MAILSLOT_SIZE_AUTO 0

// private
typedef struct _FILE_IO_COMPLETION_INFORMATION
{
    PVOID KeyContext;
    PVOID ApcContext;
    IO_STATUS_BLOCK IoStatusBlock;
} FILE_IO_COMPLETION_INFORMATION, * PFILE_IO_COMPLETION_INFORMATION;

#ifndef _KERNEL_MODE
typedef enum _FILE_INFORMATION_CLASS
{
    FileDirectoryInformation = 1, // q: FILE_DIRECTORY_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileFullDirectoryInformation, // q: FILE_FULL_DIR_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileBothDirectoryInformation, // q: FILE_BOTH_DIR_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileBasicInformation, // q; s: FILE_BASIC_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES)
    FileStandardInformation, // q: FILE_STANDARD_INFORMATION, FILE_STANDARD_INFORMATION_EX
    FileInternalInformation, // q: FILE_INTERNAL_INFORMATION
    FileEaInformation, // q: FILE_EA_INFORMATION
    FileAccessInformation, // q: FILE_ACCESS_INFORMATION
    FileNameInformation, // q: FILE_NAME_INFORMATION
    FileRenameInformation, // s: FILE_RENAME_INFORMATION (requires DELETE) // 10
    FileLinkInformation, // s: FILE_LINK_INFORMATION
    FileNamesInformation, // q: FILE_NAMES_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileDispositionInformation, // s: FILE_DISPOSITION_INFORMATION (requires DELETE)
    FilePositionInformation, // q; s: FILE_POSITION_INFORMATION
    FileFullEaInformation, // FILE_FULL_EA_INFORMATION
    FileModeInformation, // q; s: FILE_MODE_INFORMATION
    FileAlignmentInformation, // q: FILE_ALIGNMENT_INFORMATION
    FileAllInformation, // q: FILE_ALL_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FileAllocationInformation, // s: FILE_ALLOCATION_INFORMATION (requires FILE_WRITE_DATA)
    FileEndOfFileInformation, // s: FILE_END_OF_FILE_INFORMATION (requires FILE_WRITE_DATA) // 20
    FileAlternateNameInformation, // q: FILE_NAME_INFORMATION
    FileStreamInformation, // q: FILE_STREAM_INFORMATION
    FilePipeInformation, // q; s: FILE_PIPE_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES)
    FilePipeLocalInformation, // q: FILE_PIPE_LOCAL_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FilePipeRemoteInformation, // q; s: FILE_PIPE_REMOTE_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES)
    FileMailslotQueryInformation, // q: FILE_MAILSLOT_QUERY_INFORMATION
    FileMailslotSetInformation, // s: FILE_MAILSLOT_SET_INFORMATION
    FileCompressionInformation, // q: FILE_COMPRESSION_INFORMATION
    FileObjectIdInformation, // q: FILE_OBJECTID_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileCompletionInformation, // s: FILE_COMPLETION_INFORMATION // 30
    FileMoveClusterInformation, // s: FILE_MOVE_CLUSTER_INFORMATION (requires FILE_WRITE_DATA)
    FileQuotaInformation, // q: FILE_QUOTA_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileReparsePointInformation, // q: FILE_REPARSE_POINT_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileNetworkOpenInformation, // q: FILE_NETWORK_OPEN_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FileAttributeTagInformation, // q: FILE_ATTRIBUTE_TAG_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FileTrackingInformation, // s: FILE_TRACKING_INFORMATION (requires FILE_WRITE_DATA)
    FileIdBothDirectoryInformation, // q: FILE_ID_BOTH_DIR_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileIdFullDirectoryInformation, // q: FILE_ID_FULL_DIR_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex])
    FileValidDataLengthInformation, // s: FILE_VALID_DATA_LENGTH_INFORMATION (requires FILE_WRITE_DATA and/or SeManageVolumePrivilege)
    FileShortNameInformation, // s: FILE_NAME_INFORMATION (requires DELETE) // 40
    FileIoCompletionNotificationInformation, // q; s: FILE_IO_COMPLETION_NOTIFICATION_INFORMATION (q: requires FILE_READ_ATTRIBUTES) // since VISTA
    FileIoStatusBlockRangeInformation, // s: FILE_IOSTATUSBLOCK_RANGE_INFORMATION (requires SeLockMemoryPrivilege)
    FileIoPriorityHintInformation, // q; s: FILE_IO_PRIORITY_HINT_INFORMATION, FILE_IO_PRIORITY_HINT_INFORMATION_EX (q: requires FILE_READ_DATA)
    FileSfioReserveInformation, // q; s: FILE_SFIO_RESERVE_INFORMATION (q: requires FILE_READ_DATA)
    FileSfioVolumeInformation, // q: FILE_SFIO_VOLUME_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FileHardLinkInformation, // q: FILE_LINKS_INFORMATION
    FileProcessIdsUsingFileInformation, // q: FILE_PROCESS_IDS_USING_FILE_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FileNormalizedNameInformation, // q: FILE_NAME_INFORMATION
    FileNetworkPhysicalNameInformation, // q: FILE_NETWORK_PHYSICAL_NAME_INFORMATION
    FileIdGlobalTxDirectoryInformation, // q: FILE_ID_GLOBAL_TX_DIR_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex]) // since WIN7 // 50
    FileIsRemoteDeviceInformation, // q: FILE_IS_REMOTE_DEVICE_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FileUnusedInformation,
    FileNumaNodeInformation, // q: FILE_NUMA_NODE_INFORMATION
    FileStandardLinkInformation, // q: FILE_STANDARD_LINK_INFORMATION
    FileRemoteProtocolInformation, // q: FILE_REMOTE_PROTOCOL_INFORMATION
    FileRenameInformationBypassAccessCheck, // (kernel-mode only); s: FILE_RENAME_INFORMATION // since WIN8
    FileLinkInformationBypassAccessCheck, // (kernel-mode only); s: FILE_LINK_INFORMATION
    FileVolumeNameInformation, // q: FILE_VOLUME_NAME_INFORMATION
    FileIdInformation, // q: FILE_ID_INFORMATION
    FileIdExtdDirectoryInformation, // q: FILE_ID_EXTD_DIR_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex]) // 60
    FileReplaceCompletionInformation, // s: FILE_COMPLETION_INFORMATION // since WINBLUE
    FileHardLinkFullIdInformation, // q: FILE_LINK_ENTRY_FULL_ID_INFORMATION // FILE_LINKS_FULL_ID_INFORMATION
    FileIdExtdBothDirectoryInformation, // q: FILE_ID_EXTD_BOTH_DIR_INFORMATION (requires FILE_LIST_DIRECTORY) (NtQueryDirectoryFile[Ex]) // since THRESHOLD
    FileDispositionInformationEx, // s: FILE_DISPOSITION_INFO_EX (requires DELETE) // since REDSTONE
    FileRenameInformationEx, // s: FILE_RENAME_INFORMATION_EX
    FileRenameInformationExBypassAccessCheck, // (kernel-mode only); s: FILE_RENAME_INFORMATION_EX
    FileDesiredStorageClassInformation, // q; s: FILE_DESIRED_STORAGE_CLASS_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES) // since REDSTONE2
    FileStatInformation, // q: FILE_STAT_INFORMATION (requires FILE_READ_ATTRIBUTES)
    FileMemoryPartitionInformation, // s: FILE_MEMORY_PARTITION_INFORMATION // since REDSTONE3
    FileStatLxInformation, // q: FILE_STAT_LX_INFORMATION (requires FILE_READ_ATTRIBUTES and FILE_READ_EA) // since REDSTONE4 // 70
    FileCaseSensitiveInformation, // q; s: FILE_CASE_SENSITIVE_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES)
    FileLinkInformationEx, // s: FILE_LINK_INFORMATION_EX // since REDSTONE5
    FileLinkInformationExBypassAccessCheck, // (kernel-mode only); s: FILE_LINK_INFORMATION_EX
    FileStorageReserveIdInformation, // q; s: FILE_STORAGE_RESERVE_ID_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES)
    FileCaseSensitiveInformationForceAccessCheck, // q; s: FILE_CASE_SENSITIVE_INFORMATION 
    FileKnownFolderInformation, // q; s: FILE_KNOWN_FOLDER_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES) // since WIN11
    FileStatBasicInformation, // since 23H2
    FileId64ExtdDirectoryInformation,
    FileId64ExtdBothDirectoryInformation,
    FileIdAllExtdDirectoryInformation,
    FileIdAllExtdBothDirectoryInformation,
    FileMaximumInformation
} FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;

// NtQueryInformationFile/NtSetInformationFile types

typedef struct _FILE_BASIC_INFORMATION
{
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION, * PFILE_BASIC_INFORMATION;

typedef struct _FILE_STANDARD_INFORMATION
{
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_INFORMATION, * PFILE_STANDARD_INFORMATION;

#if (NTDDI_VERSION >= NTDDI_WINTHRESHOLD)
typedef struct _FILE_STANDARD_INFORMATION_EX
{
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
    BOOLEAN AlternateStream;
    BOOLEAN MetadataAttribute;
} FILE_STANDARD_INFORMATION_EX, * PFILE_STANDARD_INFORMATION_EX;
#endif

typedef struct _FILE_INTERNAL_INFORMATION
{
    union
    {
        LARGE_INTEGER IndexNumber;
        struct
        {
            LONGLONG MftRecordIndex : 48; // rev
            LONGLONG SequenceNumber : 16; // rev
        };
    };
} FILE_INTERNAL_INFORMATION, * PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_EA_INFORMATION
{
    ULONG EaSize;
} FILE_EA_INFORMATION, * PFILE_EA_INFORMATION;

typedef struct _FILE_ACCESS_INFORMATION
{
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, * PFILE_ACCESS_INFORMATION;

typedef struct _FILE_POSITION_INFORMATION
{
    LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, * PFILE_POSITION_INFORMATION;

typedef struct _FILE_MODE_INFORMATION
{
    ULONG Mode;
} FILE_MODE_INFORMATION, * PFILE_MODE_INFORMATION;

typedef struct _FILE_ALIGNMENT_INFORMATION
{
    ULONG AlignmentRequirement;
} FILE_ALIGNMENT_INFORMATION, * PFILE_ALIGNMENT_INFORMATION;

typedef struct _FILE_NAME_INFORMATION
{
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAME_INFORMATION, * PFILE_NAME_INFORMATION;

typedef struct _FILE_ALL_INFORMATION
{
    FILE_BASIC_INFORMATION BasicInformation;
    FILE_STANDARD_INFORMATION StandardInformation;
    FILE_INTERNAL_INFORMATION InternalInformation;
    FILE_EA_INFORMATION EaInformation;
    FILE_ACCESS_INFORMATION AccessInformation;
    FILE_POSITION_INFORMATION PositionInformation;
    FILE_MODE_INFORMATION ModeInformation;
    FILE_ALIGNMENT_INFORMATION AlignmentInformation;
    FILE_NAME_INFORMATION NameInformation;
} FILE_ALL_INFORMATION, * PFILE_ALL_INFORMATION;

typedef struct _FILE_NETWORK_OPEN_INFORMATION
{
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, * PFILE_NETWORK_OPEN_INFORMATION;

typedef struct _FILE_ATTRIBUTE_TAG_INFORMATION
{
    ULONG FileAttributes;
    ULONG ReparseTag;
} FILE_ATTRIBUTE_TAG_INFORMATION, * PFILE_ATTRIBUTE_TAG_INFORMATION;

typedef struct _FILE_ALLOCATION_INFORMATION
{
    LARGE_INTEGER AllocationSize;
} FILE_ALLOCATION_INFORMATION, * PFILE_ALLOCATION_INFORMATION;

typedef struct _FILE_COMPRESSION_INFORMATION
{
    LARGE_INTEGER CompressedFileSize;
    USHORT CompressionFormat;
    UCHAR CompressionUnitShift;
    UCHAR ChunkShift;
    UCHAR ClusterShift;
    UCHAR Reserved[3];
} FILE_COMPRESSION_INFORMATION, * PFILE_COMPRESSION_INFORMATION;

typedef struct _FILE_DISPOSITION_INFORMATION
{
    BOOLEAN DeleteFile;
} FILE_DISPOSITION_INFORMATION, * PFILE_DISPOSITION_INFORMATION;

#if (NTDDI_VERSION >= NTDDI_WIN10_RS1)
#define FILE_DISPOSITION_DO_NOT_DELETE              0x00000000
#define FILE_DISPOSITION_DELETE                     0x00000001
#define FILE_DISPOSITION_POSIX_SEMANTICS            0x00000002
#define FILE_DISPOSITION_FORCE_IMAGE_SECTION_CHECK  0x00000004
#define FILE_DISPOSITION_ON_CLOSE                   0x00000008
#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
#define FILE_DISPOSITION_IGNORE_READONLY_ATTRIBUTE  0x00000010
#endif

typedef struct _FILE_DISPOSITION_INFORMATION_EX
{
    ULONG Flags;
} FILE_DISPOSITION_INFORMATION_EX, * PFILE_DISPOSITION_INFORMATION_EX;
#endif

typedef struct _FILE_END_OF_FILE_INFORMATION
{
    LARGE_INTEGER EndOfFile;
} FILE_END_OF_FILE_INFORMATION, * PFILE_END_OF_FILE_INFORMATION;

#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
#define FLAGS_END_OF_FILE_INFO_EX_EXTEND_PAGING             0x00000001
#define FLAGS_END_OF_FILE_INFO_EX_NO_EXTRA_PAGING_EXTEND    0x00000002
#define FLAGS_END_OF_FILE_INFO_EX_TIME_CONSTRAINED          0x00000004

#define FLAGS_DELAY_REASONS_LOG_FILE_FULL   0x00000001
#define FLAGS_DELAY_REASONS_BITMAP_SCANNED  0x00000002

typedef struct _FILE_END_OF_FILE_INFORMATION_EX
{
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER PagingFileSizeInMM;
    LARGE_INTEGER PagingFileMaxSize;
    ULONG Flags;
} FILE_END_OF_FILE_INFORMATION_EX, * PFILE_END_OF_FILE_INFORMATION_EX;
#endif

typedef struct _FILE_VALID_DATA_LENGTH_INFORMATION
{
    LARGE_INTEGER ValidDataLength;
} FILE_VALID_DATA_LENGTH_INFORMATION, * PFILE_VALID_DATA_LENGTH_INFORMATION;

#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
#define FILE_LINK_REPLACE_IF_EXISTS                     0x00000001
#define FILE_LINK_POSIX_SEMANTICS                       0x00000002
#endif

//  available                                           0x00000004

#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
#define FILE_LINK_SUPPRESS_STORAGE_RESERVE_INHERITANCE  0x00000008
#define FILE_LINK_NO_INCREASE_AVAILABLE_SPACE           0x00000010
#define FILE_LINK_NO_DECREASE_AVAILABLE_SPACE           0x00000020
#define FILE_LINK_PRESERVE_AVAILABLE_SPACE              0x00000030  // combination
#define FILE_LINK_IGNORE_READONLY_ATTRIBUTE             0x00000040
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_19H1)
#define FILE_LINK_FORCE_RESIZE_TARGET_SR                0x00000080
#define FILE_LINK_FORCE_RESIZE_SOURCE_SR                0x00000100
#define FILE_LINK_FORCE_RESIZE_SR                       0x00000180  // combination
#endif

typedef struct _FILE_LINK_INFORMATION
{
#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
    union
    {
        BOOLEAN ReplaceIfExists; // FileLinkInformation
        ULONG Flags; // FileLinkInformationEx
    };
#else
    BOOLEAN ReplaceIfExists;
#endif
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_LINK_INFORMATION, * PFILE_LINK_INFORMATION;

typedef struct _FILE_MOVE_CLUSTER_INFORMATION
{
    ULONG ClusterCount;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_MOVE_CLUSTER_INFORMATION, * PFILE_MOVE_CLUSTER_INFORMATION;

#if (NTDDI_VERSION >= NTDDI_WIN10_RS1)
#define FILE_RENAME_REPLACE_IF_EXISTS                     0x00000001
#define FILE_RENAME_POSIX_SEMANTICS                       0x00000002
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_RS3)
#define FILE_RENAME_SUPPRESS_PIN_STATE_INHERITANCE        0x00000004
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
#define FILE_RENAME_SUPPRESS_STORAGE_RESERVE_INHERITANCE  0x00000008
#define FILE_RENAME_NO_INCREASE_AVAILABLE_SPACE           0x00000010
#define FILE_RENAME_NO_DECREASE_AVAILABLE_SPACE           0x00000020
#define FILE_RENAME_PRESERVE_AVAILABLE_SPACE              0x00000030  // combination
#define FILE_RENAME_IGNORE_READONLY_ATTRIBUTE             0x00000040
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_19H1)
#define FILE_RENAME_FORCE_RESIZE_TARGET_SR                0x00000080
#define FILE_RENAME_FORCE_RESIZE_SOURCE_SR                0x00000100
#define FILE_RENAME_FORCE_RESIZE_SR                       0x00000180  // combination
#endif

typedef struct _FILE_RENAME_INFORMATION
{
#if (NTDDI_VERSION >= NTDDI_WIN10_RS1)
    union
    {
        BOOLEAN ReplaceIfExists;  // FileRenameInformation
        ULONG Flags;              // FileRenameInformationEx
    } DUMMYUNIONNAME;
#else
    BOOLEAN ReplaceIfExists;
#endif
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_RENAME_INFORMATION, * PFILE_RENAME_INFORMATION;

typedef struct _FILE_STREAM_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG StreamNameLength;
    LARGE_INTEGER StreamSize;
    LARGE_INTEGER StreamAllocationSize;
    WCHAR StreamName[1];
} FILE_STREAM_INFORMATION, * PFILE_STREAM_INFORMATION;

typedef struct _FILE_TRACKING_INFORMATION
{
    HANDLE DestinationFile;
    ULONG ObjectInformationLength;
    CHAR ObjectInformation[1];
} FILE_TRACKING_INFORMATION, * PFILE_TRACKING_INFORMATION;

typedef struct _FILE_COMPLETION_INFORMATION
{
    HANDLE Port;
    PVOID Key;
} FILE_COMPLETION_INFORMATION, * PFILE_COMPLETION_INFORMATION;

typedef struct _FILE_PIPE_INFORMATION
{
    ULONG ReadMode;
    ULONG CompletionMode;
} FILE_PIPE_INFORMATION, * PFILE_PIPE_INFORMATION;

typedef struct _FILE_PIPE_LOCAL_INFORMATION
{
    ULONG NamedPipeType;
    ULONG NamedPipeConfiguration;
    ULONG MaximumInstances;
    ULONG CurrentInstances;
    ULONG InboundQuota;
    ULONG ReadDataAvailable;
    ULONG OutboundQuota;
    ULONG WriteQuotaAvailable;
    ULONG NamedPipeState;
    ULONG NamedPipeEnd;
} FILE_PIPE_LOCAL_INFORMATION, * PFILE_PIPE_LOCAL_INFORMATION;

typedef struct _FILE_PIPE_REMOTE_INFORMATION
{
    LARGE_INTEGER CollectDataTime;
    ULONG MaximumCollectionCount;
} FILE_PIPE_REMOTE_INFORMATION, * PFILE_PIPE_REMOTE_INFORMATION;

typedef struct _FILE_MAILSLOT_QUERY_INFORMATION
{
    ULONG MaximumMessageSize;
    ULONG MailslotQuota;
    ULONG NextMessageSize;
    ULONG MessagesAvailable;
    LARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_QUERY_INFORMATION, * PFILE_MAILSLOT_QUERY_INFORMATION;

typedef struct _FILE_MAILSLOT_SET_INFORMATION
{
    PLARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_SET_INFORMATION, * PFILE_MAILSLOT_SET_INFORMATION;

typedef struct _FILE_REPARSE_POINT_INFORMATION
{
    LONGLONG FileReference;
    ULONG Tag;
} FILE_REPARSE_POINT_INFORMATION, * PFILE_REPARSE_POINT_INFORMATION;

typedef struct _FILE_LINK_ENTRY_INFORMATION
{
    ULONG NextEntryOffset;
    LONGLONG ParentFileId; // LARGE_INTEGER
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_LINK_ENTRY_INFORMATION, * PFILE_LINK_ENTRY_INFORMATION;

typedef struct _FILE_LINKS_INFORMATION
{
    ULONG BytesNeeded;
    ULONG EntriesReturned;
    FILE_LINK_ENTRY_INFORMATION Entry;
} FILE_LINKS_INFORMATION, * PFILE_LINKS_INFORMATION;

typedef struct _FILE_NETWORK_PHYSICAL_NAME_INFORMATION
{
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NETWORK_PHYSICAL_NAME_INFORMATION, * PFILE_NETWORK_PHYSICAL_NAME_INFORMATION;

typedef struct _FILE_STANDARD_LINK_INFORMATION
{
    ULONG NumberOfAccessibleLinks;
    ULONG TotalNumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_LINK_INFORMATION, * PFILE_STANDARD_LINK_INFORMATION;

typedef struct _FILE_SFIO_RESERVE_INFORMATION
{
    ULONG RequestsPerPeriod;
    ULONG Period;
    BOOLEAN RetryFailures;
    BOOLEAN Discardable;
    ULONG RequestSize;
    ULONG NumOutstandingRequests;
} FILE_SFIO_RESERVE_INFORMATION, * PFILE_SFIO_RESERVE_INFORMATION;

typedef struct _FILE_SFIO_VOLUME_INFORMATION
{
    ULONG MaximumRequestsPerPeriod;
    ULONG MinimumPeriod;
    ULONG MinimumTransferSize;
} FILE_SFIO_VOLUME_INFORMATION, * PFILE_SFIO_VOLUME_INFORMATION;

typedef enum _IO_PRIORITY_HINT
{
    IoPriorityVeryLow = 0, // Defragging, content indexing and other background I/Os.
    IoPriorityLow, // Prefetching for applications.
    IoPriorityNormal, // Normal I/Os.
    IoPriorityHigh, // Used by filesystems for checkpoint I/O.
    IoPriorityCritical, // Used by memory manager. Not available for applications.
    MaxIoPriorityTypes
} IO_PRIORITY_HINT;

typedef struct DECLSPEC_ALIGN(8) _FILE_IO_PRIORITY_HINT_INFORMATION
{
    IO_PRIORITY_HINT PriorityHint;
} FILE_IO_PRIORITY_HINT_INFORMATION, * PFILE_IO_PRIORITY_HINT_INFORMATION;

typedef struct _FILE_IO_PRIORITY_HINT_INFORMATION_EX
{
    IO_PRIORITY_HINT PriorityHint;
    BOOLEAN BoostOutstanding;
} FILE_IO_PRIORITY_HINT_INFORMATION_EX, * PFILE_IO_PRIORITY_HINT_INFORMATION_EX;

#define FILE_SKIP_COMPLETION_PORT_ON_SUCCESS    0x1
#define FILE_SKIP_SET_EVENT_ON_HANDLE           0x2
#define FILE_SKIP_SET_USER_EVENT_ON_FAST_IO     0x4

typedef struct _FILE_IO_COMPLETION_NOTIFICATION_INFORMATION
{
    ULONG Flags;
} FILE_IO_COMPLETION_NOTIFICATION_INFORMATION, * PFILE_IO_COMPLETION_NOTIFICATION_INFORMATION;

typedef struct _FILE_PROCESS_IDS_USING_FILE_INFORMATION
{
    ULONG NumberOfProcessIdsInList;
    ULONG_PTR ProcessIdList[1];
} FILE_PROCESS_IDS_USING_FILE_INFORMATION, * PFILE_PROCESS_IDS_USING_FILE_INFORMATION;

typedef struct _FILE_IS_REMOTE_DEVICE_INFORMATION
{
    BOOLEAN IsRemote;
} FILE_IS_REMOTE_DEVICE_INFORMATION, * PFILE_IS_REMOTE_DEVICE_INFORMATION;

typedef struct _FILE_NUMA_NODE_INFORMATION
{
    USHORT NodeNumber;
} FILE_NUMA_NODE_INFORMATION, * PFILE_NUMA_NODE_INFORMATION;

typedef struct _FILE_IOSTATUSBLOCK_RANGE_INFORMATION
{
    PUCHAR IoStatusBlockRange;
    ULONG Length;
} FILE_IOSTATUSBLOCK_RANGE_INFORMATION, * PFILE_IOSTATUSBLOCK_RANGE_INFORMATION;

// Win32 FILE_REMOTE_PROTOCOL_INFO
typedef struct _FILE_REMOTE_PROTOCOL_INFORMATION
{
    // Structure Version
    USHORT StructureVersion;     // 1 for Win7, 2 for Win8 SMB3, 3 for Blue SMB3, 4 for RS5
    USHORT StructureSize;        // sizeof(FILE_REMOTE_PROTOCOL_INFORMATION)

    ULONG Protocol;             // Protocol (WNNC_NET_*) defined in winnetwk.h or ntifs.h.

    // Protocol Version & Type
    USHORT ProtocolMajorVersion;
    USHORT ProtocolMinorVersion;
    USHORT ProtocolRevision;

    USHORT Reserved;

    // Protocol-Generic Information
    ULONG Flags;

    struct
    {
        ULONG Reserved[8];
    } GenericReserved;

    // Protocol specific information

#if (NTDDI_VERSION < NTDDI_WIN8)
    struct
    {
        ULONG Reserved[16];
    } ProtocolSpecificReserved;
#endif

#if (NTDDI_VERSION >= NTDDI_WIN8)
    union
    {
        struct
        {
            struct
            {
                ULONG Capabilities;
            } Server;
            struct
            {
                ULONG Capabilities;
#if (NTDDI_VERSION >= NTDDI_WIN10_FE)
                ULONG ShareFlags;
#else
                ULONG CachingFlags;
#endif
#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
                UCHAR ShareType;
                UCHAR Reserved0[3];
                ULONG Reserved1;
#endif
            } Share;
        } Smb2;
        ULONG Reserved[16];
    } ProtocolSpecific;
#endif
} FILE_REMOTE_PROTOCOL_INFORMATION, * PFILE_REMOTE_PROTOCOL_INFORMATION;

#define CHECKSUM_ENFORCEMENT_OFF 0x00000001

typedef struct _FILE_INTEGRITY_STREAM_INFORMATION
{
    USHORT ChecksumAlgorithm;
    UCHAR ChecksumChunkShift;
    UCHAR ClusterShift;
    ULONG Flags;
} FILE_INTEGRITY_STREAM_INFORMATION, * PFILE_INTEGRITY_STREAM_INFORMATION;

typedef struct _FILE_VOLUME_NAME_INFORMATION
{
    ULONG DeviceNameLength;
    WCHAR DeviceName[1];
} FILE_VOLUME_NAME_INFORMATION, * PFILE_VOLUME_NAME_INFORMATION;

#define FILE_ID_IS_INVALID(FID) ((FID).QuadPart == FILE_INVALID_FILE_ID)

#define FILE_ID_128_IS_INVALID(FID128) (((FID128).Identifier[0] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[1] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[2] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[3] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[4] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[5] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[6] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[7] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[8] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[9] == (UCHAR)-1) &&    \
                                        ((FID128).Identifier[10] == (UCHAR)-1) &&   \
                                        ((FID128).Identifier[11] == (UCHAR)-1) &&   \
                                        ((FID128).Identifier[12] == (UCHAR)-1) &&   \
                                        ((FID128).Identifier[13] == (UCHAR)-1) &&   \
                                        ((FID128).Identifier[14] == (UCHAR)-1) &&   \
                                        ((FID128).Identifier[15] == (UCHAR)-1))

#define MAKE_INVALID_FILE_ID_128(FID128) {  \
    ((FID128).Identifier[0] = (UCHAR)-1);   \
    ((FID128).Identifier[1] = (UCHAR)-1);   \
    ((FID128).Identifier[2] = (UCHAR)-1);   \
    ((FID128).Identifier[3] = (UCHAR)-1);   \
    ((FID128).Identifier[4] = (UCHAR)-1);   \
    ((FID128).Identifier[5] = (UCHAR)-1);   \
    ((FID128).Identifier[6] = (UCHAR)-1);   \
    ((FID128).Identifier[7] = (UCHAR)-1);   \
    ((FID128).Identifier[8] = (UCHAR)-1);   \
    ((FID128).Identifier[9] = (UCHAR)-1);   \
    ((FID128).Identifier[10] = (UCHAR)-1);  \
    ((FID128).Identifier[11] = (UCHAR)-1);  \
    ((FID128).Identifier[12] = (UCHAR)-1);  \
    ((FID128).Identifier[13] = (UCHAR)-1);  \
    ((FID128).Identifier[14] = (UCHAR)-1);  \
    ((FID128).Identifier[15] = (UCHAR)-1);  \
}

typedef struct _FILE_ID_INFORMATION
{
    ULONGLONG VolumeSerialNumber;
    union
    {
        FILE_ID_128 FileId;
        struct
        {
            LONGLONG FileIdLowPart : 64; // rev
            LONGLONG FileIdHighPart : 64; // rev
        };
    };
} FILE_ID_INFORMATION, * PFILE_ID_INFORMATION;

typedef struct _FILE_ID_EXTD_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    ULONG ReparsePointTag;
    FILE_ID_128 FileId;
    WCHAR FileName[1];
} FILE_ID_EXTD_DIR_INFORMATION, * PFILE_ID_EXTD_DIR_INFORMATION;

typedef struct _FILE_LINK_ENTRY_FULL_ID_INFORMATION
{
    ULONG NextEntryOffset;
    FILE_ID_128 ParentFileId;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_LINK_ENTRY_FULL_ID_INFORMATION, * PFILE_LINK_ENTRY_FULL_ID_INFORMATION;

typedef struct _FILE_LINKS_FULL_ID_INFORMATION
{
    ULONG BytesNeeded;
    ULONG EntriesReturned;
    FILE_LINK_ENTRY_FULL_ID_INFORMATION Entry;
} FILE_LINKS_FULL_ID_INFORMATION, * PFILE_LINKS_FULL_ID_INFORMATION;

typedef struct _FILE_ID_EXTD_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    ULONG ReparsePointTag;
    FILE_ID_128 FileId;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_ID_EXTD_BOTH_DIR_INFORMATION, * PFILE_ID_EXTD_BOTH_DIR_INFORMATION;

typedef struct _FILE_STAT_INFORMATION
{
    LARGE_INTEGER FileId;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
    ULONG ReparseTag;
    ULONG NumberOfLinks;
    ACCESS_MASK EffectiveAccess;
} FILE_STAT_INFORMATION, * PFILE_STAT_INFORMATION;

typedef struct _FILE_MEMORY_PARTITION_INFORMATION
{
    HANDLE OwnerPartitionHandle;
    union
    {
        struct
        {
            UCHAR NoCrossPartitionAccess;
            UCHAR Spare[3];
        };
        ULONG AllFlags;
    } Flags;
} FILE_MEMORY_PARTITION_INFORMATION, * PFILE_MEMORY_PARTITION_INFORMATION;

// LxFlags
#define LX_FILE_METADATA_HAS_UID        0x1
#define LX_FILE_METADATA_HAS_GID        0x2
#define LX_FILE_METADATA_HAS_MODE       0x4
#define LX_FILE_METADATA_HAS_DEVICE_ID  0x8
#define LX_FILE_CASE_SENSITIVE_DIR      0x10

// private
typedef struct _FILE_STAT_LX_INFORMATION
{
    LARGE_INTEGER FileId;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
    ULONG ReparseTag;
    ULONG NumberOfLinks;
    ACCESS_MASK EffectiveAccess;
    ULONG LxFlags;
    ULONG LxUid;
    ULONG LxGid;
    ULONG LxMode;
    ULONG LxDeviceIdMajor;
    ULONG LxDeviceIdMinor;
} FILE_STAT_LX_INFORMATION, * PFILE_STAT_LX_INFORMATION;

typedef struct _FILE_STORAGE_RESERVE_ID_INFORMATION
{
    STORAGE_RESERVE_ID StorageReserveId;
} FILE_STORAGE_RESERVE_ID_INFORMATION, * PFILE_STORAGE_RESERVE_ID_INFORMATION;

#define FILE_CS_FLAG_CASE_SENSITIVE_DIR 0x00000001

typedef struct _FILE_CASE_SENSITIVE_INFORMATION
{
    ULONG Flags;
} FILE_CASE_SENSITIVE_INFORMATION, * PFILE_CASE_SENSITIVE_INFORMATION;

typedef enum _FILE_KNOWN_FOLDER_TYPE
{
    KnownFolderNone,
    KnownFolderDesktop,
    KnownFolderDocuments,
    KnownFolderDownloads,
    KnownFolderMusic,
    KnownFolderPictures,
    KnownFolderVideos,
    KnownFolderOther,
    KnownFolderMax = 7
} FILE_KNOWN_FOLDER_TYPE;

typedef struct _FILE_KNOWN_FOLDER_INFORMATION
{
    FILE_KNOWN_FOLDER_TYPE Type;
} FILE_KNOWN_FOLDER_INFORMATION, * PFILE_KNOWN_FOLDER_INFORMATION;

// NtQueryDirectoryFile types

typedef struct _FILE_DIRECTORY_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_DIRECTORY_INFORMATION, * PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    WCHAR FileName[1];
} FILE_FULL_DIR_INFORMATION, * PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_ID_FULL_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_FULL_DIR_INFORMATION, * PFILE_ID_FULL_DIR_INFORMATION;

typedef struct _FILE_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_BOTH_DIR_INFORMATION, * PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_ID_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, * PFILE_ID_BOTH_DIR_INFORMATION;

typedef struct _FILE_NAMES_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAMES_INFORMATION, * PFILE_NAMES_INFORMATION;

typedef struct _FILE_ID_GLOBAL_TX_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    LARGE_INTEGER FileId;
    GUID LockingTransactionId;
    ULONG TxInfoFlags;
    WCHAR FileName[1];
} FILE_ID_GLOBAL_TX_DIR_INFORMATION, * PFILE_ID_GLOBAL_TX_DIR_INFORMATION;

#define FILE_ID_GLOBAL_TX_DIR_INFO_FLAG_WRITELOCKED         0x00000001
#define FILE_ID_GLOBAL_TX_DIR_INFO_FLAG_VISIBLE_TO_TX       0x00000002
#define FILE_ID_GLOBAL_TX_DIR_INFO_FLAG_VISIBLE_OUTSIDE_TX  0x00000004

typedef struct _FILE_OBJECTID_INFORMATION
{
    LONGLONG FileReference;
    UCHAR ObjectId[16]; // GUID
    union
    {
        struct
        {
            UCHAR BirthVolumeId[16];
            UCHAR BirthObjectId[16];
            UCHAR DomainId[16];
        };
        UCHAR ExtendedInfo[48];
    };
} FILE_OBJECTID_INFORMATION, * PFILE_OBJECTID_INFORMATION;

typedef struct _FILE_DIRECTORY_NEXT_INFORMATION
{
    ULONG NextEntryOffset;
} FILE_DIRECTORY_NEXT_INFORMATION, * PFILE_DIRECTORY_NEXT_INFORMATION;

// NtQueryEaFile/NtSetEaFile types

typedef struct _FILE_FULL_EA_INFORMATION
{
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[1];
} FILE_FULL_EA_INFORMATION, * PFILE_FULL_EA_INFORMATION;

typedef struct _FILE_GET_EA_INFORMATION
{
    ULONG NextEntryOffset;
    UCHAR EaNameLength;
    CHAR EaName[1];
} FILE_GET_EA_INFORMATION, * PFILE_GET_EA_INFORMATION;

// NtQueryQuotaInformationFile/NtSetQuotaInformationFile types

typedef struct _FILE_GET_QUOTA_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG SidLength;
    SID Sid;
} FILE_GET_QUOTA_INFORMATION, * PFILE_GET_QUOTA_INFORMATION;

typedef struct _FILE_QUOTA_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG SidLength;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER QuotaUsed;
    LARGE_INTEGER QuotaThreshold;
    LARGE_INTEGER QuotaLimit;
    SID Sid;
} FILE_QUOTA_INFORMATION, * PFILE_QUOTA_INFORMATION;

typedef enum _FSINFOCLASS
{
    FileFsVolumeInformation = 1, // q: FILE_FS_VOLUME_INFORMATION
    FileFsLabelInformation, // s: FILE_FS_LABEL_INFORMATION (requires FILE_WRITE_DATA to volume)
    FileFsSizeInformation, // q: FILE_FS_SIZE_INFORMATION
    FileFsDeviceInformation, // q: FILE_FS_DEVICE_INFORMATION
    FileFsAttributeInformation, // q: FILE_FS_ATTRIBUTE_INFORMATION
    FileFsControlInformation, // q, s: FILE_FS_CONTROL_INFORMATION  (q: requires FILE_READ_DATA; s: requires FILE_WRITE_DATA to volume)
    FileFsFullSizeInformation, // q: FILE_FS_FULL_SIZE_INFORMATION
    FileFsObjectIdInformation, // q; s: FILE_FS_OBJECTID_INFORMATION (s: requires FILE_WRITE_DATA to volume)
    FileFsDriverPathInformation, // q: FILE_FS_DRIVER_PATH_INFORMATION
    FileFsVolumeFlagsInformation, // q; s: FILE_FS_VOLUME_FLAGS_INFORMATION (q: requires FILE_READ_ATTRIBUTES; s: requires FILE_WRITE_ATTRIBUTES to volume) // 10
    FileFsSectorSizeInformation, // q: FILE_FS_SECTOR_SIZE_INFORMATION // since WIN8
    FileFsDataCopyInformation, // q: FILE_FS_DATA_COPY_INFORMATION
    FileFsMetadataSizeInformation, // q: FILE_FS_METADATA_SIZE_INFORMATION // since THRESHOLD
    FileFsFullSizeInformationEx, // q: FILE_FS_FULL_SIZE_INFORMATION_EX // since REDSTONE5
    FileFsGuidInformation, // q: FILE_FS_GUID_INFORMATION // since 23H2
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, * PFS_INFORMATION_CLASS;

// NtQueryVolumeInformation/NtSetVolumeInformation types

typedef struct _FILE_FS_VOLUME_INFORMATION
{
    LARGE_INTEGER VolumeCreationTime;
    ULONG VolumeSerialNumber;
    ULONG VolumeLabelLength;
    BOOLEAN SupportsObjects;
    WCHAR VolumeLabel[1];
} FILE_FS_VOLUME_INFORMATION, * PFILE_FS_VOLUME_INFORMATION;

typedef struct _FILE_FS_LABEL_INFORMATION
{
    ULONG VolumeLabelLength;
    WCHAR VolumeLabel[1];
} FILE_FS_LABEL_INFORMATION, * PFILE_FS_LABEL_INFORMATION;

typedef struct _FILE_FS_SIZE_INFORMATION
{
    LARGE_INTEGER TotalAllocationUnits;
    LARGE_INTEGER AvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_SIZE_INFORMATION, * PFILE_FS_SIZE_INFORMATION;

// FileSystemControlFlags
#define FILE_VC_QUOTA_NONE              0x00000000
#define FILE_VC_QUOTA_TRACK             0x00000001
#define FILE_VC_QUOTA_ENFORCE           0x00000002
#define FILE_VC_QUOTA_MASK              0x00000003
#define FILE_VC_CONTENT_INDEX_DISABLED  0x00000008
#define FILE_VC_LOG_QUOTA_THRESHOLD     0x00000010
#define FILE_VC_LOG_QUOTA_LIMIT         0x00000020
#define FILE_VC_LOG_VOLUME_THRESHOLD    0x00000040
#define FILE_VC_LOG_VOLUME_LIMIT        0x00000080
#define FILE_VC_QUOTAS_INCOMPLETE       0x00000100
#define FILE_VC_QUOTAS_REBUILDING       0x00000200
#define FILE_VC_VALID_MASK              0x000003ff

typedef struct _FILE_FS_CONTROL_INFORMATION
{
    LARGE_INTEGER FreeSpaceStartFiltering;
    LARGE_INTEGER FreeSpaceThreshold;
    LARGE_INTEGER FreeSpaceStopFiltering;
    LARGE_INTEGER DefaultQuotaThreshold;
    LARGE_INTEGER DefaultQuotaLimit;
    ULONG FileSystemControlFlags;
} FILE_FS_CONTROL_INFORMATION, * PFILE_FS_CONTROL_INFORMATION;

typedef struct _FILE_FS_FULL_SIZE_INFORMATION
{
    LARGE_INTEGER TotalAllocationUnits;
    LARGE_INTEGER CallerAvailableAllocationUnits;
    LARGE_INTEGER ActualAvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_FULL_SIZE_INFORMATION, * PFILE_FS_FULL_SIZE_INFORMATION;

typedef struct _FILE_FS_OBJECTID_INFORMATION
{
    UCHAR ObjectId[16];
    union
    {
        struct
        {
            UCHAR BirthVolumeId[16];
            UCHAR BirthObjectId[16];
            UCHAR DomainId[16];
        };
        UCHAR ExtendedInfo[48];
    };
} FILE_FS_OBJECTID_INFORMATION, * PFILE_FS_OBJECTID_INFORMATION;

typedef struct _FILE_FS_DEVICE_INFORMATION
{
    DEVICE_TYPE DeviceType;
    ULONG Characteristics;
} FILE_FS_DEVICE_INFORMATION, * PFILE_FS_DEVICE_INFORMATION;

typedef struct _FILE_FS_ATTRIBUTE_INFORMATION
{
    ULONG FileSystemAttributes;
    LONG MaximumComponentNameLength;
    ULONG FileSystemNameLength;
    WCHAR FileSystemName[1];
} FILE_FS_ATTRIBUTE_INFORMATION, * PFILE_FS_ATTRIBUTE_INFORMATION;

typedef struct _FILE_FS_DRIVER_PATH_INFORMATION
{
    BOOLEAN DriverInPath;
    ULONG DriverNameLength;
    WCHAR DriverName[1];
} FILE_FS_DRIVER_PATH_INFORMATION, * PFILE_FS_DRIVER_PATH_INFORMATION;

typedef struct _FILE_FS_VOLUME_FLAGS_INFORMATION
{
    ULONG Flags;
} FILE_FS_VOLUME_FLAGS_INFORMATION, * PFILE_FS_VOLUME_FLAGS_INFORMATION;

#define SSINFO_FLAGS_ALIGNED_DEVICE                 0x00000001
#define SSINFO_FLAGS_PARTITION_ALIGNED_ON_DEVICE    0x00000002
#define SSINFO_FLAGS_NO_SEEK_PENALTY                0x00000004
#define SSINFO_FLAGS_TRIM_ENABLED                   0x00000008
#define SSINFO_FLAGS_BYTE_ADDRESSABLE               0x00000010 // since REDSTONE

// If set for Sector and Partition fields, alignment is not known.
#define SSINFO_OFFSET_UNKNOWN 0xffffffff

typedef struct _FILE_FS_SECTOR_SIZE_INFORMATION
{
    ULONG LogicalBytesPerSector;
    ULONG PhysicalBytesPerSectorForAtomicity;
    ULONG PhysicalBytesPerSectorForPerformance;
    ULONG FileSystemEffectivePhysicalBytesPerSectorForAtomicity;
    ULONG Flags; // SSINFO_FLAGS_*
    ULONG ByteOffsetForSectorAlignment;
    ULONG ByteOffsetForPartitionAlignment;
} FILE_FS_SECTOR_SIZE_INFORMATION, * PFILE_FS_SECTOR_SIZE_INFORMATION;

typedef struct _FILE_FS_DATA_COPY_INFORMATION
{
    ULONG NumberOfCopies;
} FILE_FS_DATA_COPY_INFORMATION, * PFILE_FS_DATA_COPY_INFORMATION;

typedef struct _FILE_FS_METADATA_SIZE_INFORMATION
{
    LARGE_INTEGER TotalMetadataAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_METADATA_SIZE_INFORMATION, * PFILE_FS_METADATA_SIZE_INFORMATION;

typedef struct _FILE_FS_FULL_SIZE_INFORMATION_EX
{
    ULONGLONG ActualTotalAllocationUnits;
    ULONGLONG ActualAvailableAllocationUnits;
    ULONGLONG ActualPoolUnavailableAllocationUnits;
    ULONGLONG CallerTotalAllocationUnits;
    ULONGLONG CallerAvailableAllocationUnits;
    ULONGLONG CallerPoolUnavailableAllocationUnits;
    ULONGLONG UsedAllocationUnits;
    ULONGLONG TotalReservedAllocationUnits;
    ULONGLONG VolumeStorageReserveAllocationUnits;
    ULONGLONG AvailableCommittedAllocationUnits;
    ULONGLONG PoolAvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_FULL_SIZE_INFORMATION_EX, * PFILE_FS_FULL_SIZE_INFORMATION_EX;

#if defined(NTDDI_WIN10_NI) && (NTDDI_VERSION >= NTDDI_WIN10_NI)

typedef struct _FILE_FS_GUID_INFORMATION {
    GUID FsGuid;
} FILE_FS_GUID_INFORMATION, * PFILE_FS_GUID_INFORMATION;

#endif // #if (NTDDI_VERSION >= NTDDI_WIN10_NI)

#endif // !_KERNEL_MODE

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateFile(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_opt_ PLARGE_INTEGER AllocationSize,
    _In_ ULONG FileAttributes,
    _In_ ULONG ShareAccess,
    _In_ ULONG CreateDisposition,
    _In_ ULONG CreateOptions,
    _In_reads_bytes_opt_(EaLength) PVOID EaBuffer,
    _In_ ULONG EaLength
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateFile(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_opt_ PLARGE_INTEGER AllocationSize,
    _In_ ULONG FileAttributes,
    _In_ ULONG ShareAccess,
    _In_ ULONG CreateDisposition,
    _In_ ULONG CreateOptions,
    _In_reads_bytes_opt_(EaLength) PVOID EaBuffer,
    _In_ ULONG EaLength
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateNamedPipeFile(
    _Out_ PHANDLE FileHandle,
    _In_ ULONG DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG CreateDisposition,
    _In_ ULONG CreateOptions,
    _In_ ULONG NamedPipeType,
    _In_ ULONG ReadMode,
    _In_ ULONG CompletionMode,
    _In_ ULONG MaximumInstances,
    _In_ ULONG InboundQuota,
    _In_ ULONG OutboundQuota,
    _In_ PLARGE_INTEGER DefaultTimeout
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
NTAPI
ZwCreateNamedPipeFile(
    _Out_ PHANDLE FileHandle,
    _In_ ULONG DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG CreateDisposition,
    _In_ ULONG CreateOptions,
    _In_ ULONG NamedPipeType,
    _In_ ULONG ReadMode,
    _In_ ULONG CompletionMode,
    _In_ ULONG MaximumInstances,
    _In_ ULONG InboundQuota,
    _In_ ULONG OutboundQuota,
    _In_opt_ PLARGE_INTEGER DefaultTimeout
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateMailslotFile(
    _Out_ PHANDLE FileHandle,
    _In_ ULONG DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG CreateOptions,
    _In_ ULONG MailslotQuota,
    _In_ ULONG MaximumMessageSize,
    _In_ PLARGE_INTEGER ReadTimeout
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
NTAPI
ZwCreateMailslotFile(
    _Out_ PHANDLE FileHandle,
    _In_ ULONG DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG CreateOptions,
    _In_ ULONG MailslotQuota,
    _In_ ULONG MaximumMessageSize,
    _In_ PLARGE_INTEGER ReadTimeout
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenFile(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG OpenOptions
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenFile(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG OpenOptions
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtDeleteFile(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteFile(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
ZwFlushBuffersFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

#define FLUSH_FLAGS_FILE_DATA_ONLY      0x00000001
#define FLUSH_FLAGS_NO_SYNC             0x00000002
#define FLUSH_FLAGS_FILE_DATA_SYNC_ONLY 0x00000004 // REDSTONE1

#if (NTDDI_VERSION >= NTDDI_WIN8)
__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtFlushBuffersFileEx (
    _In_ HANDLE FileHandle,
    _In_ ULONG Flags,
    _In_reads_bytes_(ParametersSize) PVOID Parameters,
    _In_ ULONG ParametersSize,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
ZwFlushBuffersFileEx(
    _In_ HANDLE FileHandle,
    _In_ ULONG FLags,
    _In_reads_bytes_(ParametersSize) PVOID Parameters,
    _In_ ULONG ParametersSize,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);
#endif

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
);

#if (NTDDI_VERSION >= NTDDI_WIN10_RS2)
__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryInformationByName(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationByName(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
);
#endif // NTDDI_VERSION >= NTDDI_WIN10_RS2

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryDirectoryFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_opt_ PUNICODE_STRING FileName,
    _In_ BOOLEAN RestartScan
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_opt_ PUNICODE_STRING FileName,
    _In_ BOOLEAN RestartScan
);

#if (NTDDI_VERSION >= NTDDI_WIN10_RS3)

//
// QueryDirectory / QueryEa / QueryQuota (IRP_MJ_DIRECTORY_CONTROL/IRP_MJ_QUERY_EA/IRP_MJ_QUERY_QUOTA))
//

#define SL_RESTART_SCAN                 0x01
#define SL_RETURN_SINGLE_ENTRY          0x02
#define SL_INDEX_SPECIFIED              0x04
#define SL_RETURN_ON_DISK_ENTRIES_ONLY  0x08 // RS2
#if (NTDDI_VERSION >= NTDDI_WIN10_RS5)
#define SL_NO_CURSOR_UPDATE             0x10
#endif

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryDirectoryFileEx(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass,
    _In_ ULONG QueryFlags,  // Valid flags are in SL_QUERY_DIRECTORY_MASK
    _In_opt_ PUNICODE_STRING FileName
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryFileEx(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass,
    _In_ ULONG QueryFlags,
    _In_opt_ PUNICODE_STRING FileName
);
#endif // NTDDI_VERSION >= NTDDI_WIN10_RS3

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryEaFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_reads_bytes_opt_(EaListLength) PVOID EaList,
    _In_ ULONG EaListLength,
    _In_opt_ PULONG EaIndex,
    _In_ BOOLEAN RestartScan
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
ZwQueryEaFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_reads_bytes_opt_(EaListLength) PVOID EaList,
    _In_ ULONG EaListLength,
    _In_opt_ PULONG EaIndex,
    _In_ BOOLEAN RestartScan
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetEaFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
ZwSetEaFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryQuotaInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_reads_bytes_opt_(SidListLength) PVOID SidList,
    _In_ ULONG SidListLength,
    _In_reads_bytes_opt_((8 + (4 * ((SID*)StartSid)->SubAuthorityCount))) // SeLengthSid()
    PSID StartSid,
    _In_ BOOLEAN RestartScan
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryQuotaInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_reads_bytes_opt_(SidListLength) PVOID SidList,
    _In_ ULONG SidListLength,
    _In_opt_ PSID StartSid,
    _In_ BOOLEAN RestartScan
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetQuotaInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwSetQuotaInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryVolumeInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FsInformation,
    _In_ ULONG Length,
    _In_ FS_INFORMATION_CLASS FsInformationClass
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVolumeInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FsInformation,
    _In_ ULONG Length,
    _In_ FS_INFORMATION_CLASS FsInformationClass
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetVolumeInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID FsInformation,
    _In_ ULONG Length,
    _In_ FS_INFORMATION_CLASS FsInformationClass
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwSetVolumeInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID FsInformation,
    _In_ ULONG Length,
    _In_ FS_INFORMATION_CLASS FsInformationClass
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCancelIoFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCancelIoFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCancelIoFileEx(
    _In_ HANDLE FileHandle,
    _In_opt_ PIO_STATUS_BLOCK IoRequestToCancel,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCancelIoFileEx(
    _In_ HANDLE FileHandle,
    _In_opt_ PIO_STATUS_BLOCK IoRequestToCancel,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCancelSynchronousIoFile(
    _In_ HANDLE ThreadHandle,
    _In_opt_ PIO_STATUS_BLOCK IoRequestToCancel,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCancelSynchronousIoFile(
    _In_ HANDLE ThreadHandle,
    _In_opt_ PIO_STATUS_BLOCK IoRequestToCancel,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG IoControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwDeviceIoControlFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG IoControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtFsControlFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG FsControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwFsControlFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG FsControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtReadFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwReadFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtWriteFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwWriteFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

#if (NTDDI_VERSION >= NTDDI_WIN11)
__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCopyFileChunk(
    _In_ HANDLE SourceHandle,
    _In_ HANDLE DestHandle,
    _In_opt_ HANDLE Event,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG Length,
    _In_ PLARGE_INTEGER SourceOffset,
    _In_ PLARGE_INTEGER DestOffset,
    _In_opt_ PULONG SourceKey,
    _In_opt_ PULONG DestKey,
    _In_ ULONG Flags
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCopyFileChunk(
    _In_ HANDLE SourceHandle,
    _In_ HANDLE DestHandle,
    _In_opt_ HANDLE Event,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG Length,
    _In_ PLARGE_INTEGER SourceOffset,
    _In_ PLARGE_INTEGER DestOffset,
    _In_opt_ PULONG SourceKey,
    _In_opt_ PULONG DestKey,
    _In_ ULONG Flags
);
#endif // (NTDDI_VERSION >= NTDDI_WIN11)

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtReadFileScatter(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PFILE_SEGMENT_ELEMENT SegmentArray,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwReadFileScatter(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PFILE_SEGMENT_ELEMENT SegmentArray,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtWriteFileGather(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PFILE_SEGMENT_ELEMENT SegmentArray,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwWriteFileGather(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PFILE_SEGMENT_ELEMENT SegmentArray,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtLockFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PLARGE_INTEGER ByteOffset,
    _In_ PLARGE_INTEGER Length,
    _In_ ULONG Key,
    _In_ BOOLEAN FailImmediately,
    _In_ BOOLEAN ExclusiveLock
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwLockFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PLARGE_INTEGER ByteOffset,
    _In_ PLARGE_INTEGER Length,
    _In_ ULONG Key,
    _In_ BOOLEAN FailImmediately,
    _In_ BOOLEAN ExclusiveLock
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtUnlockFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PLARGE_INTEGER ByteOffset,
    _In_ PLARGE_INTEGER Length,
    _In_ ULONG Key
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwUnlockFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PLARGE_INTEGER ByteOffset,
    _In_ PLARGE_INTEGER Length,
    _In_ ULONG Key
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryAttributesFile(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PFILE_BASIC_INFORMATION FileInformation
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryAttributesFile(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PFILE_BASIC_INFORMATION FileInformation
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryFullAttributesFile(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PFILE_NETWORK_OPEN_INFORMATION FileInformation
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryFullAttributesFile(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PFILE_NETWORK_OPEN_INFORMATION FileInformation
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtNotifyChangeDirectoryFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer, // FILE_NOTIFY_INFORMATION
    _In_ ULONG Length,
    _In_ ULONG CompletionFilter,
    _In_ BOOLEAN WatchTree
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwNotifyChangeDirectoryFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer, // FILE_NOTIFY_INFORMATION
    _In_ ULONG Length,
    _In_ ULONG CompletionFilter,
    _In_ BOOLEAN WatchTree
);

#ifndef _KERNEL_MODE
typedef enum _DIRECTORY_NOTIFY_INFORMATION_CLASS
{
    DirectoryNotifyInformation = 1,     // FILE_NOTIFY_INFORMATION
    DirectoryNotifyExtendedInformation, // FILE_NOTIFY_EXTENDED_INFORMATION
#if (NTDDI_VERSION >= NTDDI_WIN10_NI)
    DirectoryNotifyFullInformation,     // since 22H2
#endif
                                        // add new classes above
    DirectoryNotifyMaximumInformation
} DIRECTORY_NOTIFY_INFORMATION_CLASS, * PDIRECTORY_NOTIFY_INFORMATION_CLASS;
#endif // !_KERNEL_MODE

#if (NTDDI_VERSION >= NTDDI_WIN10_RS3)
__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtNotifyChangeDirectoryFileEx(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_ ULONG CompletionFilter,
    _In_ BOOLEAN WatchTree,
    _In_opt_ DIRECTORY_NOTIFY_INFORMATION_CLASS DirectoryNotifyInformationClass
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwNotifyChangeDirectoryFileEx(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_ ULONG CompletionFilter,
    _In_ BOOLEAN WatchTree,
    _In_opt_ DIRECTORY_NOTIFY_INFORMATION_CLASS DirectoryNotifyInformationClass
);
#endif // NTDDI_VERSION >= NTDDI_WIN10_RS3

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtLoadDriver(
    _In_ PUNICODE_STRING DriverServiceName
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwLoadDriver(
    _In_ PUNICODE_STRING DriverServiceName
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtUnloadDriver(
    _In_ PUNICODE_STRING DriverServiceName
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadDriver(
    _In_ PUNICODE_STRING DriverServiceName
);

//
// I/O completion port
//

#define IO_COMPLETION_QUERY_STATE   0x0001
#define IO_COMPLETION_MODIFY_STATE  0x0002
#define IO_COMPLETION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3) 

typedef enum _IO_COMPLETION_INFORMATION_CLASS
{
    IoCompletionBasicInformation
} IO_COMPLETION_INFORMATION_CLASS;

typedef struct _IO_COMPLETION_BASIC_INFORMATION
{
    LONG Depth;
} IO_COMPLETION_BASIC_INFORMATION, * PIO_COMPLETION_BASIC_INFORMATION;

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateIoCompletion(
    _Out_ PHANDLE IoCompletionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ ULONG Count
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateIoCompletion(
    _Out_ PHANDLE IoCompletionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ ULONG Count
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenIoCompletion(
    _Out_ PHANDLE IoCompletionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenIoCompletion(
    _Out_ PHANDLE IoCompletionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryIoCompletion(
    _In_ HANDLE IoCompletionHandle,
    _In_ IO_COMPLETION_INFORMATION_CLASS IoCompletionInformationClass,
    _Out_writes_bytes_(IoCompletionInformationLength) PVOID IoCompletionInformation,
    _In_ ULONG IoCompletionInformationLength,
    _Out_opt_ PULONG ReturnLength
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryIoCompletion(
    _In_ HANDLE IoCompletionHandle,
    _In_ IO_COMPLETION_INFORMATION_CLASS IoCompletionInformationClass,
    _Out_writes_bytes_(IoCompletionInformationLength) PVOID IoCompletionInformation,
    _In_ ULONG IoCompletionInformationLength,
    _Out_opt_ PULONG ReturnLength
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetIoCompletion(
    _In_ HANDLE IoCompletionHandle,
    _In_opt_ PVOID KeyContext,
    _In_opt_ PVOID ApcContext,
    _In_ NTSTATUS IoStatus,
    _In_ ULONG_PTR IoStatusInformation
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwSetIoCompletion(
    _In_ HANDLE IoCompletionHandle,
    _In_opt_ PVOID KeyContext,
    _In_opt_ PVOID ApcContext,
    _In_ NTSTATUS IoStatus,
    _In_ ULONG_PTR IoStatusInformation
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetIoCompletionEx(
    _In_ HANDLE IoCompletionHandle,
    _In_ HANDLE IoCompletionPacketHandle,
    _In_opt_ PVOID KeyContext,
    _In_opt_ PVOID ApcContext,
    _In_ NTSTATUS IoStatus,
    _In_ ULONG_PTR IoStatusInformation
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwSetIoCompletionEx(
    _In_ HANDLE IoCompletionHandle,
    _In_ HANDLE IoCompletionPacketHandle,
    _In_opt_ PVOID KeyContext,
    _In_opt_ PVOID ApcContext,
    _In_ NTSTATUS IoStatus,
    _In_ ULONG_PTR IoStatusInformation
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtRemoveIoCompletion(
    _In_ HANDLE IoCompletionHandle,
    _Out_ PVOID* KeyContext,
    _Out_ PVOID* ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_opt_ PLARGE_INTEGER Timeout
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwRemoveIoCompletion(
    _In_ HANDLE IoCompletionHandle,
    _Out_ PVOID* KeyContext,
    _Out_ PVOID* ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_opt_ PLARGE_INTEGER Timeout
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtRemoveIoCompletionEx(
    _In_ HANDLE IoCompletionHandle,
    _Out_writes_to_(Count, *NumEntriesRemoved) PFILE_IO_COMPLETION_INFORMATION IoCompletionInformation,
    _In_ ULONG Count,
    _Out_ PULONG NumEntriesRemoved,
    _In_opt_ PLARGE_INTEGER Timeout,
    _In_ BOOLEAN Alertable
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwRemoveIoCompletionEx(
    _In_ HANDLE IoCompletionHandle,
    _Out_writes_to_(Count, *NumEntriesRemoved) PFILE_IO_COMPLETION_INFORMATION IoCompletionInformation,
    _In_ ULONG Count,
    _Out_ PULONG NumEntriesRemoved,
    _In_opt_ PLARGE_INTEGER Timeout,
    _In_ BOOLEAN Alertable
);

//
// Wait completion packet
//

#if (NTDDI_VERSION >= NTDDI_WIN8)
__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateWaitCompletionPacket(
    _Out_ PHANDLE WaitCompletionPacketHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateWaitCompletionPacket(
    _Out_ PHANDLE WaitCompletionPacketHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtAssociateWaitCompletionPacket(
    _In_ HANDLE WaitCompletionPacketHandle,
    _In_ HANDLE IoCompletionHandle,
    _In_ HANDLE TargetObjectHandle,
    _In_opt_ PVOID KeyContext,
    _In_opt_ PVOID ApcContext,
    _In_ NTSTATUS IoStatus,
    _In_ ULONG_PTR IoStatusInformation,
    _Out_opt_ PBOOLEAN AlreadySignaled
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwAssociateWaitCompletionPacket(
    _In_ HANDLE WaitCompletionPacketHandle,
    _In_ HANDLE IoCompletionHandle,
    _In_ HANDLE TargetObjectHandle,
    _In_opt_ PVOID KeyContext,
    _In_opt_ PVOID ApcContext,
    _In_ NTSTATUS IoStatus,
    _In_ ULONG_PTR IoStatusInformation,
    _Out_opt_ PBOOLEAN AlreadySignaled
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCancelWaitCompletionPacket(
    _In_ HANDLE WaitCompletionPacketHandle,
    _In_ BOOLEAN RemoveSignaledPacket
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCancelWaitCompletionPacket(
    _In_ HANDLE WaitCompletionPacketHandle,
    _In_ BOOLEAN RemoveSignaledPacket
);
#endif

//
// Sessions
//

#ifndef _KERNEL_MODE
typedef enum _IO_SESSION_EVENT
{
    IoSessionEventIgnore,
    IoSessionEventCreated,
    IoSessionEventTerminated,
    IoSessionEventConnected,
    IoSessionEventDisconnected,
    IoSessionEventLogon,
    IoSessionEventLogoff,
    IoSessionEventMax
} IO_SESSION_EVENT;

typedef enum _IO_SESSION_STATE
{
    IoSessionStateCreated = 1,
    IoSessionStateInitialized = 2,
    IoSessionStateConnected = 3,
    IoSessionStateDisconnected = 4,
    IoSessionStateDisconnectedLoggedOn = 5,
    IoSessionStateLoggedOn = 6,
    IoSessionStateLoggedOff = 7,
    IoSessionStateTerminated = 8,
    IoSessionStateMax
} IO_SESSION_STATE;
#endif // !_KERNEL_MODE

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenSession(
    _Out_ PHANDLE SessionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenSession(
    _Out_ PHANDLE SessionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtNotifyChangeSession(
    _In_ HANDLE SessionHandle,
    _In_ ULONG ChangeSequenceNumber,
    _In_ PLARGE_INTEGER ChangeTimeStamp,
    _In_ IO_SESSION_EVENT Event,
    _In_ IO_SESSION_STATE NewState,
    _In_ IO_SESSION_STATE PreviousState,
    _In_reads_bytes_opt_(PayloadSize) PVOID Payload,
    _In_ ULONG PayloadSize
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwNotifyChangeSession(
    _In_ HANDLE SessionHandle,
    _In_ ULONG ChangeSequenceNumber,
    _In_ PLARGE_INTEGER ChangeTimeStamp,
    _In_ IO_SESSION_EVENT Event,
    _In_ IO_SESSION_STATE NewState,
    _In_ IO_SESSION_STATE PreviousState,
    _In_reads_bytes_opt_(PayloadSize) PVOID Payload,
    _In_ ULONG PayloadSize
);

// I/O Ring

#if (NTDDI_VERSION >= NTDDI_WIN11)
__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateIoRing(
    _Out_ PHANDLE IoRingHandle,
    _In_ ULONG CreateParametersLength,
    _In_ PVOID CreateParameters,
    _In_ ULONG OutputParametersLength,
    _Out_ PVOID OutputParameters
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateIoRing(
    _Out_ PHANDLE IoRingHandle,
    _In_ ULONG CreateParametersLength,
    _In_ PVOID CreateParameters,
    _In_ ULONG OutputParametersLength,
    _Out_ PVOID OutputParameters
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSubmitIoRing(
    _In_ HANDLE IoRingHandle,
    _In_ ULONG Flags,
    _In_opt_ ULONG WaitOperations,
    _In_opt_ PLARGE_INTEGER Timeout
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwSubmitIoRing(
    _In_ HANDLE IoRingHandle,
    _In_ ULONG Flags,
    _In_opt_ ULONG WaitOperations,
    _In_opt_ PLARGE_INTEGER Timeout
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryIoRingCapabilities(
    _In_ SIZE_T IoRingCapabilitiesLength,
    _Out_ PVOID IoRingCapabilities
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryIoRingCapabilities(
    _In_ SIZE_T IoRingCapabilitiesLength,
    _Out_ PVOID IoRingCapabilities
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetInformationIoRing(
    _In_ HANDLE IoRingHandle,
    _In_ ULONG IoRingInformationClass,
    _In_ ULONG IoRingInformationLength,
    _In_ PVOID IoRingInformation
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationIoRing(
    _In_ HANDLE IoRingHandle,
    _In_ ULONG IoRingInformationClass,
    _In_ ULONG IoRingInformationLength,
    _In_ PVOID IoRingInformation
);
#endif // (NTDDI_VERSION >= NTDDI_WIN11)

//
// Other types
//

#ifndef _KERNEL_MODE
//
// Define the I/O bus interface types.
//
typedef enum _INTERFACE_TYPE
{
    InterfaceTypeUndefined = -1,
    Internal = 0,
    Isa = 1,
    Eisa = 2,
    MicroChannel = 3,
    TurboChannel = 4,
    PCIBus = 5,
    VMEBus = 6,
    NuBus = 7,
    PCMCIABus = 8,
    CBus = 9,
    MPIBus = 10,
    MPSABus = 11,
    ProcessorInternal = 12,
    InternalPowerBus = 13,
    PNPISABus = 14,
    PNPBus = 15,
    Vmcs = 16,
    ACPIBus = 17,
    MaximumInterfaceType
} INTERFACE_TYPE, * PINTERFACE_TYPE;

//
// Define the DMA transfer widths.
//

typedef enum _DMA_WIDTH
{
    Width8Bits,
    Width16Bits,
    Width32Bits,
    Width64Bits,
    WidthNoWrap,
    MaximumDmaWidth
} DMA_WIDTH, * PDMA_WIDTH;

//
// Define DMA transfer speeds.
//

typedef enum _DMA_SPEED
{
    Compatible,
    TypeA,
    TypeB,
    TypeC,
    TypeF,
    MaximumDmaSpeed
} DMA_SPEED, * PDMA_SPEED;

typedef enum _BUS_DATA_TYPE
{
    ConfigurationSpaceUndefined = -1,
    Cmos,
    EisaConfiguration,
    Pos,
    CbusConfiguration,
    PCIConfiguration,
    VMEConfiguration,
    NuBusConfiguration,
    PCMCIAConfiguration,
    MPIConfiguration,
    MPSAConfiguration,
    PNPISAConfiguration,
    SgiInternalConfiguration,
    MaximumBusDataType
} BUS_DATA_TYPE, * PBUS_DATA_TYPE;
#endif // !_KERNEL_MODE

//
// Control structures
//

#ifndef _KERNEL_MODE
// Reparse structure for FSCTL_SET_REPARSE_POINT, FSCTL_GET_REPARSE_POINT, FSCTL_DELETE_REPARSE_POINT

#define SYMLINK_FLAG_RELATIVE   0x00000001

#if (NTDDI_VERSION >= NTDDI_WIN10_RS4)
#define SYMLINK_DIRECTORY       0x80000000 // If set then this is a directory symlink
#define SYMLINK_FILE            0x40000000 // If set then this is a file symlink
#endif

typedef struct _REPARSE_DATA_BUFFER
{
    ULONG ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;

    _Field_size_bytes_(ReparseDataLength)
        union
    {
        struct
        {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG Flags;
            WCHAR PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct
        {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR PathBuffer[1];
        } MountPointReparseBuffer;
        struct
        {
            ULONG StringCount;
            WCHAR StringList[1];
        } AppExecLinkReparseBuffer;
        struct
        {
            UCHAR DataBuffer[1];
        } GenericReparseBuffer;
    };
} REPARSE_DATA_BUFFER, * PREPARSE_DATA_BUFFER;

#define REPARSE_DATA_BUFFER_HEADER_SIZE UFIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)

#if (NTDDI_VERSION >= NTDDI_WIN10_RS1)

//
//======================= FSCTL_SET_REPARSE_POINT_EX =========================
//
//  This FSCTL can be used to set or replace  reparse tag of a file.
//

typedef struct _REPARSE_DATA_BUFFER_EX {

    ULONG Flags;

    //
    //  This is the existing reparse tag on the file if any,  if the
    //  caller wants to replace the reparse tag too.
    //
    //    - To set the reparse data  along with the reparse tag that
    //      could be different,  pass the current reparse tag of the
    //      file.
    //
    //    - To update the reparse data while having the same reparse
    //      tag,  the caller should give the existing reparse tag in
    //      this ExistingReparseTag field.
    //
    //    - To set the reparse tag along with reparse data on a file
    //      that doesn't have a reparse tag yet, set this to zero.
    //
    //  If the ExistingReparseTag  does not match the reparse tag on
    //  the file,  the FSCTL_SET_REPARSE_POINT_EX  would  fail  with
    //  STATUS_IO_REPARSE_TAG_MISMATCH. NOTE: If a file doesn't have
    //  a reparse tag, ExistingReparseTag should be 0.
    //

    ULONG ExistingReparseTag;

    //
    //  For non-Microsoft reparse tags, this is the existing reparse
    //  guid on the file if any,  if the caller wants to replace the
    //  reparse tag and / or guid along with the data.
    //
    //  If ExistingReparseTag is 0, the file is not expected to have
    //  any reparse tags, so ExistingReparseGuid is ignored. And for
    //  non-Microsoft tags ExistingReparseGuid should match the guid
    //  in the file if ExistingReparseTag is non zero.
    //

    GUID ExistingReparseGuid;

    //
    //  Reserved
    //

    ULONGLONG Reserved;

    //
    //  Reparse data to set
    //

    union {

        REPARSE_DATA_BUFFER ReparseDataBuffer;
        REPARSE_GUID_DATA_BUFFER ReparseGuidDataBuffer;

    } DUMMYUNIONNAME;

} REPARSE_DATA_BUFFER_EX, * PREPARSE_DATA_BUFFER_EX;

//
//  REPARSE_DATA_BUFFER_EX Flags
//
//  REPARSE_DATA_EX_FLAG_GIVEN_TAG_OR_NONE - Forces the FSCTL to set the
//  reparse tag if the file has no tag or the tag on the file is same as
//  the one in  ExistingReparseTag.   NOTE: If the ExistingReparseTag is
//  not a Microsoft tag then the ExistingReparseGuid should match if the
//  file has the ExistingReparseTag.
//

#define REPARSE_DATA_EX_FLAG_GIVEN_TAG_OR_NONE              (0x00000001)


#define REPARSE_GUID_DATA_BUFFER_EX_HEADER_SIZE \
    UFIELD_OFFSET(REPARSE_DATA_BUFFER_EX, ReparseGuidDataBuffer.GenericReparseBuffer)

#define REPARSE_DATA_BUFFER_EX_HEADER_SIZE \
    UFIELD_OFFSET(REPARSE_DATA_BUFFER_EX, ReparseDataBuffer.GenericReparseBuffer)

#endif /* (NTDDI_VERSION >= NTDDI_WIN10_RS1) */

#endif // !_KERNEL_MODE

// Named pipe FS control definitions

#define DEVICE_NAMED_PIPE L"\\Device\\NamedPipe\\"

#ifndef _KERNEL_MODE
#define FSCTL_PIPE_ASSIGN_EVENT             CTL_CODE(FILE_DEVICE_NAMED_PIPE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_DISCONNECT               CTL_CODE(FILE_DEVICE_NAMED_PIPE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_LISTEN                   CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_PEEK                     CTL_CODE(FILE_DEVICE_NAMED_PIPE, 3, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_PIPE_QUERY_EVENT              CTL_CODE(FILE_DEVICE_NAMED_PIPE, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_TRANSCEIVE               CTL_CODE(FILE_DEVICE_NAMED_PIPE, 5, METHOD_NEITHER,  FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_PIPE_WAIT                     CTL_CODE(FILE_DEVICE_NAMED_PIPE, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_IMPERSONATE              CTL_CODE(FILE_DEVICE_NAMED_PIPE, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_SET_CLIENT_PROCESS       CTL_CODE(FILE_DEVICE_NAMED_PIPE, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_QUERY_CLIENT_PROCESS     CTL_CODE(FILE_DEVICE_NAMED_PIPE, 9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_GET_PIPE_ATTRIBUTE       CTL_CODE(FILE_DEVICE_NAMED_PIPE, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_SET_PIPE_ATTRIBUTE       CTL_CODE(FILE_DEVICE_NAMED_PIPE, 11, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_GET_CONNECTION_ATTRIBUTE CTL_CODE(FILE_DEVICE_NAMED_PIPE, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_SET_CONNECTION_ATTRIBUTE CTL_CODE(FILE_DEVICE_NAMED_PIPE, 13, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_GET_HANDLE_ATTRIBUTE     CTL_CODE(FILE_DEVICE_NAMED_PIPE, 14, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_SET_HANDLE_ATTRIBUTE     CTL_CODE(FILE_DEVICE_NAMED_PIPE, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_FLUSH                    CTL_CODE(FILE_DEVICE_NAMED_PIPE, 16, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_PIPE_DISABLE_IMPERSONATE      CTL_CODE(FILE_DEVICE_NAMED_PIPE, 17, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_SILO_ARRIVAL             CTL_CODE(FILE_DEVICE_NAMED_PIPE, 18, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_PIPE_CREATE_SYMLINK           CTL_CODE(FILE_DEVICE_NAMED_PIPE, 19, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define FSCTL_PIPE_DELETE_SYMLINK           CTL_CODE(FILE_DEVICE_NAMED_PIPE, 20, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define FSCTL_PIPE_QUERY_CLIENT_PROCESS_V2  CTL_CODE(FILE_DEVICE_NAMED_PIPE, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FSCTL_PIPE_INTERNAL_READ            CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2045, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_PIPE_INTERNAL_WRITE           CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2046, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_PIPE_INTERNAL_TRANSCEIVE      CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2047, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_PIPE_INTERNAL_READ_OVFLOW     CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2048, METHOD_BUFFERED, FILE_READ_DATA)

// Flags for query event

#define FILE_PIPE_READ_DATA     0x00000000
#define FILE_PIPE_WRITE_SPACE   0x00000001

// Input for FSCTL_PIPE_ASSIGN_EVENT
typedef struct _FILE_PIPE_ASSIGN_EVENT_BUFFER
{
    HANDLE EventHandle;
    ULONG KeyValue;
} FILE_PIPE_ASSIGN_EVENT_BUFFER, * PFILE_PIPE_ASSIGN_EVENT_BUFFER;

// Output for FILE_PIPE_PEEK_BUFFER
typedef struct _FILE_PIPE_PEEK_BUFFER
{
    ULONG NamedPipeState;
    ULONG ReadDataAvailable;
    ULONG NumberOfMessages;
    ULONG MessageLength;
    CHAR Data[1];
} FILE_PIPE_PEEK_BUFFER, * PFILE_PIPE_PEEK_BUFFER;

// Output for FSCTL_PIPE_QUERY_EVENT
typedef struct _FILE_PIPE_EVENT_BUFFER
{
    ULONG NamedPipeState;
    ULONG EntryType;
    ULONG ByteCount;
    ULONG KeyValue;
    ULONG NumberRequests;
} FILE_PIPE_EVENT_BUFFER, * PFILE_PIPE_EVENT_BUFFER;

// Input for FSCTL_PIPE_WAIT
typedef struct _FILE_PIPE_WAIT_FOR_BUFFER
{
    LARGE_INTEGER Timeout;
    ULONG NameLength;
    BOOLEAN TimeoutSpecified;
    WCHAR Name[1];
} FILE_PIPE_WAIT_FOR_BUFFER, * PFILE_PIPE_WAIT_FOR_BUFFER;

// Input for FSCTL_PIPE_SET_CLIENT_PROCESS, Output for FSCTL_PIPE_QUERY_CLIENT_PROCESS
typedef struct _FILE_PIPE_CLIENT_PROCESS_BUFFER
{
#if !defined(BUILD_WOW6432)
    PVOID ClientSession;
    PVOID ClientProcess;
#else
    ULONGLONG ClientSession;
    ULONGLONG ClientProcess;
#endif
} FILE_PIPE_CLIENT_PROCESS_BUFFER, * PFILE_PIPE_CLIENT_PROCESS_BUFFER;

// Control structure for FSCTL_PIPE_QUERY_CLIENT_PROCESS_V2

typedef struct _FILE_PIPE_CLIENT_PROCESS_BUFFER_V2
{
    ULONGLONG ClientSession;
#if !defined(BUILD_WOW6432)
    PVOID ClientProcess;
#else
    ULONGLONG ClientProcess;
#endif
} FILE_PIPE_CLIENT_PROCESS_BUFFER_V2, * PFILE_PIPE_CLIENT_PROCESS_BUFFER_V2;

#define FILE_PIPE_COMPUTER_NAME_LENGTH 15

// Input for FSCTL_PIPE_SET_CLIENT_PROCESS, Output for FSCTL_PIPE_QUERY_CLIENT_PROCESS
typedef struct _FILE_PIPE_CLIENT_PROCESS_BUFFER_EX
{
#if !defined(BUILD_WOW6432)
    PVOID ClientSession;
    PVOID ClientProcess;
#else
    ULONGLONG ClientSession;
    ULONGLONG ClientProcess;
#endif
    USHORT ClientComputerNameLength; // in bytes
    WCHAR ClientComputerBuffer[FILE_PIPE_COMPUTER_NAME_LENGTH + 1]; // null-terminated
} FILE_PIPE_CLIENT_PROCESS_BUFFER_EX, * PFILE_PIPE_CLIENT_PROCESS_BUFFER_EX;

// Control structure for FSCTL_PIPE_SILO_ARRIVAL

typedef struct _FILE_PIPE_SILO_ARRIVAL_INPUT
{
    HANDLE JobHandle;
} FILE_PIPE_SILO_ARRIVAL_INPUT, * PFILE_PIPE_SILO_ARRIVAL_INPUT;

//
// Flags for create symlink
//

//
// A global symlink will cause resolution of the symlink's target to occur in
// the host silo (i.e. not in any current silo).  For example, if there is a
// symlink at \Device\Silos\37\Device\NamedPipe\symlink then the target will be
// resolved as \Device\NamedPipe\target instead of \Device\Silos\37\Device\NamedPipe\target
//
#define FILE_PIPE_SYMLINK_FLAG_GLOBAL   0x1

//
// A relative symlink will cause resolution of the symlink's target to occur relative
// to the root of the named pipe file system.  For example, if there is a symlink at
// \Device\NamedPipe\symlink that has a target called "target", then the target will
// be resolved as \Device\NamedPipe\target
//
#define FILE_PIPE_SYMLINK_FLAG_RELATIVE 0x2

#define FILE_PIPE_SYMLINK_VALID_FLAGS \
    (FILE_PIPE_SYMLINK_FLAG_GLOBAL | FILE_PIPE_SYMLINK_FLAG_RELATIVE)

// Control structure for FSCTL_PIPE_CREATE_SYMLINK

typedef struct _FILE_PIPE_CREATE_SYMLINK_INPUT
{
    USHORT NameOffset;
    USHORT NameLength;
    USHORT SubstituteNameOffset;
    USHORT SubstituteNameLength;
    ULONG Flags;
} FILE_PIPE_CREATE_SYMLINK_INPUT, * PFILE_PIPE_CREATE_SYMLINK_INPUT;

// Control structure for FSCTL_PIPE_DELETE_SYMLINK

typedef struct _FILE_PIPE_DELETE_SYMLINK_INPUT
{
    USHORT NameOffset;
    USHORT NameLength;
} FILE_PIPE_DELETE_SYMLINK_INPUT, * PFILE_PIPE_DELETE_SYMLINK_INPUT;
#endif // !_KERNEL_MODE

// Mailslot FS control definitions

#define MAILSLOT_CLASS_FIRSTCLASS   1
#define MAILSLOT_CLASS_SECONDCLASS  2

#define FSCTL_MAILSLOT_PEEK         CTL_CODE(FILE_DEVICE_MAILSLOT, 0, METHOD_NEITHER, FILE_READ_DATA)

// Output for FSCTL_MAILSLOT_PEEK
typedef struct _FILE_MAILSLOT_PEEK_BUFFER
{
    ULONG ReadDataAvailable;
    ULONG NumberOfMessages;
    ULONG MessageLength;
} FILE_MAILSLOT_PEEK_BUFFER, * PFILE_MAILSLOT_PEEK_BUFFER;

// Mount manager FS control definitions

#define MOUNTMGR_DEVICE_NAME L"\\Device\\MountPointManager"
#define MOUNTMGRCONTROLTYPE 0x0000006D // 'm'
#define MOUNTDEVCONTROLTYPE 0x0000004D // 'M'

#define IOCTL_MOUNTMGR_CREATE_POINT                 CTL_CODE(MOUNTMGRCONTROLTYPE, 0, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_DELETE_POINTS                CTL_CODE(MOUNTMGRCONTROLTYPE, 1, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_QUERY_POINTS                 CTL_CODE(MOUNTMGRCONTROLTYPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MOUNTMGR_DELETE_POINTS_DBONLY         CTL_CODE(MOUNTMGRCONTROLTYPE, 3, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_NEXT_DRIVE_LETTER            CTL_CODE(MOUNTMGRCONTROLTYPE, 4, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_AUTO_DL_ASSIGNMENTS          CTL_CODE(MOUNTMGRCONTROLTYPE, 5, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_VOLUME_MOUNT_POINT_CREATED   CTL_CODE(MOUNTMGRCONTROLTYPE, 6, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_VOLUME_MOUNT_POINT_DELETED   CTL_CODE(MOUNTMGRCONTROLTYPE, 7, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_CHANGE_NOTIFY                CTL_CODE(MOUNTMGRCONTROLTYPE, 8, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_MOUNTMGR_KEEP_LINKS_WHEN_OFFLINE      CTL_CODE(MOUNTMGRCONTROLTYPE, 9, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_CHECK_UNPROCESSED_VOLUMES    CTL_CODE(MOUNTMGRCONTROLTYPE, 10, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_MOUNTMGR_VOLUME_ARRIVAL_NOTIFICATION  CTL_CODE(MOUNTMGRCONTROLTYPE, 11, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_MOUNTMGR_QUERY_DOS_VOLUME_PATH        CTL_CODE(MOUNTMGRCONTROLTYPE, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MOUNTMGR_QUERY_DOS_VOLUME_PATHS       CTL_CODE(MOUNTMGRCONTROLTYPE, 13, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MOUNTMGR_SCRUB_REGISTRY               CTL_CODE(MOUNTMGRCONTROLTYPE, 14, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_QUERY_AUTO_MOUNT             CTL_CODE(MOUNTMGRCONTROLTYPE, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MOUNTMGR_SET_AUTO_MOUNT               CTL_CODE(MOUNTMGRCONTROLTYPE, 16, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_BOOT_DL_ASSIGNMENT           CTL_CODE(MOUNTMGRCONTROLTYPE, 17, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS) // since WIN7
#define IOCTL_MOUNTMGR_TRACELOG_CACHE               CTL_CODE(MOUNTMGRCONTROLTYPE, 18, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_MOUNTMGR_PREPARE_VOLUME_DELETE        CTL_CODE(MOUNTMGRCONTROLTYPE, 19, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MOUNTMGR_CANCEL_VOLUME_DELETE         CTL_CODE(MOUNTMGRCONTROLTYPE, 20, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS) // since WIN8
#define IOCTL_MOUNTMGR_SILO_ARRIVAL                 CTL_CODE(MOUNTMGRCONTROLTYPE, 21, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS) // since RS1

#define IOCTL_MOUNTDEV_QUERY_DEVICE_NAME            CTL_CODE(MOUNTDEVCONTROLTYPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Input structure for IOCTL_MOUNTMGR_CREATE_POINT.
typedef struct _MOUNTMGR_CREATE_POINT_INPUT
{
    USHORT SymbolicLinkNameOffset;
    USHORT SymbolicLinkNameLength;
    USHORT DeviceNameOffset;
    USHORT DeviceNameLength;
} MOUNTMGR_CREATE_POINT_INPUT, * PMOUNTMGR_CREATE_POINT_INPUT;

// Input structure for IOCTL_MOUNTMGR_DELETE_POINTS, IOCTL_MOUNTMGR_QUERY_POINTS, and IOCTL_MOUNTMGR_DELETE_POINTS_DBONLY.
typedef struct _MOUNTMGR_MOUNT_POINT
{
    ULONG SymbolicLinkNameOffset;
    USHORT SymbolicLinkNameLength;
    USHORT Reserved1;
    ULONG UniqueIdOffset;
    USHORT UniqueIdLength;
    USHORT Reserved2;
    ULONG DeviceNameOffset;
    USHORT DeviceNameLength;
    USHORT Reserved3;
} MOUNTMGR_MOUNT_POINT, * PMOUNTMGR_MOUNT_POINT;

// Output structure for IOCTL_MOUNTMGR_DELETE_POINTS, IOCTL_MOUNTMGR_QUERY_POINTS, and IOCTL_MOUNTMGR_DELETE_POINTS_DBONLY.
typedef struct _MOUNTMGR_MOUNT_POINTS
{
    ULONG Size;
    ULONG NumberOfMountPoints;
    MOUNTMGR_MOUNT_POINT MountPoints[1];
} MOUNTMGR_MOUNT_POINTS, * PMOUNTMGR_MOUNT_POINTS;

// Input structure for IOCTL_MOUNTMGR_NEXT_DRIVE_LETTER.
typedef struct _MOUNTMGR_DRIVE_LETTER_TARGET
{
    USHORT DeviceNameLength;
    WCHAR DeviceName[1];
} MOUNTMGR_DRIVE_LETTER_TARGET, * PMOUNTMGR_DRIVE_LETTER_TARGET;

// Output structure for IOCTL_MOUNTMGR_NEXT_DRIVE_LETTER.
typedef struct _MOUNTMGR_DRIVE_LETTER_INFORMATION
{
    BOOLEAN DriveLetterWasAssigned;
    UCHAR CurrentDriveLetter;
} MOUNTMGR_DRIVE_LETTER_INFORMATION, * PMOUNTMGR_DRIVE_LETTER_INFORMATION;

// Input structure for IOCTL_MOUNTMGR_VOLUME_MOUNT_POINT_CREATED and
// IOCTL_MOUNTMGR_VOLUME_MOUNT_POINT_DELETED.
typedef struct _MOUNTMGR_VOLUME_MOUNT_POINT
{
    USHORT SourceVolumeNameOffset;
    USHORT SourceVolumeNameLength;
    USHORT TargetVolumeNameOffset;
    USHORT TargetVolumeNameLength;
} MOUNTMGR_VOLUME_MOUNT_POINT, * PMOUNTMGR_VOLUME_MOUNT_POINT;

// Input structure for IOCTL_MOUNTMGR_CHANGE_NOTIFY.
// Output structure for IOCTL_MOUNTMGR_CHANGE_NOTIFY.
typedef struct _MOUNTMGR_CHANGE_NOTIFY_INFO
{
    ULONG EpicNumber;
} MOUNTMGR_CHANGE_NOTIFY_INFO, * PMOUNTMGR_CHANGE_NOTIFY_INFO;

// Input structure for IOCTL_MOUNTMGR_KEEP_LINKS_WHEN_OFFLINE,
// IOCTL_MOUNTMGR_VOLUME_ARRIVAL_NOTIFICATION,
// IOCTL_MOUNTMGR_QUERY_DOS_VOLUME_PATH, and
// IOCTL_MOUNTMGR_QUERY_DOS_VOLUME_PATHS.
// IOCTL_MOUNTMGR_PREPARE_VOLUME_DELETE
// IOCTL_MOUNTMGR_CANCEL_VOLUME_DELETE
typedef struct _MOUNTMGR_TARGET_NAME
{
    USHORT DeviceNameLength;
    WCHAR DeviceName[1];
} MOUNTMGR_TARGET_NAME, * PMOUNTMGR_TARGET_NAME;

// Input / Output structure for querying / setting the auto-mount setting
typedef enum _MOUNTMGR_AUTO_MOUNT_STATE
{
    Disabled = 0,
    Enabled
} MOUNTMGR_AUTO_MOUNT_STATE;

// IOCTL_MOUNTMGR_QUERY_AUTO_MOUNT
typedef struct _MOUNTMGR_QUERY_AUTO_MOUNT
{
    MOUNTMGR_AUTO_MOUNT_STATE CurrentState;
} MOUNTMGR_QUERY_AUTO_MOUNT, * PMOUNTMGR_QUERY_AUTO_MOUNT;

// IOCTL_MOUNTMGR_SET_AUTO_MOUNT
typedef struct _MOUNTMGR_SET_AUTO_MOUNT
{
    MOUNTMGR_AUTO_MOUNT_STATE NewState;
} MOUNTMGR_SET_AUTO_MOUNT, * PMOUNTMGR_SET_AUTO_MOUNT;

// Input structure for IOCTL_MOUNTMGR_SILO_ARRIVAL.
typedef struct _MOUNTMGR_SILO_ARRIVAL_INPUT
{
    HANDLE JobHandle;
} MOUNTMGR_SILO_ARRIVAL_INPUT, * PMOUNTMGR_SILO_ARRIVAL_INPUT;

// Macro that defines what a "drive letter" mount point is.  This macro can
// be used to scan the result from QUERY_POINTS to discover which mount points
// are find "drive letter" mount points.
#define MOUNTMGR_IS_DRIVE_LETTER(s) ( \
    (s)->Length == 28 && \
    (s)->Buffer[0] == '\\' && \
    (s)->Buffer[1] == 'D' && \
    (s)->Buffer[2] == 'o' && \
    (s)->Buffer[3] == 's' && \
    (s)->Buffer[4] == 'D' && \
    (s)->Buffer[5] == 'e' && \
    (s)->Buffer[6] == 'v' && \
    (s)->Buffer[7] == 'i' && \
    (s)->Buffer[8] == 'c' && \
    (s)->Buffer[9] == 'e' && \
    (s)->Buffer[10] == 's' && \
    (s)->Buffer[11] == '\\' && \
    (s)->Buffer[12] >= 'A' && \
    (s)->Buffer[12] <= 'Z' && \
    (s)->Buffer[13] == ':')

// Macro that defines what a "volume name" mount point is.  This macro can
// be used to scan the result from QUERY_POINTS to discover which mount points
// are "volume name" mount points.
#define MOUNTMGR_IS_VOLUME_NAME(s) ( \
     ((s)->Length == 96 || ((s)->Length == 98 && (s)->Buffer[48] == '\\')) && \
     (s)->Buffer[0] == '\\' && \
     ((s)->Buffer[1] == '?' || (s)->Buffer[1] == '\\') && \
     (s)->Buffer[2] == '?' && \
     (s)->Buffer[3] == '\\' && \
     (s)->Buffer[4] == 'V' && \
     (s)->Buffer[5] == 'o' && \
     (s)->Buffer[6] == 'l' && \
     (s)->Buffer[7] == 'u' && \
     (s)->Buffer[8] == 'm' && \
     (s)->Buffer[9] == 'e' && \
     (s)->Buffer[10] == '{' && \
     (s)->Buffer[19] == '-' && \
     (s)->Buffer[24] == '-' && \
     (s)->Buffer[29] == '-' && \
     (s)->Buffer[34] == '-' && \
     (s)->Buffer[47] == '}')

// Output structure for IOCTL_MOUNTDEV_QUERY_DEVICE_NAME.
typedef struct _MOUNTDEV_NAME
{
    USHORT NameLength;
    WCHAR Name[1];
} MOUNTDEV_NAME, * PMOUNTDEV_NAME;

// Output structure for IOCTL_MOUNTMGR_QUERY_DOS_VOLUME_PATH and IOCTL_MOUNTMGR_QUERY_DOS_VOLUME_PATHS.
typedef struct _MOUNTMGR_VOLUME_PATHS
{
    ULONG MultiSzLength;
    WCHAR MultiSz[1];
} MOUNTMGR_VOLUME_PATHS, * PMOUNTMGR_VOLUME_PATHS;

#define MOUNTMGR_IS_DOS_VOLUME_NAME(s) ( \
     MOUNTMGR_IS_VOLUME_NAME(s) && \
     (s)->Length == 96 && \
     (s)->Buffer[1] == '\\')

#define MOUNTMGR_IS_DOS_VOLUME_NAME_WB(s) ( \
     MOUNTMGR_IS_VOLUME_NAME(s) && \
     (s)->Length == 98 && \
     (s)->Buffer[1] == '\\')

#define MOUNTMGR_IS_NT_VOLUME_NAME(s) ( \
     MOUNTMGR_IS_VOLUME_NAME(s) && \
     (s)->Length == 96 && \
     (s)->Buffer[1] == '?')

#define MOUNTMGR_IS_NT_VOLUME_NAME_WB(s) ( \
     MOUNTMGR_IS_VOLUME_NAME(s) && \
     (s)->Length == 98 && \
     (s)->Buffer[1] == '?')

// KSecDD FS control definitions

#define KSEC_DEVICE_NAME L"\\Device\\KSecDD"

#define IOCTL_KSEC_CONNECT_LSA                      CTL_CODE(FILE_DEVICE_KSEC,  0, METHOD_BUFFERED,     FILE_WRITE_ACCESS )
#define IOCTL_KSEC_RNG                              CTL_CODE(FILE_DEVICE_KSEC,  1, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_RNG_REKEY                        CTL_CODE(FILE_DEVICE_KSEC,  2, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_ENCRYPT_MEMORY                   CTL_CODE(FILE_DEVICE_KSEC,  3, METHOD_OUT_DIRECT,   FILE_ANY_ACCESS )
#define IOCTL_KSEC_DECRYPT_MEMORY                   CTL_CODE(FILE_DEVICE_KSEC,  4, METHOD_OUT_DIRECT,   FILE_ANY_ACCESS )
#define IOCTL_KSEC_ENCRYPT_MEMORY_CROSS_PROC        CTL_CODE(FILE_DEVICE_KSEC,  5, METHOD_OUT_DIRECT,   FILE_ANY_ACCESS )
#define IOCTL_KSEC_DECRYPT_MEMORY_CROSS_PROC        CTL_CODE(FILE_DEVICE_KSEC,  6, METHOD_OUT_DIRECT,   FILE_ANY_ACCESS )
#define IOCTL_KSEC_ENCRYPT_MEMORY_SAME_LOGON        CTL_CODE(FILE_DEVICE_KSEC,  7, METHOD_OUT_DIRECT,   FILE_ANY_ACCESS )
#define IOCTL_KSEC_DECRYPT_MEMORY_SAME_LOGON        CTL_CODE(FILE_DEVICE_KSEC,  8, METHOD_OUT_DIRECT,   FILE_ANY_ACCESS )
#define IOCTL_KSEC_FIPS_GET_FUNCTION_TABLE          CTL_CODE(FILE_DEVICE_KSEC,  9, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_ALLOC_POOL                       CTL_CODE(FILE_DEVICE_KSEC, 10, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_FREE_POOL                        CTL_CODE(FILE_DEVICE_KSEC, 11, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_COPY_POOL                        CTL_CODE(FILE_DEVICE_KSEC, 12, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_DUPLICATE_HANDLE                 CTL_CODE(FILE_DEVICE_KSEC, 13, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_REGISTER_EXTENSION               CTL_CODE(FILE_DEVICE_KSEC, 14, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_CLIENT_CALLBACK                  CTL_CODE(FILE_DEVICE_KSEC, 15, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_GET_BCRYPT_EXTENSION             CTL_CODE(FILE_DEVICE_KSEC, 16, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_GET_SSL_EXTENSION                CTL_CODE(FILE_DEVICE_KSEC, 17, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_GET_DEVICECONTROL_EXTENSION      CTL_CODE(FILE_DEVICE_KSEC, 18, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_ALLOC_VM                         CTL_CODE(FILE_DEVICE_KSEC, 19, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_FREE_VM                          CTL_CODE(FILE_DEVICE_KSEC, 20, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_COPY_VM                          CTL_CODE(FILE_DEVICE_KSEC, 21, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_CLIENT_FREE_VM                   CTL_CODE(FILE_DEVICE_KSEC, 22, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_INSERT_PROTECTED_PROCESS_ADDRESS CTL_CODE(FILE_DEVICE_KSEC, 23, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_REMOVE_PROTECTED_PROCESS_ADDRESS CTL_CODE(FILE_DEVICE_KSEC, 24, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_GET_BCRYPT_EXTENSION2            CTL_CODE(FILE_DEVICE_KSEC, 25, METHOD_BUFFERED,     FILE_ANY_ACCESS )
#define IOCTL_KSEC_IPC_GET_QUEUED_FUNCTION_CALLS    CTL_CODE(FILE_DEVICE_KSEC, 26, METHOD_OUT_DIRECT,   FILE_ANY_ACCESS)
#define IOCTL_KSEC_IPC_SET_FUNCTION_RETURN          CTL_CODE(FILE_DEVICE_KSEC, 27, METHOD_NEITHER,      FILE_ANY_ACCESS)

//
// Only Kernel
//

#ifdef _KERNEL_MODE

// Driver

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
IoCreateDriver(
    _In_opt_ PUNICODE_STRING DriverName,
    _In_ PDRIVER_INITIALIZE InitializationFunction
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
VOID
NTAPI
IoDeleteDriver(
    _In_ PDRIVER_OBJECT DriverObject
);

#endif // _KERNEL_MODE

VEIL_END()

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
