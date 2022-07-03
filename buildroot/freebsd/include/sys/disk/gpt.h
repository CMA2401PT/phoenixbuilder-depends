/*-
 * Copyright (c) 2002 Marcel Moolenaar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef _SYS_DISK_GPT_H_
#define	_SYS_DISK_GPT_H_

#include <sys/types.h>

/*
 * Applications can define GPT_UUID_TYPE if they want the GPT structures
 * to use a particular type definition for UUIDs/GUIDs.  This header uses
 * a generic (DCE 1.1 compatible) definition otherwise.
 */
#ifndef GPT_UUID_TYPE
struct gpt_uuid {
	uint32_t	time_low;
	uint16_t	time_mid;
	uint16_t	time_hi_and_version;
	uint8_t		clock_seq_hi_and_reserved;
	uint8_t		clock_seq_low;
	uint8_t		node[6];
};
#define	GPT_UUID_TYPE	struct gpt_uuid
#endif /* !GPT_UUID_TYPE */

typedef GPT_UUID_TYPE gpt_uuid_t;

#ifdef CTASSERT
CTASSERT(sizeof(gpt_uuid_t) == 16);
#endif

struct gpt_hdr {
	char		hdr_sig[8];
#define	GPT_HDR_SIG		"EFI PART"
	uint32_t	hdr_revision;
#define	GPT_HDR_REVISION	0x00010000
	uint32_t	hdr_size;
	uint32_t	hdr_crc_self;
	uint32_t	__reserved;
	uint64_t	hdr_lba_self;
	uint64_t	hdr_lba_alt;
	uint64_t	hdr_lba_start;
	uint64_t	hdr_lba_end;
	gpt_uuid_t	hdr_uuid;
	uint64_t	hdr_lba_table;
	uint32_t	hdr_entries;
	uint32_t	hdr_entsz;
	uint32_t	hdr_crc_table;
	/*
	 * The header as defined in the EFI spec is not a multiple of 8 bytes
	 * and given that the alignment requirement is on an 8 byte boundary,
	 * padding will happen. We make the padding explicit so that we can
	 * correct the value returned by sizeof() when we put the size of the
	 * header in field hdr_size, or otherwise use offsetof().
	 */
	uint32_t	padding;
};
#ifdef CTASSERT
CTASSERT(offsetof(struct gpt_hdr, padding) == 92);
#endif

struct gpt_ent {
	gpt_uuid_t	ent_type;
	gpt_uuid_t	ent_uuid;
	uint64_t	ent_lba_start;
	uint64_t	ent_lba_end;
	uint64_t	ent_attr;
#define	GPT_ENT_ATTR_PLATFORM		(1ULL << 0)
#define	GPT_ENT_ATTR_BOOTME		(1ULL << 59)
#define	GPT_ENT_ATTR_BOOTONCE		(1ULL << 58)
#define	GPT_ENT_ATTR_BOOTFAILED		(1ULL << 57)
	uint16_t	ent_name[36];		/* UTF-16. */
};
#ifdef CTASSERT
CTASSERT(sizeof(struct gpt_ent) == 128);
#endif /* CTASSERT */

#define	GPT_ENT_TYPE_UNUSED		\
	{0x00000000,0x0000,0x0000,0x00,0x00,{0x00,0x00,0x00,0x00,0x00,0x00}}
#define	GPT_ENT_TYPE_EFI		\
	{0xc12a7328,0xf81f,0x11d2,0xba,0x4b,{0x00,0xa0,0xc9,0x3e,0xc9,0x3b}}
#define	GPT_ENT_TYPE_MBR		\
	{0x024dee41,0x33e7,0x11d3,0x9d,0x69,{0x00,0x08,0xc7,0x81,0xf3,0x9f}}
#define	GPT_ENT_TYPE_FREEBSD		\
	{0x516e7cb4,0x6ecf,0x11d6,0x8f,0xf8,{0x00,0x02,0x2d,0x09,0x71,0x2b}}
#define	GPT_ENT_TYPE_FREEBSD_BOOT	\
	{0x83bd6b9d,0x7f41,0x11dc,0xbe,0x0b,{0x00,0x15,0x60,0xb8,0x4f,0x0f}}
#define	GPT_ENT_TYPE_FREEBSD_NANDFS	\
	{0x74ba7dd9,0xa689,0x11e1,0xbd,0x04,{0x00,0xe0,0x81,0x28,0x6a,0xcf}}
#define	GPT_ENT_TYPE_FREEBSD_SWAP	\
	{0x516e7cb5,0x6ecf,0x11d6,0x8f,0xf8,{0x00,0x02,0x2d,0x09,0x71,0x2b}}
#define	GPT_ENT_TYPE_FREEBSD_UFS	\
	{0x516e7cb6,0x6ecf,0x11d6,0x8f,0xf8,{0x00,0x02,0x2d,0x09,0x71,0x2b}}
#define	GPT_ENT_TYPE_FREEBSD_VINUM	\
	{0x516e7cb8,0x6ecf,0x11d6,0x8f,0xf8,{0x00,0x02,0x2d,0x09,0x71,0x2b}}
#define	GPT_ENT_TYPE_FREEBSD_ZFS	\
	{0x516e7cba,0x6ecf,0x11d6,0x8f,0xf8,{0x00,0x02,0x2d,0x09,0x71,0x2b}}
#define	GPT_ENT_TYPE_PREP_BOOT		\
	{0x9e1a2d38,0xc612,0x4316,0xaa,0x26,{0x8b,0x49,0x52,0x1e,0x5a,0x8b}}

/*
 * The following are unused but documented here to avoid reuse.
 *
 * GPT_ENT_TYPE_FREEBSD_UFS2	\
 *	{0x516e7cb7,0x6ecf,0x11d6,0x8f,0xf8,{0x00,0x02,0x2d,0x09,0x71,0x2b}}
 */

/*
 * Foreign partition types that we're likely to encounter. Note that Linux
 * apparently choose to share data partitions with MS. I don't what the
 * advantage might be. I can see how sharing swap partitions is advantageous
 * though.
 */
#define	GPT_ENT_TYPE_MS_BASIC_DATA	\
	{0xebd0a0a2,0xb9e5,0x4433,0x87,0xc0,{0x68,0xb6,0xb7,0x26,0x99,0xc7}}
#define	GPT_ENT_TYPE_MS_LDM_DATA	\
	{0xaf9b60a0,0x1431,0x4f62,0xbc,0x68,{0x33,0x11,0x71,0x4a,0x69,0xad}}
#define	GPT_ENT_TYPE_MS_LDM_METADATA	\
	{0x5808c8aa,0x7e8f,0x42e0,0x85,0xd2,{0xe1,0xe9,0x04,0x34,0xcf,0xb3}}
#define	GPT_ENT_TYPE_MS_RECOVERY	\
	{0xde94bba4,0x06d1,0x4d40,0xa1,0x6a,{0xbf,0xd5,0x01,0x79,0xd6,0xac}}
#define	GPT_ENT_TYPE_MS_RESERVED	\
	{0xe3c9e316,0x0b5c,0x4db8,0x81,0x7d,{0xf9,0x2d,0xf0,0x02,0x15,0xae}}
#define	GPT_ENT_TYPE_MS_SPACES	\
	{0xe75caf8f,0xf680,0x4cee,0xaf,0xa3,{0xb0,0x01,0xe5,0x6e,0xfc,0x2d}}

#define	GPT_ENT_TYPE_LINUX_DATA		\
	{0x0fc63daf,0x8483,0x4772,0x8e,0x79,{0x3d,0x69,0xd8,0x47,0x7d,0xe4}}
#define	GPT_ENT_TYPE_LINUX_RAID		\
	{0xa19d880f,0x05fc,0x4d3b,0xa0,0x06,{0x74,0x3f,0x0f,0x84,0x91,0x1e}}
#define	GPT_ENT_TYPE_LINUX_SWAP		\
	{0x0657fd6d,0xa4ab,0x43c4,0x84,0xe5,{0x09,0x33,0xc8,0x4b,0x4f,0x4f}}
#define	GPT_ENT_TYPE_LINUX_LVM		\
	{0xe6d6d379,0xf507,0x44c2,0xa2,0x3c,{0x23,0x8f,0x2a,0x3d,0xf9,0x28}}

#define	GPT_ENT_TYPE_VMFS		\
	{0xaa31e02a,0x400f,0x11db,0x95,0x90,{0x00,0x0c,0x29,0x11,0xd1,0xb8}}
#define	GPT_ENT_TYPE_VMKDIAG		\
	{0x9d275380,0x40ad,0x11db,0xbf,0x97,{0x00,0x0c,0x29,0x11,0xd1,0xb8}}
#define	GPT_ENT_TYPE_VMRESERVED		\
	{0x9198effc,0x31c0,0x11db,0x8f,0x78,{0x00,0x0c,0x29,0x11,0xd1,0xb8}}
#define	GPT_ENT_TYPE_VMVSANHDR		\
	{0x381cfccc,0x7288,0x11e0,0x92,0xee,{0x00,0x0c,0x29,0x11,0xd0,0xb2}}

#define	GPT_ENT_TYPE_APPLE_BOOT		\
	{0x426F6F74,0x0000,0x11aa,0xaa,0x11,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define	GPT_ENT_TYPE_APPLE_HFS		\
	{0x48465300,0x0000,0x11aa,0xaa,0x11,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define	GPT_ENT_TYPE_APPLE_UFS		\
	{0x55465300,0x0000,0x11aa,0xaa,0x11,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define	GPT_ENT_TYPE_APPLE_ZFS		\
	{0x6a898cc3,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define	GPT_ENT_TYPE_APPLE_RAID		\
	{0x52414944,0x0000,0x11aa,0xaa,0x22,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define	GPT_ENT_TYPE_APPLE_RAID_OFFLINE	\
	{0x52414944,0x5f4f,0x11aa,0xaa,0x22,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define GPT_ENT_TYPE_APPLE_LABEL	\
    	{0x4C616265,0x6c00,0x11aa,0xaa,0x11,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define	GPT_ENT_TYPE_APPLE_TV_RECOVERY	\
    	{0x5265636f,0x7665,0x11AA,0xaa,0x11,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define	GPT_ENT_TYPE_APPLE_CORE_STORAGE	\
	{0x53746f72,0x6167,0x11AA,0xaa,0x11,{0x00,0x30,0x65,0x43,0xec,0xac}}
#define	GPT_ENT_TYPE_APPLE_APFS	\
	{0x7c3457ef,0x0000,0x11aa,0xaa,0x11,{0x00,0x30,0x65,0x43,0xec,0xac}}

#define	GPT_ENT_TYPE_NETBSD_FFS		\
	{0x49f48d5a,0xb10e,0x11dc,0xb9,0x9b,{0x00,0x19,0xd1,0x87,0x96,0x48}}
#define	GPT_ENT_TYPE_NETBSD_LFS		\
	{0x49f48d82,0xb10e,0x11dc,0xb9,0x9b,{0x00,0x19,0xd1,0x87,0x96,0x48}}
#define	GPT_ENT_TYPE_NETBSD_SWAP	\
	{0x49f48d32,0xb10e,0x11dc,0xB9,0x9B,{0x00,0x19,0xd1,0x87,0x96,0x48}}
#define	GPT_ENT_TYPE_NETBSD_RAID	\
	{0x49f48daa,0xb10e,0x11dc,0xb9,0x9b,{0x00,0x19,0xd1,0x87,0x96,0x48}}
#define	GPT_ENT_TYPE_NETBSD_CCD		\
	{0x2db519c4,0xb10f,0x11dc,0xb9,0x9b,{0x00,0x19,0xd1,0x87,0x96,0x48}}
#define	GPT_ENT_TYPE_NETBSD_CGD		\
	{0x2db519ec,0xb10f,0x11dc,0xb9,0x9b,{0x00,0x19,0xd1,0x87,0x96,0x48}}

#define GPT_ENT_TYPE_DRAGONFLY_LABEL32	\
	{0x9d087404,0x1ca5,0x11dc,0x88,0x17,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_SWAP	\
	{0x9d58fdbd,0x1ca5,0x11dc,0x88,0x17,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_UFS1	\
	{0x9d94ce7c,0x1ca5,0x11dc,0x88,0x17,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_VINUM	\
	{0x9dd4478f,0x1ca5,0x11dc,0x88,0x17,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_CCD	\
	{0xdbd5211b,0x1ca5,0x11dc,0x88,0x17,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_LABEL64	\
	{0x3d48ce54,0x1d16,0x11dc,0x86,0x96,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_LEGACY	\
	{0xbd215ab2,0x1d16,0x11dc,0x86,0x96,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_HAMMER	\
	{0x61dc63ac,0x6e38,0x11dc,0x85,0x13,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}
#define GPT_ENT_TYPE_DRAGONFLY_HAMMER2	\
	{0x5cbb9ad1,0x862d,0x11dc,0xa9,0x4d,{0x01,0x30,0x1b,0xb8,0xa9,0xf5}}

#define GPT_ENT_TYPE_CHROMEOS_FIRMWARE	\
	{0xcab6e88e,0xabf3,0x4102,0xa0,0x7a,{0xd4,0xbb,0x9b,0xe3,0xc1,0xd3}}
#define GPT_ENT_TYPE_CHROMEOS_KERNEL	\
	{0xfe3a2a5d,0x4f32,0x41a7,0xb7,0x25,{0xac,0xcc,0x32,0x85,0xa3,0x09}}
#define GPT_ENT_TYPE_CHROMEOS_RESERVED	\
	{0x2e0a753d,0x9e48,0x43b0,0x83,0x37,{0xb1,0x51,0x92,0xcb,0x1b,0x5e}}
#define GPT_ENT_TYPE_CHROMEOS_ROOT	\
	{0x3cb8e202,0x3b7e,0x47dd,0x8a,0x3c,{0x7f,0xf2,0xa1,0x3c,0xfc,0xec}}

#define GPT_ENT_TYPE_OPENBSD_DATA	\
	{0x824cc7a0,0x36a8,0x11e3,0x89,0x0a,{0x95,0x25,0x19,0xad,0x3f,0x61}}

#define GPT_ENT_TYPE_SOLARIS_BOOT	\
	{0x6a82cb45,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define GPT_ENT_TYPE_SOLARIS_ROOT	\
	{0x6a85cf4d,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define GPT_ENT_TYPE_SOLARIS_SWAP	\
	{0x6a87c46f,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define GPT_ENT_TYPE_SOLARIS_BACKUP	\
	{0x6a8b642b,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define GPT_ENT_TYPE_SOLARIS_VAR	\
	{0x6a8ef2e9,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define GPT_ENT_TYPE_SOLARIS_HOME	\
	{0x6a90ba39,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define GPT_ENT_TYPE_SOLARIS_ALTSEC	\
	{0x6a9283a5,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}
#define GPT_ENT_TYPE_SOLARIS_RESERVED	\
	{0x6a945a3b,0x1dd2,0x11b2,0x99,0xa6,{0x08,0x00,0x20,0x73,0x66,0x31}}

#define GPT_ENT_TYPE_HIFIVE_FSBL	\
	{0x5b193300,0xfc78,0x40cd,0x80,0x02,{0xe8,0x6c,0x45,0x58,0x0b,0x47}}
#define GPT_ENT_TYPE_HIFIVE_BBL		\
	{0x2e54b353,0x1271,0x4842,0x80,0x6f,{0xe4,0x36,0xd6,0xaf,0x69,0x85}}

/*
 * Boot partition used by GRUB 2.
 */
#define	GPT_ENT_TYPE_BIOS_BOOT		\
	{0x21686148,0x6449,0x6e6f,0x74,0x4e,{0x65,0x65,0x64,0x45,0x46,0x49}}

#endif /* _SYS_DISK_GPT_H_ */
