/*	$NetBSD: namei.h,v 1.98.2.1 2021/06/21 14:52:58 martin Exp $	*/


/*
 * WARNING: GENERATED FILE.  DO NOT EDIT
 * (edit namei.src and run make namei in src/sys/sys)
 *   by:   NetBSD: gennameih.awk,v 1.5 2009/12/23 14:17:19 pooka Exp 
 *   from: NetBSD: namei.src,v 1.42.2.1 2021/06/21 14:50:57 martin Exp 
 */

/*
 * Copyright (c) 1985, 1989, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)namei.h	8.5 (Berkeley) 8/20/94
 */

#ifndef _SYS_NAMEI_H_
#define	_SYS_NAMEI_H_

#include <sys/queue.h>
#include <sys/mutex.h>

#ifdef _KERNEL
#include <sys/kauth.h>

/*
 * Abstraction for a single pathname.
 *
 * This contains both the pathname string and (eventually) all
 * metadata that determines how the path is to be interpreted.
 * It is an opaque structure; the implementation is in vfs_lookup.c.
 *
 * To call namei, first set up a pathbuf with pathbuf_create or
 * pathbuf_copyin, then do NDINIT(), then call namei, then AFTER THE
 * STRUCT NAMEIDATA IS DEAD, call pathbuf_destroy. Don't destroy the
 * pathbuf before you've finished using the nameidata, or mysterious
 * bad things may happen.
 *
 * pathbuf_assimilate is like pathbuf_create but assumes ownership of
 * the string buffer passed in, which MUST BE of size PATH_MAX and
 * have been allocated with PNBUF_GET(). This should only be used when
 * absolutely necessary; e.g. nfsd uses it for loading paths from
 * mbufs.
 */
struct pathbuf;

struct pathbuf *pathbuf_create(const char *path);
struct pathbuf *pathbuf_assimilate(char *path);
int pathbuf_copyin(const char *userpath, struct pathbuf **ret);
void pathbuf_destroy(struct pathbuf *);

/* get a copy of the (current) path string */
void pathbuf_copystring(const struct pathbuf *, char *buf, size_t maxlen);

/* hold a reference copy of the original path string */
const char *pathbuf_stringcopy_get(struct pathbuf *);
void pathbuf_stringcopy_put(struct pathbuf *, const char *);

// XXX remove this
int pathbuf_maybe_copyin(const char *userpath, enum uio_seg seg, struct pathbuf **ret);

/*
 * Lookup parameters: this structure describes the subset of
 * information from the nameidata structure that is passed
 * through the VOP interface.
 */
struct componentname {
	/*
	 * Arguments to lookup.
	 */
	uint32_t	cn_nameiop;	/* namei operation */
	uint32_t	cn_flags;	/* flags to namei */
	kauth_cred_t 	cn_cred;	/* credentials */
	/*
	 * Shared between lookup and commit routines.
	 */
	const char 	*cn_nameptr;	/* pointer to looked up name */
	size_t		cn_namelen;	/* length of looked up comp */
	size_t		cn_consume;	/* chars to consume in lookup */
};

/*
 * Encapsulation of namei parameters.
 */
struct nameidata {
	/*
	 * Arguments to namei/lookup.
	 */
	struct vnode *ni_atdir;		/* startup dir, cwd if null */
	struct pathbuf *ni_pathbuf;	/* pathname container */
	char *ni_pnbuf;			/* extra pathname buffer ref (XXX) */
	/*
	 * Arguments to lookup.
	 */
	struct	vnode *ni_rootdir;	/* logical root directory */
	struct	vnode *ni_erootdir;	/* emulation root directory */
	/*
	 * Results: returned from/manipulated by lookup
	 */
	struct	vnode *ni_vp;		/* vnode of result */
	struct	vnode *ni_dvp;		/* vnode of intermediate directory */
	/*
	 * Shared between namei and lookup/commit routines.
	 */
	size_t		ni_pathlen;	/* remaining chars in path */
	const char	*ni_next;	/* next location in pathname */
	unsigned int	ni_loopcnt;	/* count of symlinks encountered */
	/*
	 * Lookup parameters: this structure describes the subset of
	 * information from the nameidata structure that is passed
	 * through the VOP interface.
	 */
	struct componentname ni_cnd;
};

/*
 * namei operations
 */
#define	LOOKUP		0	/* perform name lookup only */
#define	CREATE		1	/* setup for file creation */
#define	DELETE		2	/* setup for file deletion */
#define	RENAME		3	/* setup for file renaming */
#define	OPMASK		3	/* mask for operation */
/*
 * namei operational modifier flags, stored in ni_cnd.cn_flags
 */
#define	LOCKLEAF	0x00000004	/* lock inode on return */
#define	LOCKPARENT	0x00000008	/* want parent vnode returned locked */
#define	TRYEMULROOT	0x00000010	/* try relative to emulation root
					   first */
#define	NOCACHE		0x00000020	/* name must not be left in cache */
#define	FOLLOW		0x00000040	/* follow symbolic links */
#define	NOFOLLOW	0x00000000	/* do not follow symbolic links
					   (pseudo) */
#define	EMULROOTSET	0x00000080	/* emulation root already
					   in ni_erootdir */
#define	NOCHROOT	0x01000000	/* no chroot on abs path lookups */
#define	NONEXCLHACK	0x02000000	/* open wwith O_CREAT but not O_EXCL */
#define	MODMASK		0x030000fc	/* mask of operational modifiers */
/*
 * Namei parameter descriptors.
 */
#define	NOCROSSMOUNT	0x0000100	/* do not cross mount points */
#define	RDONLY		0x0000200	/* lookup with read-only semantics */
#define	ISDOTDOT	0x0002000	/* current component name is .. */
#define	MAKEENTRY	0x0004000	/* entry is to be added to name cache */
#define	ISLASTCN	0x0008000	/* this is last component of pathname */
#define	ISWHITEOUT	0x0020000	/* found whiteout */
#define	DOWHITEOUT	0x0040000	/* do whiteouts */
#define	REQUIREDIR	0x0080000	/* must be a directory */
#define	CREATEDIR	0x0200000	/* trailing slashes are ok */
#define	PARAMASK	0x02ee300	/* mask of parameter descriptors */

/*
 * Initialization of a nameidata structure.
 */
#define NDINIT(ndp, op, flags, pathbuf) { \
	(ndp)->ni_cnd.cn_nameiop = op; \
	(ndp)->ni_cnd.cn_flags = flags; \
	(ndp)->ni_atdir = NULL; \
	(ndp)->ni_pathbuf = pathbuf; \
	(ndp)->ni_cnd.cn_cred = kauth_cred_get(); \
}

/*
 * Use this to set the start directory for openat()-type operations.
 */
#define NDAT(ndp, dir) {			\
	(ndp)->ni_atdir = (dir);		\
}

#endif

/*
 * This structure describes the elements in the cache of recent
 * names looked up by namei. NCHNAMLEN is sized to make structure
 * size a power of two to optimize allocations.  Minimum reasonable
 * size is 15.
 */

#define	NCHNAMLEN	31	/* maximum name segment length we bother with */

/*
 * Namecache entry.  This structure is arranged so that frequently
 * accessed and mostly read-only data is toward the front, with
 * infrequently accessed data and the lock towards the rear.  The
 * lock is then more likely to be in a separate cache line.
 *
 * Locking rules:
 *
 *      -       stable after initialization
 *      L       namecache_lock
 *      C       struct nchcpu::cpu_lock
 *      L/C     insert needs L, read needs L or any C,
 *              must hold L and all C after (or during) delete before free
 *      N       struct namecache::nc_lock
 */
struct namecache {
	LIST_ENTRY(namecache) nc_hash;	/* L/C hash chain */
	LIST_ENTRY(namecache) nc_vhash;	/* L directory hash chain */
	struct	vnode *nc_dvp;		/* N vnode of parent of name */
	struct	vnode *nc_vp;		/* N vnode the name refers to */
	int	nc_flags;		/* - copy of componentname ISWHITEOUT */
	char	nc_nlen;		/* - length of name */
	char	nc_name[NCHNAMLEN];	/* - segment name */
	void	*nc_gcqueue;		/* N queue for garbage collection */
	TAILQ_ENTRY(namecache) nc_lru;	/* L psuedo-lru chain */
	LIST_ENTRY(namecache) nc_dvlist;/* L dvp's list of cache entries */
	LIST_ENTRY(namecache) nc_vlist; /* L vp's list of cache entries */
	kmutex_t nc_lock;		/*   lock on this entry */
	int	nc_hittime;		/* N last time scored a hit */
};

#ifdef _KERNEL
#include <sys/pool.h>

struct mount;
struct cpu_info;

extern pool_cache_t pnbuf_cache;	/* pathname buffer cache */

#define	PNBUF_GET()	((char *)pool_cache_get(pnbuf_cache, PR_WAITOK))
#define	PNBUF_PUT(pnb)	pool_cache_put(pnbuf_cache, (void *)(pnb))

/*
 * Typesafe flags for namei_simple/nameiat_simple.
 *
 * This encoding is not optimal but serves the important purpose of
 * not being type-compatible with the regular namei flags.
 */
struct namei_simple_flags_type; /* Opaque. */
typedef const struct namei_simple_flags_type *namei_simple_flags_t; /* Gross. */
extern const namei_simple_flags_t
	NSM_NOFOLLOW_NOEMULROOT,
	NSM_NOFOLLOW_TRYEMULROOT,
	NSM_FOLLOW_NOEMULROOT,
	NSM_FOLLOW_TRYEMULROOT;

/*
 * namei(at)?_simple_* - the simple cases of namei, with no struct
 *                       nameidata involved.
 *
 * namei_simple_kernel takes a kernel-space path as the first argument.
 * namei_simple_user takes a user-space path as the first argument.
 * The nameiat_simple_* variants handle relative path using the given 
 * directory vnode instead of current directory.
 *
 * A namei call can be converted to namei_simple_* if:
 *    - the second arg to NDINIT is LOOKUP;
 *    - it does not need the parent vnode, nd.ni_dvp;
 *    - the only flags it uses are (NO)FOLLOW and TRYEMULROOT;
 *    - it does not do anything else gross with the contents of nd.
 */
int namei_simple_kernel(const char *, namei_simple_flags_t, struct vnode **);
int namei_simple_user(const char *, namei_simple_flags_t, struct vnode **);
int nameiat_simple_kernel(struct vnode *, const char *, namei_simple_flags_t,
    struct vnode **);
int nameiat_simple_user(struct vnode *, const char *, namei_simple_flags_t, 
    struct vnode **);

int	namei(struct nameidata *);
uint32_t namei_hash(const char *, const char **);
int	lookup_for_nfsd(struct nameidata *, struct vnode *, int neverfollow);
int	lookup_for_nfsd_index(struct nameidata *, struct vnode *);
int	relookup(struct vnode *, struct vnode **, struct componentname *, int);
void	cache_purge1(struct vnode *, const char *, size_t, int);
#define	PURGE_PARENTS	1
#define	PURGE_CHILDREN	2
#define	cache_purge(vp)	cache_purge1((vp),NULL,0,PURGE_PARENTS|PURGE_CHILDREN)
bool	cache_lookup(struct vnode *, const char *, size_t, uint32_t, uint32_t,
			int *, struct vnode **);
bool	cache_lookup_raw(struct vnode *, const char *, size_t, uint32_t,
			int *, struct vnode **);
int	cache_revlookup(struct vnode *, struct vnode **, char **, char *);
void	cache_enter(struct vnode *, struct vnode *,
			const char *, size_t, uint32_t);
void	nchinit(void);
void	nchreinit(void);
void	namecache_count_pass2(void);
void	namecache_count_2passes(void);
void	cache_cpu_init(struct cpu_info *);
void	cache_purgevfs(struct mount *);
void	namecache_print(struct vnode *, void (*)(const char *, ...)
    __printflike(1, 2));

#endif

/*
 * Stats on usefulness of namei caches.  A couple of structures are
 * used for counting, with members having the same names but different
 * types.  Containerize member names with the preprocessor to avoid
 * cut-'n'-paste.  A (U) in the comment documents values that are
 * incremented unlocked; we may treat these specially.
 */
#define	_NAMEI_CACHE_STATS(type) {					\
	type	ncs_goodhits;	/* hits that we can really use (U) */	\
	type	ncs_neghits;	/* negative hits that we can use */	\
	type	ncs_badhits;	/* hits we must drop */			\
	type	ncs_falsehits;	/* hits with id mismatch (U) */		\
	type	ncs_miss;	/* misses */				\
	type	ncs_long;	/* long names that ignore cache */	\
	type	ncs_pass2;	/* names found with passes == 2 (U) */	\
	type	ncs_2passes;	/* number of times we attempt it (U) */	\
	type	ncs_revhits;	/* reverse-cache hits */		\
	type	ncs_revmiss;	/* reverse-cache misses */		\
}

/*
 * Sysctl deals with a uint64_t version of the stats and summary
 * totals are kept that way.
 */
struct	nchstats _NAMEI_CACHE_STATS(uint64_t);

/* #endif !_SYS_NAMEI_H_ (generated by gennameih.awk) */

/* Definitions match above, but with NAMEI_ prefix */
#define NAMEI_LOOKUP	0
#define NAMEI_CREATE	1
#define NAMEI_DELETE	2
#define NAMEI_RENAME	3
#define NAMEI_OPMASK	3
#define NAMEI_LOCKLEAF	0x00000004
#define NAMEI_LOCKPARENT	0x00000008
#define NAMEI_TRYEMULROOT	0x00000010
#define NAMEI_NOCACHE	0x00000020
#define NAMEI_FOLLOW	0x00000040
#define NAMEI_NOFOLLOW	0x00000000
#define NAMEI_EMULROOTSET	0x00000080
#define NAMEI_NOCHROOT	0x01000000
#define NAMEI_NONEXCLHACK	0x02000000
#define NAMEI_MODMASK	0x030000fc
#define NAMEI_NOCROSSMOUNT	0x0000100
#define NAMEI_RDONLY	0x0000200
#define NAMEI_ISDOTDOT	0x0002000
#define NAMEI_MAKEENTRY	0x0004000
#define NAMEI_ISLASTCN	0x0008000
#define NAMEI_ISWHITEOUT	0x0020000
#define NAMEI_DOWHITEOUT	0x0040000
#define NAMEI_REQUIREDIR	0x0080000
#define NAMEI_CREATEDIR	0x0200000
#define NAMEI_PARAMASK	0x02ee300

#endif /* !_SYS_NAMEI_H_ */
