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
#ifndef __libeep_cnt_seg_h__
#define __libeep_cnt_seg_h__

#include <stdint.h>

/**
  *
  */
struct libeep_seg_info {
  double   date;
  double   fraction;
  uint32_t sample_count;
};
typedef struct libeep_seg_info libeep_seg_info_t;
/**
  *
  */
struct libeep_seg {
  int                 count;
  libeep_seg_info_t * array;
};
typedef struct libeep_seg libeep_seg_t;
/**
 *
 */
libeep_seg_t * libeep_seg_read(const char *);
void           libeep_seg_delete(libeep_seg_t *);

#endif
