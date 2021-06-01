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

// system
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// libeep
#include <cnt/seg.h>
/*
 *
 */
libeep_seg_t * libeep_seg_read(const char * filename) {
  libeep_seg_t * rv;
  FILE         * f;
  char           line_buffer[1024];
  uint32_t       segment_index;           

  rv = NULL;
  segment_index = 0;
  f=fopen(filename, "rb");
  if(f != NULL) {
    rv = (libeep_seg_t *)malloc(sizeof(libeep_seg_t));
    rv->count=0;
    rv->array=NULL;
    while(!feof(f)) {
      if(fgets(line_buffer, 1024, f)) {
        if(sscanf(line_buffer, "NumberSegments=%i", & rv->count)) {
          --rv->count;
          assert(rv->array == NULL);
          rv->array=(libeep_seg_info_t *)malloc(sizeof(libeep_seg_info_t) * rv->count);
          memset(rv->array, 0, sizeof(libeep_seg_info_t) * rv->count);
        }
        if(sscanf(line_buffer, "%lf %lf %i", & rv->array[segment_index].date, & rv->array[segment_index].fraction, & rv->array[segment_index].sample_count)) {
          ++segment_index;
        }
      }
    }
    fclose(f);
  }
  return rv;
}
/*
 *
 */
void libeep_seg_delete(libeep_seg_t * s) {
  if(s != NULL) {
    if(s->count > 0 && s->array != NULL) {
      free(s->array);
    }
    free(s);
  }
}
