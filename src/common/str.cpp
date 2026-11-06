// SPDX-FileCopyrightText: 2023 Open Salamander Authors
// SPDX-License-Identifier: GPL-2.0-or-later

#include "precomp.h"

#include <windows.h>
#include <crtdbg.h>
#include <ostream>
#include <commctrl.h> // need LPCOLORMAP

#if defined(_DEBUG) && defined(_MSC_VER) // without passing file+line to 'new' operator, list of memory leaks shows only 'crtdbg.h(552)'
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#ifndef STR_DISABLE

#ifdef INSIDE_SPL // for use in plugins
#include "spl_base.h"
#include "dbg.h"
#else                     //INSIDE_SPL
#pragma warning(3 : 4706) // warning C4706: assignment within conditional expression
#include "trace.h"
#include "messages.h"
#include "handles.h"
#endif //INSIDE_SPL

#include "str.h"

#ifndef INSIDE_SPL // except for use in plugins

// The order here is important.
// Section names must be 8 characters or less.
// The sections with the same name before the $
// are merged into one section. The order that
// they are merged is determined by sorting
// the characters after the $.
// i_str and i_str_end are used to set
// boundaries so we can find the real functions
// that we need to call for initialization.

#pragma warning(disable : 4075) // define the module initialization order

typedef void(__cdecl* _PVFV)(void);

#pragma section(".i_str$a", read)
__declspec(allocate(".i_str$a")) const _PVFV i_str = (_PVFV)1; // place the i_str variable at the start of the .i_str section

#pragma section(".i_str$z", read)
__declspec(allocate(".i_str$z")) const _PVFV i_str_end = (_PVFV)1; // and place the i_str_end variable at the end of the .i_str section

void Initialize__Str()
{
    const _PVFV* x = &i_str;
    for (++x; x < &i_str_end; ++x)
        if (*x != NULL)
            (*x)();
}

#pragma init_seg(".i_str$m")

#endif //INSIDE_SPL

BYTE LowerCase[256];
BYTE UpperCase[256];

void InitializeCase();

class C__STR_module // automatic module initialization
{
public:
    C__STR_module() { InitializeCase(); }
} __STR_module;

// ****************************************************************************

char* DupStr(const char* txt)
{
    if (txt == NULL)
        return NULL;
    int l = (int)strlen(txt);
    char* s = (char*)malloc(l + 1);
    if (s == NULL)
    {
        TRACE_E("Low memory.");
        return NULL;
    }
    memcpy(s, txt, l + 1);
    return s;
}

char* DupStrEx(const char* str, BOOL& err)
{
    char* s = DupStr(str);
    if (str != NULL && s == NULL)
        err = TRUE;
    return s;
}

// ****************************************************************************

char* StrNCat(char* dst, const char* src, int dstSize)
{
    int i = lstrlenA(dst);
    lstrcpynA(dst + i, src, dstSize - i);
    return dst;
}

// ****************************************************************************

void InitializeCase()
{
    int i;
    for (i = 0; i < 256; i++)
        LowerCase[i] = (char)(UINT_PTR)CharLowerA((LPSTR)(UINT_PTR)i);
    for (i = 0; i < 256; i++)
        UpperCase[i] = (char)(UINT_PTR)CharUpperA((LPSTR)(UINT_PTR)i);
}

//
//*****************************************************************************

int StrICpy(char* dest, const char* src)
{
    const char* s = src;
    while (*src != 0)
        *dest++ = LowerCase[*src++];
    *dest = 0;
    return (int)(src - s); // return the number of copied characters
}

//
//*****************************************************************************

// x64 has no inline assembler, so the string operations are implemented in C++.

// original function
int StrICmp(const char* s1, const char* s2)
{
    int res;
    while (1)
    {
        res = (unsigned)LowerCase[*s1] - (unsigned)LowerCase[*s2++];
        if (res != 0)
            return (res < 0) ? -1 : 1; // < or >
        if (*s1++ == 0)
            return 0; // ==
    }
}

//
//*****************************************************************************

/*
// original function
// warning: StrNICmp("a", "aa", 2) incorrectly returns 0
int StrNICmp(const char *s1, const char *s2, int n)
{
  int res;
  while (n--)
  {
    res = (unsigned)LowerCase[*s1++] - (unsigned)LowerCase[*s2++];
    if (res != 0) return (res < 0) ? -1 : 1;               // < a >
    if (*s1 == 0) return 0;                                // ==
  }
  return 0;
}
*/

// fixed version
int StrNICmp(const char* s1, const char* s2, int n)
{
    int res;
    while (n--)
    {
        res = (unsigned)LowerCase[*s1] - (unsigned)LowerCase[*s2++];
        if (res != 0)
            return (res < 0) ? -1 : 1; // < a >
        if (*s1++ == 0)
            return 0; // ==
    }
    return 0;
}

//
//*****************************************************************************

int MemICmp(const void* buf1, const void* buf2, int n)
{
    int ret = _memicmp(buf1, buf2, n);
    // normalize the return value to match our specification
    if (ret == 0)
        return 0;
    return (ret < 0) ? -1 : 1;
}

//
//*****************************************************************************

// original function
int StrICmpEx(const char* s1, int l1, const char* s2, int l2)
{
    int res, l = (l1 < l2) ? l1 : l2;
    while (l--)
    {
        res = (unsigned)LowerCase[*s1++] - (unsigned)LowerCase[*s2++];
        if (res != 0)
            return (res < 0) ? -1 : 1; // < or >
    }
    if (l1 != l2)
        return (l1 < l2) ? -1 : 1; // < or >
    else
        return 0;
}

/*
// logic of the assembly function
int StrICmpEx(const char *s1, int l1, const char *s2, int l2)
{
  int l = (l1 < l2) ? l1 : l2;

  if (l > 0)
  {
    int res = MemICmp(s1, s2, l);
    if (res != 0) return (res < 0) ? -1 : 1;    // < a >
  }

  if (l1 != l2) return (l1 < l2) ? -1 : 1;
  else return 0;
}
*/

//
//*****************************************************************************

/*
// original function
int StrCmpEx(const char *s1, int l1, const char *s2, int l2)
{
  int res, l = (l1 < l2) ? l1 : l2;
  while (l--)
  {
    res = (unsigned)(*s1++) - (unsigned)(*s2++);
    if (res != 0) return (res < 0) ? -1 : 1;    // < a >
  }
  if (l1 != l2) return (l1 < l2) ? -1 : 1;    // < a >
  else return 0;
}
*/

// faster variant
int StrCmpEx(const char* s1, int l1, const char* s2, int l2)
{
    int l = (l1 < l2) ? l1 : l2;

    if (l > 0)
    {
        int res = memcmp(s1, s2, l);
        if (res != 0)
            return (res < 0) ? -1 : 1; // < a >
    }

    if (l1 != l2)
        return (l1 < l2) ? -1 : 1; // < a >
    else
        return 0;
}

//
//*****************************************************************************

const char* StrIStr(const char* txt, const char* pattern)
{
    if (txt == NULL || pattern == NULL)
        return NULL;

    const char* s = txt;
    int len = (int)strlen(pattern);
    int txtLen = (int)strlen(txt);
    while (txtLen >= len)
    {
        if (StrNICmp(s, pattern, len) == 0)
            return s;
        s++;
        txtLen--;
    }
    return NULL;
}

const char* StrIStr(const char* txtStart, const char* txtEnd,
                    const char* patternStart, const char* patternEnd)
{
    if (txtStart == NULL || patternStart == NULL)
        return NULL;

    const char* s = txtStart;
    int len = (int)(patternEnd - patternStart);
    int txtLen = (int)(txtEnd - txtStart);
    while (txtLen >= len)
    {
        if (StrNICmp(s, patternStart, len) == 0)
            return s;
        s++;
        txtLen--;
    }
    return NULL;
}

//
//*****************************************************************************

/*
int StrLen(const char *str)
{
  const char *s = str;
  while (1)
  {
    if ((((*(DWORD *)s) & 0xF0F0F0F0) - 0x10101010) & 0x8F0F0F0F)
    {                            // contains a character < 16
      if (*s != 0) s++;
      else break;
      if (*s != 0) s++;
      else break;
      if (*s != 0) s++;
      else break;
      if (*s != 0) s++;
      else break;
    }
    else s += 4;
  }
  return s - str;
}
*/

/*
//*****************************************************************************
//
// Tables for converting Kamenicky encoding to MS Windows
//

BYTE KodKamenickych[CONVERT_TAB_CHARS] =
{
0x84,0x8e,
0xa0,0x8f,
0x87,0x80,
0x83,0x85,
0x82,0x90,
0x88,0x89,
0xa1,0x8b,
0x8d,0x8a,
0x8c,0x9c,
0xa4,0xa5,
0xa2,0x95,
0x94,0x99,
0x93,0xa7,
0xaa,0xab,
0xa9,0x9e,
0xa8,0x9b,
0x9f,0x86,
0xa3,0x97,
0x96,0xa6,
0x81,0x9a,
0x98,0x9d,
0x91,0x92,
}; //done

BYTE KodWindows[CONVERT_TAB_CHARS] =
{
0xe4,0xc4,
0xe1,0xc1,
0xe8,0xc8,
0xef,0xcf,
0xe9,0xc9,
0xec,0xcc,
0xed,0xcd,
0xe5,0xc5,
0xbe,0xbc,
0xf2,0xd2,
0xf3,0xd3,
0xf6,0xd6,
0xf4,0xd4,
0xe0,0xc0,
0xf8,0xd8,
0x9a,0x8a,
0x9d,0x8d,
0xfa,0xda,
0xf9,0xd9,
0xfc,0xdc,
0xfd,0xdd,
0x9e,0x8e
}; //done

CConvertTab::CConvertTab()
{
  WORD i;
  for(i = 0; i < CONVERT_TAB_MAX_CHARS; i++)
    Data[i] = (BYTE)i;
  for(i = 0; i < CONVERT_TAB_CHARS; i++)
    Data[KodKamenickych[i]] = KodWindows[i];
}

void
CConvertTab::Convert(char *str)
{
  char *ptr = str;
  while(*ptr != 0) *ptr++ = Data[*ptr];
}

CConvertTab ConvertTab;
*/

#endif // STR_DISABLE
