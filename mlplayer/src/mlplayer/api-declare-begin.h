/*
 * api-declare-begin.h
 * Copyright 2010 John Lindgren
 *
 * This file is part of MLPlayer.
 *
 * MLPlayer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 2 or version 3 of the License.
 *
 * MLPlayer is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * MLPlayer. If not, see <http://www.gnu.org/licenses/>.
 *
 * The MLPlayer team does not consider modular code linking to MLPlayer or
 * using our public API to be a derived work.
 */

#if ! defined AUD_API_NAME || ! defined AUD_API_SYMBOL || defined AUD_API_DECLARE_H
#error Bad usage of api-declare-begin.h
#endif

#define AUD_API_DECLARE_H

#define AUD_FUNC0(t,n) .n = n,
#define AUD_FUNC1(t,n,t1,n1) .n = n,
#define AUD_FUNC2(t,n,t1,n1,t2,n2) .n = n,
#define AUD_FUNC3(t,n,t1,n1,t2,n2,t3,n3) .n = n,
#define AUD_FUNC4(t,n,t1,n1,t2,n2,t3,n3,t4,n4) .n = n,
#define AUD_FUNC5(t,n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5) .n = n,
#define AUD_FUNC6(t,n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5,t6,n6) .n = n,
#define AUD_FUNC7(t,n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5,t6,n6,t7,n7) .n = n,
#define AUD_FUNC8(t,n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5,t6,n6,t7,n7,t8,n8) .n = n,

#define AUD_VFUNC0(n) .n = n,
#define AUD_VFUNC1(n,t1,n1) .n = n,
#define AUD_VFUNC2(n,t1,n1,t2,n2) .n = n,
#define AUD_VFUNC3(n,t1,n1,t2,n2,t3,n3) .n = n,
#define AUD_VFUNC4(n,t1,n1,t2,n2,t3,n3,t4,n4) .n = n,
#define AUD_VFUNC5(n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5) .n = n,
#define AUD_VFUNC6(n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5,t6,n6) .n = n,
#define AUD_VFUNC7(n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5,t6,n6,t7,n7) .n = n,
#define AUD_VFUNC8(n,t1,n1,t2,n2,t3,n3,t4,n4,t5,n5,t6,n6,t7,n7,t8,n8) .n = n,

const struct AUD_API_NAME AUD_API_SYMBOL = {
