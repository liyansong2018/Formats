// copyright (c) 2017-2021 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef XMACH_H
#define XMACH_H

#include "xbinary.h"
#include "xmach_def.h"

class XMACH : public XBinary
{
    Q_OBJECT

public:
    struct COMMAND_RECORD
    {
        qint64 nStructOffset;
        quint32 nType;
        qint64 nSize;
    };

    struct LIBRARY_RECORD
    {
        qint64 nStructOffset;
        qint64 nStructSize;
        QString sName;
        QString sFullName;
        quint32 name;
        quint32 timestamp;
        quint32 current_version;
        quint32 compatibility_version;
        qint64 nMaxStringSize;
    };

    struct SEGMENT_RECORD
    {
        qint64 nStructOffset;
        union
        {
            XMACH_DEF::segment_command segment32;
            XMACH_DEF::segment_command_64 segment64;
        }s;
    };

    struct SECTION_RECORD
    {
        qint64 nStructOffset;
        union
        {
            XMACH_DEF::section section32;
            XMACH_DEF::section_64 section64;
        }s;
    };

    struct NLIST_RECORD
    {
        qint64 nStructOffset;
        bool bIs64;
        union
        {
            XMACH_DEF::nlist nlist32;
            XMACH_DEF::nlist_64 nlist64;
        }s;
    };

    struct DICE_RECORD
    {
        qint64 nStructOffset;
        XMACH_DEF::data_in_code_entry dice;
    };

    struct FUNCTION_RECORD
    {
        qint64 nDataOffset;
        qint64 nFunctionOffset;
        qint64 nFunctionAddress;
    };

    enum TYPE
    {
        TYPE_UNKNOWN=0,
        TYPE_OBJECT,
        TYPE_EXECUTE,
        TYPE_FVMLIB,
        TYPE_CORE,
        TYPE_PRELOAD,
        TYPE_DYLIB,
        TYPE_DYLINKER,
        TYPE_BUNDLE,
        TYPE_DYLIB_STUB,
        TYPE_DSYM,
        TYPE_KEXT_BUNDLE
    };

    XMACH(QIODevice *pDevice=nullptr,bool bIsImage=false,qint64 nImageAddress=-1);
    ~XMACH();

    bool isValid();
    static bool isValid(QIODevice *pDevice,bool bIsImage=false,qint64 nImageAddress=-1);
    static MODE getMode(QIODevice *pDevice,bool bIsImage=false,qint64 nImageAddress=-1);
    bool isBigEndian();

    qint64 getHeaderOffset();
    qint64 getHeader32Size();
    qint64 getHeader64Size();

    quint32 getHeader_magic();
    qint32 getHeader_cputype();
    qint32 getHeader_cpusubtype();
    quint32 getHeader_filetype();
    quint32 getHeader_ncmds();
    quint32 getHeader_sizeofcmds();
    quint32 getHeader_flags();
    quint32 getHeader_reserved();

    void setHeader_magic(quint32 nValue);
    void setHeader_cputype(qint32 nValue);
    void setHeader_cpusubtype(qint32 nValue);
    void setHeader_filetype(quint32 nValue);
    void setHeader_ncmds(quint32 nValue);
    void setHeader_sizeofcmds(quint32 nValue);
    void setHeader_flags(quint32 nValue);
    void setHeader_reserved(quint32 nValue);

    qint64 getHeaderSize();

    static QMap<quint64,QString> getHeaderMagics();
    static QMap<quint64,QString> getHeaderMagicsS();
    static QMap<quint64,QString> getHeaderCpuTypes();
    static QMap<quint64,QString> getHeaderCpuTypesS();
    static QMap<quint64,QString> getHeaderCpuSubTypes(quint32 nCpuType);
    static QMap<quint64,QString> getHeaderCpuSubTypesS(quint32 nCpuType);
    static QMap<quint64,QString> getHeaderFileTypes();
    static QMap<quint64,QString> getHeaderFileTypesS();
    static QMap<quint64,QString> getHeaderFlags();
    static QMap<quint64,QString> getHeaderFlagsS();
    static QMap<quint64,QString> getLoadCommandTypes();
    static QMap<quint64,QString> getLoadCommandTypesS();
    static QMap<quint64,QString> getVMProtections();
    static QMap<quint64,QString> getVMProtectionsS();
    static QMap<quint64,QString> getSectionFlagsTypes();
    static QMap<quint64,QString> getSectionFlagsTypesS();
    static QMap<quint64,QString> getSectionAttributesUsr();
    static QMap<quint64,QString> getSectionAttributesUsrS();
    static QMap<quint64,QString> getSectionAttributesSys();
    static QMap<quint64,QString> getSectionAttributesSysS();
    static QMap<quint64,QString> getDICEKinds();
    static QMap<quint64,QString> getDICEKindsS();

    COMMAND_RECORD _readLoadCommand(qint64 nOffset,bool bIsBigEndian);

    void _setCommand_cmd(qint64 nOffset,quint32 nValue);
    void _setCommand_cmdsize(qint64 nOffset,quint32 nValue);

    QList<COMMAND_RECORD> getCommandRecords(quint32 nCommandID=0);
    static QList<COMMAND_RECORD> getCommandRecords(quint32 nCommandID,QList<COMMAND_RECORD> *pListCommandRecords);

    bool isCommandPresent(quint32 nCommandID,int nIndex=0);
    bool isCommandPresent(quint32 nCommandID,int nIndex,QList<COMMAND_RECORD> *pListCommandRecords);
    bool isCommandPresent(quint32 nCommandID,QList<COMMAND_RECORD> *pListCommandRecords);

    QByteArray getCommandData(quint32 nCommandID,int nIndex=0);
    bool setCommandData(quint32 nCommandID,QByteArray baData,int nIndex=0);
    QByteArray getCommandData(quint32 nCommandID,int nIndex,QList<COMMAND_RECORD> *pListCommandRecords);
    bool setCommandData(quint32 nCommandID,QByteArray baData,int nIndex,QList<COMMAND_RECORD> *pListCommandRecords);

    qint64 getCommandRecordOffset(quint32 nCommandID,int nIndex);
    qint64 getCommandRecordOffset(quint32 nCommandID,int nIndex,QList<COMMAND_RECORD> *pListCommandRecords);

    qint64 getCommandHeaderSize();

    qint64 getAddressOfEntryPoint();

    virtual _MEMORY_MAP getMemoryMap();
    virtual qint64 getEntryPointOffset(_MEMORY_MAP *pMemoryMap);

    QList<LIBRARY_RECORD> getLibraryRecords(int nType=XMACH_DEF::S_LC_LOAD_DYLIB);
    QList<LIBRARY_RECORD> getLibraryRecords(QList<COMMAND_RECORD> *pListCommandRecords,int nType=XMACH_DEF::S_LC_LOAD_DYLIB);
    static LIBRARY_RECORD getLibraryRecordByName(QString sName,QList<LIBRARY_RECORD> *pListLibraryRecords);
    bool isLibraryRecordNamePresent(QString sName);
    static bool isLibraryRecordNamePresent(QString sName,QList<LIBRARY_RECORD> *pListLibraryRecords);

    LIBRARY_RECORD _readLibraryRecord(qint64 nOffset,bool bIsBigEndian);

    void _setLibraryRecord_timestamp(qint64 nOffset,quint32 nValue);
    void _setLibraryRecord_current_version(qint64 nOffset,quint32 nValue);
    void _setLibraryRecord_compatibility_version(qint64 nOffset,quint32 nValue);
    void _setLibraryRecord_name(qint64 nOffset,QString sValue);

    QList<SEGMENT_RECORD> getSegmentRecords();
    QList<SEGMENT_RECORD> getSegmentRecords(QList<COMMAND_RECORD> *pListCommandRecords);

    XMACH_DEF::segment_command _read_segment_command(qint64 nOffset,bool bIsBigEndian);
    XMACH_DEF::segment_command_64 _read_segment_command_64(qint64 nOffset,bool bIsBigEndian);

    void _setSegment32_segname(qint64 nOffset,QString sValue);
    void _setSegment32_vmaddr(qint64 nOffset,quint32 nValue);
    void _setSegment32_vmsize(qint64 nOffset,quint32 nValue);
    void _setSegment32_fileoff(qint64 nOffset,quint32 nValue);
    void _setSegment32_filesize(qint64 nOffset,quint32 nValue);
    void _setSegment32_maxprot(qint64 nOffset,qint32 nValue);
    void _setSegment32_initprot(qint64 nOffset,qint32 nValue);
    void _setSegment32_nsects(qint64 nOffset,quint32 nValue);
    void _setSegment32_flags(qint64 nOffset,quint32 nValue);

    void _setSegment64_segname(qint64 nOffset,QString sValue);
    void _setSegment64_vmaddr(qint64 nOffset,quint64 nValue);
    void _setSegment64_vmsize(qint64 nOffset,quint64 nValue);
    void _setSegment64_fileoff(qint64 nOffset,quint64 nValue);
    void _setSegment64_filesize(qint64 nOffset,quint64 nValue);
    void _setSegment64_maxprot(qint64 nOffset,qint32 nValue);
    void _setSegment64_initprot(qint64 nOffset,qint32 nValue);
    void _setSegment64_nsects(qint64 nOffset,quint32 nValue);
    void _setSegment64_flags(qint64 nOffset,quint32 nValue);

    QList<SECTION_RECORD> getSectionRecords();
    QList<SECTION_RECORD> getSectionRecords(QList<COMMAND_RECORD> *pListCommandRecords);

    XMACH_DEF::section _read_section(qint64 nOffset,bool bIsBigEndian);
    XMACH_DEF::section_64 _read_section_64(qint64 nOffset,bool bIsBigEndian);

    void _setSection32_sectname(qint64 nOffset,QString sValue);
    void _setSection32_segname(qint64 nOffset,QString sValue);
    void _setSection32_addr(qint64 nOffset,quint32 nValue);
    void _setSection32_size(qint64 nOffset,quint32 nValue);
    void _setSection32_offset(qint64 nOffset,quint32 nValue);
    void _setSection32_align(qint64 nOffset,quint32 nValue);
    void _setSection32_reloff(qint64 nOffset,quint32 nValue);
    void _setSection32_nreloc(qint64 nOffset,quint32 nValue);
    void _setSection32_flags(qint64 nOffset,quint32 nValue);
    void _setSection32_reserved1(qint64 nOffset,quint32 nValue);
    void _setSection32_reserved2(qint64 nOffset,quint32 nValue);

    void _setSection64_sectname(qint64 nOffset,QString sValue);
    void _setSection64_segname(qint64 nOffset,QString sValue);
    void _setSection64_addr(qint64 nOffset,quint64 nValue);
    void _setSection64_size(qint64 nOffset,quint64 nValue);
    void _setSection64_offset(qint64 nOffset,quint32 nValue);
    void _setSection64_align(qint64 nOffset,quint32 nValue);
    void _setSection64_reloff(qint64 nOffset,quint32 nValue);
    void _setSection64_nreloc(qint64 nOffset,quint32 nValue);
    void _setSection64_flags(qint64 nOffset,quint32 nValue);
    void _setSection64_reserved1(qint64 nOffset,quint32 nValue);
    void _setSection64_reserved2(qint64 nOffset,quint32 nValue);
    void _setSection64_reserved3(qint64 nOffset,quint32 nValue);

    qint64 getSegmentHeaderSize();

    quint32 getNumberOfSections();
    quint32 getNumberOfSections(QList<COMMAND_RECORD> *pListCommandRecords);

    bool isSectionNamePresent(QString sName);
    static bool isSectionNamePresent(QString sName,QList<SECTION_RECORD> *pListSectionRecords);

    static qint32 getSectionNumber(QString sName,QList<SECTION_RECORD> *pListSectionRecords);
    qint32 getSectionNumber(QString sName);

    qint64 getSectionHeaderSize();

    quint32 getLibraryCurrentVersion(QString sName,QList<LIBRARY_RECORD> *pListLibraryRecords);

    XMACH_DEF::dyld_info_command get_dyld_info_command();

    qint64 get_dyld_info_command_size();

    void _set_dyld_info_command_rebase_off(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_rebase_size(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_bind_off(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_bind_size(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_weak_bind_off(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_weak_bind_size(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_lazy_bind_off(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_lazy_bind_size(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_export_off(qint64 nOffset,quint32 nValue);
    void _set_dyld_info_command_export_size(qint64 nOffset,quint32 nValue);

    QString getUUID();
    void setUUID(QString sValue);

    QString getLoadDylinker();
    void setLoadDylinker(QString sValue);

    QString getRPath();
    void setRPath(QString sValue);

    XMACH_DEF::symtab_command get_symtab_command();

    qint64 get_symtab_command_size();

    void _set_symtab_command_symoff(qint64 nOffset,quint32 nValue);
    void _set_symtab_command_nsyms(qint64 nOffset,quint32 nValue);
    void _set_symtab_command_stroff(qint64 nOffset,quint32 nValue);
    void _set_symtab_command_strsize(qint64 nOffset,quint32 nValue);

    XMACH_DEF::dysymtab_command get_dysymtab_command();

    qint64 get_dysymtab_command_size();

    void _set_dysymtab_command_ilocalsym(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nlocalsym(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_iextdefsym(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nextdefsym(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_iundefsym(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nundefsym(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_tocoff(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_ntoc(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_modtaboff(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nmodtab(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_extrefsymoff(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nextrefsyms(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_indirectsymoff(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nindirectsyms(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_extreloff(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nextrel(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_locreloff(qint64 nOffset,quint32 nValue);
    void _set_dysymtab_command_nlocrel(qint64 nOffset,quint32 nValue);

    XMACH_DEF::version_min_command get_version_min_command();

    qint64 get_version_min_command_size();

    void _set_version_min_command_version(qint64 nOffset,quint32 nValue);
    void _set_version_min_command_sdk(qint64 nOffset,quint32 nValue);

    quint64 getSourceVersion();
    void setSourceVersion(quint64 nValue);

    qint64 get_source_version_command_size();

    void _set_encryption_info_command_64_cryptoff(qint64 nOffset,quint32 nValue);
    void _set_encryption_info_command_64_cryptsize(qint64 nOffset,quint32 nValue);
    void _set_encryption_info_command_64_cryptid(qint64 nOffset,quint32 nValue);
    void _set_encryption_info_command_64_pad(qint64 nOffset,quint32 nValue);

    qint64 get_encryption_info_command_size();
    qint64 get_encryption_info_command_64_size();

    void _set_linkedit_data_command_dataoff(qint64 nOffset,quint32 nValue);
    void _set_linkedit_data_command_datasize(qint64 nOffset,quint32 nValue);

    qint64 get_linkedit_data_command_size();

    void _set_entry_point_command_entryoff(qint64 nOffset,quint64 nValue);
    void _set_entry_point_command_stacksize(qint64 nOffset,quint64 nValue);

    qint64 get_entry_point_command_size();

    void _set_unix_thread_command_flavor(qint64 nOffset,quint32 nValue);
    void _set_unix_thread_command_count(qint64 nOffset,quint32 nValue);

    qint64 get_unix_thread_command_size();

    void _set_x86_thread_state32_t_eax(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_ebx(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_ecx(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_edx(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_edi(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_esi(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_ebp(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_esp(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_ss(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_eflags(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_eip(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_cs(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_ds(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_es(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_fs(qint64 nOffset,quint32 nValue);
    void _set_x86_thread_state32_t_gs(qint64 nOffset,quint32 nValue);

    qint64 get_x86_thread_state32_t_size();

    void _set_x86_thread_state64_t_rax(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rbx(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rcx(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rdx(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rdi(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rsi(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rbp(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rsp(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r8(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r9(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r10(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r11(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r12(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r13(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r14(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_r15(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rip(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_rflags(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_cs(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_fs(qint64 nOffset,quint64 nValue);
    void _set_x86_thread_state64_t_gs(qint64 nOffset,quint64 nValue);

    qint64 get_x86_thread_state64_t_size();

    void _set_ppc_thread_state32_t_srr0(qint64 nOffset,quint32 nValue);
    void _set_ppc_thread_state32_t_srr1(qint64 nOffset,quint32 nValue);
    void _set_ppc_thread_state32_t_r(qint64 nOffset,quint32 nValue,qint32 nIndex);
    void _set_ppc_thread_state32_t_ct(qint64 nOffset,quint32 nValue);
    void _set_ppc_thread_state32_t_xer(qint64 nOffset,quint32 nValue);
    void _set_ppc_thread_state32_t_lr(qint64 nOffset,quint32 nValue);
    void _set_ppc_thread_state32_t_ctr(qint64 nOffset,quint32 nValue);
    void _set_ppc_thread_state32_t_mq(qint64 nOffset,quint32 nValue);
    void _set_ppc_thread_state32_t_vrsave(qint64 nOffset,quint32 nValue);

    qint64 get_ppc_thread_state32_t_size();

    void _set_arm_thread_state32_t_r(qint64 nOffset,quint32 nValue,qint32 nIndex);
    void _set_arm_thread_state32_t_sp(qint64 nOffset,quint32 nValue);
    void _set_arm_thread_state32_t_lr(qint64 nOffset,quint32 nValue);
    void _set_arm_thread_state32_t_pc(qint64 nOffset,quint32 nValue);
    void _set_arm_thread_state32_t_cpsr(qint64 nOffset,quint32 nValue);

    qint64 get_arm_thread_state32_t_size();

    void _set_arm_thread_state64_t_x(qint64 nOffset,quint64 nValue,qint32 nIndex);
    void _set_arm_thread_state64_t_fp(qint64 nOffset,quint64 nValue);
    void _set_arm_thread_state64_t_lr(qint64 nOffset,quint64 nValue);
    void _set_arm_thread_state64_t_sp(qint64 nOffset,quint64 nValue);
    void _set_arm_thread_state64_t_pc(qint64 nOffset,quint64 nValue);
    void _set_arm_thread_state64_t_cpsr(qint64 nOffset,quint64 nValue);
    void _set_arm_thread_state64_t_pad(qint64 nOffset,quint64 nValue);

    qint64 get_arm_thread_state64_t_size();

    void _set_nlist_n_strx(qint64 nOffset,quint32 nValue);
    void _set_nlist_n_type(qint64 nOffset,quint8 nValue);
    void _set_nlist_n_sect(qint64 nOffset,quint8 nValue);
    void _set_nlist_n_desc(qint64 nOffset,quint16 nValue);
    void _set_nlist_n_value(qint64 nOffset,quint32 nValue);

    qint64 get_nlist_size();

    void _set_nlist_64_n_strx(qint64 nOffset,quint32 nValue);
    void _set_nlist_64_n_type(qint64 nOffset,quint8 nValue);
    void _set_nlist_64_n_sect(qint64 nOffset,quint8 nValue);
    void _set_nlist_64_n_desc(qint64 nOffset,quint16 nValue);
    void _set_nlist_64_n_value(qint64 nOffset,quint64 nValue);

    qint64 get_nlist_64_size();

    void _set_data_in_code_entry_offset(qint64 nOffset,quint32 nValue);
    void _set_data_in_code_entry_length(qint64 nOffset,quint16 nValue);
    void _set_data_in_code_entry_kind(qint64 nOffset,quint16 nValue);

    qint64 get_data_in_code_entry_size();

    XMACH_DEF::dyld_info_command _read_dyld_info_command(qint64 nOffset);
    XMACH_DEF::symtab_command _read_symtab_command(qint64 nOffset);
    XMACH_DEF::dysymtab_command _read_dysymtab_command(qint64 nOffset);
    XMACH_DEF::version_min_command _read_version_min_command(qint64 nOffset);
    XMACH_DEF::dylinker_command _read_dylinker_command(qint64 nOffset);
    XMACH_DEF::rpath_command _read_rpath_command(qint64 nOffset);
    XMACH_DEF::source_version_command _read_source_version_command(qint64 nOffset);
    XMACH_DEF::encryption_info_command _read_encryption_info_command(qint64 nOffset);
    XMACH_DEF::encryption_info_command_64 _read_encryption_info_command_64(qint64 nOffset);
    XMACH_DEF::entry_point_command _read_entry_point_command(qint64 nOffset);
    XMACH_DEF::x86_thread_state32_t _read_x86_thread_state32_t(qint64 nOffset);
    XMACH_DEF::x86_thread_state64_t _read_x86_thread_state64_t(qint64 nOffset);
    XMACH_DEF::ppc_thread_state32_t _read_ppc_thread_state32_t(qint64 nOffset);
    XMACH_DEF::arm_thread_state32_t _read_arm_thread_state32_t(qint64 nOffset);
    XMACH_DEF::arm_thread_state64_t _read_arm_thread_state64_t(qint64 nOffset);
    XMACH_DEF::state_hdr_t _read_state_hdr_t(qint64 nOffset);
    XMACH_DEF::linkedit_data_command _read_linkedit_data_command(qint64 nOffset);
    XMACH_DEF::unix_thread_command _read_unix_thread_command(qint64 nOffset);
    XMACH_DEF::nlist _read_nlist(qint64 nOffset);
    XMACH_DEF::nlist_64 _read_nlist_64(qint64 nOffset);
    XMACH_DEF::data_in_code_entry _read_data_in_code_entry(qint64 nOffset);

    QList<NLIST_RECORD> getNlistRecords();
    QList<NLIST_RECORD> getNlistRecords(QList<COMMAND_RECORD> *pListCommandRecords);

    static NLIST_RECORD searchNlistRecordByValue(QList<NLIST_RECORD> *pList,quint64 nValue);

    OFFSETSIZE getStringTableOS();
    OFFSETSIZE getStringTableOS(QList<COMMAND_RECORD> *pListCommandRecords);

    XMACH_DEF::dyld_info_command get_dyld_info();
    XMACH_DEF::symtab_command get_symtab();
    XMACH_DEF::dysymtab_command get_dysymtab();
    XMACH_DEF::encryption_info_command get_encryption_info();
    XMACH_DEF::encryption_info_command_64 get_encryption_info_64();
    XMACH_DEF::linkedit_data_command get_linkedit_data(quint32 nCommandID);

    QList<FUNCTION_RECORD> getFunctionRecords(qint64 nOffset,qint64 nSize);

    QList<DICE_RECORD> getDiceRecords(qint64 nOffset,qint64 nSize);

    virtual MODE getMode();
    virtual QString getArch();
    virtual FT getFileType();
    virtual int getType();
    virtual QString typeIdToString(int nType);

    enum OPCODE_TYPE
    {
        OPCODE_TYPE_UNKNOWN=0,
        OPCODE_TYPE_BIND,
        OPCODE_TYPE_EXPORT,
        OPCODE_TYPE_LAZY_BIND,
        OPCODE_TYPE_REBASE,
        OPCODE_TYPE_WEAK_BIND
    };

    virtual qint64 readOpcodes(quint32 nType,char *pData,qint64 nStartAddress,qint64 nSize,QList<OPCODE> *pListOpcodes,B_ERROR *pError);
};

#endif // XMACH_H
