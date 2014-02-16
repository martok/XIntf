#ifndef wfundH
#define wfundH
/*******************************************************************************
Windows fundamental types
*******************************************************************************/

// INTEGER
#include <ctype.h>
typedef __uint32_t DWORD;
typedef __uint16_t WORD;
typedef __uint8_t BYTE;

typedef DWORD ULONG;
typedef DWORD HRESULT;
typedef bool BOOL;

// INTERFACE
typedef struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID,*REFGUID,*LPGUID;
inline BOOL IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
  return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}
inline BOOL operator==(const GUID& guidOne, const GUID& guidOther)
{
  return !memcmp(&guidOne,&guidOther,sizeof(GUID));
}
inline BOOL operator!=(const GUID& g1, const GUID& g2)
{
  return !(g1 == g2);
}

// ERROR CODES
#define NO_ERROR       0
#define S_OK           0
#define E_NOINTERFACE  0x80004002

#endif