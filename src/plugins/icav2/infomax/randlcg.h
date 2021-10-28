// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

/* randlcg.h	prototypes for the minimal standard random number generator,

   Linear Congruential Method, the "minimal standard generator"
   Park & Miller, 1988, Comm of the ACM, 31(10), pp. 1192-1201


  rcsid: @(#)randlcg.h	1.1 15:48:09 11/21/94   EFC

*/

#ifndef _RANDLCG_H_
#define _RANDLCG_H_ 1.1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_PROTO
long         set_seed();
long         get_seed();
unsigned long int randlcg();

#else
long         set_seed(long);
long         get_seed(long);
unsigned long int randlcg();

#endif

#ifdef __cplusplus
}
#endif

#endif
