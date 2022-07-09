/*	$OpenBSD: tracepoint.h,v 1.1 2020/01/21 16:16:23 mpi Exp $ */

/*
 * Copyright (c) 2019 Martin Pieuchot <mpi@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _SYS_TRACEPOINT_H_
#define	_SYS_TRACEPOINT_H_

#ifdef _KERNEL

#include "dt.h"
#if NDT > 0
#include <dev/dt/dtvar.h>

#define	TRACEPOINT(func, name, args...)	DT_STATIC_ENTER(func, name, args)

#else /* NDT > 0 */

#define	TRACEPOINT(func, name, args...)

#endif /* NDT > 0 */
#endif /* _KERNEL */
#endif /* _SYS_TRACEPOINT_H_ */
