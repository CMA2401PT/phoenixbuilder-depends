/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RSTAT_H_RPCGEN
#define _RSTAT_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>

#ifndef FSCALE
/*
 * Scale factor for scaled integers used to count load averages.
 */
#define FSHIFT 8 /* bits to right of fixed binary point */
#define FSCALE (1<<FSHIFT)

#endif /* ndef FSCALE */
#define CPUSTATES 4
#define DK_NDRIVE 4

struct rstat_timeval {
	u_int tv_sec;
	u_int tv_usec;
};
typedef struct rstat_timeval rstat_timeval;

struct statstime {
	int cp_time[CPUSTATES];
	int dk_xfer[DK_NDRIVE];
	u_int v_pgpgin;
	u_int v_pgpgout;
	u_int v_pswpin;
	u_int v_pswpout;
	u_int v_intr;
	int if_ipackets;
	int if_ierrors;
	int if_oerrors;
	int if_collisions;
	u_int v_swtch;
	int avenrun[3];
	rstat_timeval boottime;
	rstat_timeval curtime;
	int if_opackets;
};
typedef struct statstime statstime;

struct statsswtch {
	int cp_time[CPUSTATES];
	int dk_xfer[DK_NDRIVE];
	u_int v_pgpgin;
	u_int v_pgpgout;
	u_int v_pswpin;
	u_int v_pswpout;
	u_int v_intr;
	int if_ipackets;
	int if_ierrors;
	int if_oerrors;
	int if_collisions;
	u_int v_swtch;
	u_int avenrun[3];
	rstat_timeval boottime;
	int if_opackets;
};
typedef struct statsswtch statsswtch;

struct stats {
	int cp_time[CPUSTATES];
	int dk_xfer[DK_NDRIVE];
	u_int v_pgpgin;
	u_int v_pgpgout;
	u_int v_pswpin;
	u_int v_pswpout;
	u_int v_intr;
	int if_ipackets;
	int if_ierrors;
	int if_oerrors;
	int if_collisions;
	int if_opackets;
};
typedef struct stats stats;

__BEGIN_DECLS
bool_t xdr_rstat_timeval(XDR *, rstat_timeval *);
bool_t xdr_statstime(XDR *, statstime *);
bool_t xdr_statsswtch(XDR *, statsswtch *);
bool_t xdr_stats(XDR *, stats *);
__END_DECLS

#define RSTATPROG 100001
#define RSTATVERS_TIME 3
#define RSTATPROC_STATS 1
#define RSTATPROC_HAVEDISK 2
#define RSTATVERS_SWTCH 2
#define RSTATVERS_ORIG 1

__BEGIN_DECLS
statstime *rstatproc_stats_3(void *, CLIENT *);
statstime *rstatproc_stats_3_svc(void *, struct svc_req *);
u_int *rstatproc_havedisk_3(void *, CLIENT *);
u_int *rstatproc_havedisk_3_svc(void *, struct svc_req *);
statsswtch *rstatproc_stats_2(void *, CLIENT *);
statsswtch *rstatproc_stats_2_svc(void *, struct svc_req *);
u_int *rstatproc_havedisk_2(void *, CLIENT *);
u_int *rstatproc_havedisk_2_svc(void *, struct svc_req *);
stats *rstatproc_stats_1(void *, CLIENT *);
stats *rstatproc_stats_1_svc(void *, struct svc_req *);
u_int *rstatproc_havedisk_1(void *, CLIENT *);
u_int *rstatproc_havedisk_1_svc(void *, struct svc_req *);
__END_DECLS

#endif /* !_RSTAT_H_RPCGEN */
