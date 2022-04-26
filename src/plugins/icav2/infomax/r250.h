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

/* r250.h	prototypes for r250 random number generator,

		Kirkpatrick, S., and E. Stoll, 1981; "A Very Fast
		Shift-Register Sequence Random Number Generator",
		Journal of Computational Physics, V.40

		also:

		see W.L. Maier, DDJ May 1991


*/

#ifndef _R250_H_
#define _R250_H_ 1.2

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_PROTO
void         r250_init();
unsigned int r250();
double      dr250();

#else
void         r250_init(int seed);
unsigned int r250( void );
double       dr250( void );
#endif

#ifdef __cplusplus
}
#endif

#endif
