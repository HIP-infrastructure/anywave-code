/**
 * @brief common definitions we want to be widely available
 *
 */
/*
 * Original Author: Bevin Brett
 *
 * Copyright © 2021 The General Hospital Corporation (Boston, MA) "MGH"
 *
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the 'FreeSurfer Software License Agreement' contained
 * in the file 'LICENSE' found in the FreeSurfer distribution, and here:
 *
 * https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferSoftwareLicense
 *
 * Reporting: freesurfer@nmr.mgh.harvard.edu
 *
 */

#pragma once

// Some trivial types
//
#ifndef uchar
typedef unsigned char unsigned_char;
#define uchar unsigned_char
#endif

typedef const float * ptr_to_const_float;


// Some trivial math functions needed lots
//
static float squaref(float x) { return x*x; }
static double squared(double x) { return x*x; }

typedef struct FloatXYZ {
    float x,y,z;
} FloatXYZ;

template <typename T, size_t SIZE>
struct FixedSizeArray {
    T&         operator[](size_t i)         { return v[i]; }
    T const&   operator[](size_t i)   const { return v[i]; }
    
    operator T       *()                    { return v;    }
    operator T const *()              const { return v;    }
    
    T       * data()                        { return v;    }
    T const * data()                  const { return v;    }
    
private:
    T v[SIZE];
};

