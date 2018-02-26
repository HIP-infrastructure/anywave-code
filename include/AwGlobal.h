/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_GLOBAL_H
#define AW_GLOBAL_H

#include <QtCore/QtGlobal>
// for now, shared libs will only be built on Windows

 // CORE
 #ifdef AW_BUILD_CORE_LIB
 #define AW_CORE_EXPORT Q_DECL_EXPORT
 #else
 #define AW_CORE_EXPORT Q_DECL_IMPORT
 #endif

 // EPOCH Lib MACRO
 #ifdef AW_BUILD_EPOCH_LIB
 #define AW_EPOCH_EXPORT Q_DECL_EXPORT
 #else
 #define AW_EPOCH_EXPORT Q_DECL_IMPORT
 #endif

 // FILTER
 #ifdef AW_BUILD_FILTER_LIB
 #define AW_FILTER_EXPORT Q_DECL_EXPORT
 #else
 #define AW_FILTER_EXPORT Q_DECL_IMPORT
 #endif



 // GRAPHICS
 #ifdef AW_BUILD_GRAPHICS_LIB
 #define AW_GRAPHICS_EXPORT Q_DECL_EXPORT
 #else
 #define AW_GRAPHICS_EXPORT Q_DECL_IMPORT
 #endif


 // MATH
 #ifdef AW_BUILD_MATH_LIB
 #define AW_MATH_EXPORT Q_DECL_EXPORT
 #else
 #define AW_MATH_EXPORT Q_DECL_IMPORT
 #endif


 // MAPPING
 #ifdef AW_BUILD_MAPPING_LIB
 #define AW_MAPPING_EXPORT Q_DECL_EXPORT
 #else
 #define AW_MAPPING_EXPORT Q_DECL_IMPORT
 #endif

 // HDF5
 #ifdef AW_BUILD_HDF5_LIB
 #define AW_HDF5_EXPORT Q_DECL_EXPORT
 #else
 #define AW_HDF5_EXPORT Q_DECL_IMPORT
 #endif

 // LAYOUT
 #ifdef AW_BUILD_LAYOUT_LIB
 #define AW_LAYOUT_EXPORT Q_DECL_EXPORT
 #else
 #define AW_LAYOUT_EXPORT Q_DECL_IMPORT
 #endif

 // PROCESS
 #ifdef AW_BUILD_PROCESS_LIB
 #define AW_PROCESS_EXPORT Q_DECL_EXPORT
 #else
 #define AW_PROCESS_EXPORT Q_DECL_IMPORT
 #endif

 // RW
 #ifdef AW_BUILD_RW_LIB
 #define AW_RW_EXPORT Q_DECL_EXPORT
 #else
 #define AW_RW_EXPORT Q_DECL_IMPORT
 #endif


 // UTILITIES
 #ifdef AW_BUILD_UTILITIES_LIB
 #define AW_UTILITIES_EXPORT Q_DECL_EXPORT
 #else
 #define AW_UTILITIES_EXPORT Q_DECL_IMPORT
 #endif


 // WIDGETS
 #ifdef AW_BUILD_WIDGETS_LIB
 #define AW_WIDGETS_EXPORT Q_DECL_EXPORT
 #else
 #define AW_WIDGETS_EXPORT Q_DECL_IMPORT
 #endif

 // MATLAB
 #ifdef AW_BUILD_MATLAB_LIB
 #define AW_MATLAB_EXPORT Q_DECL_EXPORT
 #else
 #define AW_MATLAB_EXPORT Q_DECL_IMPORT
 #endif


#endif