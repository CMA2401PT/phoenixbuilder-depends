/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RUSERS_H_RPCGEN
#define _RUSERS_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>

/*
 * Find out about remote users
 */
#define RUSERS_MAXUSERLEN 32
#define RUSERS_MAXLINELEN 32
#define RUSERS_MAXHOSTLEN 257

struct rusers_utmp {
	char *ut_user;
	char *ut_line;
	char *ut_host;
	int ut_type;
	int ut_time;
	u_int ut_idle;
};
typedef struct rusers_utmp rusers_utmp;

typedef struct {
	unsigned int utmp_array_len;
	rusers_utmp *utmp_array_val;
} utmp_array;

/*
 * Values for ut_type field above.
 */
#define RUSERS_EMPTY 0
#define RUSERS_RUN_LVL 1
#define RUSERS_BOOT_TIME 2
#define RUSERS_OLD_TIME 3
#define RUSERS_NEW_TIME 4
#define RUSERS_INIT_PROCESS 5
#define RUSERS_LOGIN_PROCESS 6
#define RUSERS_USER_PROCESS 7
#define RUSERS_DEAD_PROCESS 8
#define RUSERS_ACCOUNTING 9

__BEGIN_DECLS
bool_t xdr_rusers_utmp(XDR *, rusers_utmp *);
bool_t xdr_utmp_array(XDR *, utmp_array *);
__END_DECLS

#define RUSERSPROG 100002
#define RUSERSVERS_3 3
#define RUSERSPROC_NUM 1
#define RUSERSPROC_NAMES 2
#define RUSERSPROC_ALLNAMES 3

__BEGIN_DECLS
int *rusersproc_num_3(void *, CLIENT *);
int *rusersproc_num_3_svc(void *, struct svc_req *);
utmp_array *rusersproc_names_3(void *, CLIENT *);
utmp_array *rusersproc_names_3_svc(void *, struct svc_req *);
utmp_array *rusersproc_allnames_3(void *, CLIENT *);
utmp_array *rusersproc_allnames_3_svc(void *, struct svc_req *);
__END_DECLS

#endif /* !_RUSERS_H_RPCGEN */
