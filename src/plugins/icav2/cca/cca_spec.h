//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  cca_spec.h
//
//  Code generation for function 'cca'
//


#ifndef CCA_SPEC_H
#define CCA_SPEC_H

// Include files
#ifdef CCA_XIL_BUILD
#if defined(_MSC_VER) || defined(__LCC__)
#define CCA_DLL_EXPORT                 __declspec(dllimport)
#else
#define CCA_DLL_EXPORT                 __attribute__ ((visibility("default")))
#endif

#elif defined(BUILDING_CCA)
#if defined(_MSC_VER) || defined(__LCC__)
#define CCA_DLL_EXPORT                 __declspec(dllexport)
#else
#define CCA_DLL_EXPORT                 __attribute__ ((visibility("default")))
#endif

#else
#define CCA_DLL_EXPORT
#endif
#endif

// End of code generation (cca_spec.h)
