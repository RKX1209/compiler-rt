/*===- InstrProfilingPlatformWindows.c - Profile data on Windows ----------===*\
|*
|* Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
|* See https://llvm.org/LICENSE.txt for license information.
|* SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
|*
\*===----------------------------------------------------------------------===*/

#include "InstrProfiling.h"

#if defined(_WIN32)

#if defined(_MSC_VER)
/* Merge read-write sections into .data. */
#pragma comment(linker, "/MERGE:.lprfc=.data")
#pragma comment(linker, "/MERGE:.lprfd=.data")
#pragma comment(linker, "/MERGE:.lprfv=.data")
#pragma comment(linker, "/MERGE:.lprfnd=.data")
/* Do *NOT* merge .lprfn and .lcovmap into .rdata. llvm-cov must be able to find
 * after the fact.
 */

/* Allocate read-only section bounds. */
#pragma section(".lprfn$A", read)
#pragma section(".lprfn$Z", read)

/* Allocate read-write section bounds. */
#pragma section(".lprfd$A", read, write)
#pragma section(".lprfd$Z", read, write)
#pragma section(".lprfc$A", read, write)
#pragma section(".lprfc$Z", read, write)
#pragma section(".lprfnd$A", read, write)
#pragma section(".lprfnd$Z", read, write)
#endif

__llvm_profile_data COMPILER_RT_SECTION(".lprfd$A") DataStart = {0};
__llvm_profile_data COMPILER_RT_SECTION(".lprfd$Z") DataEnd = {0};

const char COMPILER_RT_SECTION(".lprfn$A") NamesStart = '\0';
const char COMPILER_RT_SECTION(".lprfn$Z") NamesEnd = '\0';

uint64_t COMPILER_RT_SECTION(".lprfc$A") CountersStart;
uint64_t COMPILER_RT_SECTION(".lprfc$Z") CountersEnd;

ValueProfNode COMPILER_RT_SECTION(".lprfnd$A") VNodesStart;
ValueProfNode COMPILER_RT_SECTION(".lprfnd$Z") VNodesEnd;

const __llvm_profile_data *__llvm_profile_begin_data(void) {
  return &DataStart + 1;
}
const __llvm_profile_data *__llvm_profile_end_data(void) { return &DataEnd; }

const char *__llvm_profile_begin_names(void) { return &NamesStart + 1; }
const char *__llvm_profile_end_names(void) { return &NamesEnd; }

uint64_t *__llvm_profile_begin_counters(void) { return &CountersStart + 1; }
uint64_t *__llvm_profile_end_counters(void) { return &CountersEnd; }

ValueProfNode *__llvm_profile_begin_vnodes(void) { return &VNodesStart + 1; }
ValueProfNode *__llvm_profile_end_vnodes(void) { return &VNodesEnd; }

ValueProfNode *CurrentVNode = &VNodesStart + 1;
ValueProfNode *EndVNode = &VNodesEnd;

#endif
