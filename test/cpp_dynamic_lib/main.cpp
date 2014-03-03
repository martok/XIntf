//---------------------------------------------------------------------------

#include "../../c++/xintf.h"
#include <windows>

#pragma hdrstop

#define DllExport extern "C" __declspec(dllexport)

DECLARE_GUID(IAdder, MAKEGUID(39000AE7,F922,4041,AB,6C,AE,20,FB,80,2C,3B));
DECLARE_INTERFACE_(IAdder, IUnknown)
{
  STDMETHOD_(DWORD,Add)(DWORD A, DWORD B) PURE;
};
DECLARE_GUID(IMultiplier, MAKEGUID(39000AE7,F922,4041,AB,6C,AE,20,FB,80,2C,3C));
DECLARE_INTERFACE_(IMultiplier, IUnknown)
{
  STDMETHOD_(DWORD,Times)(DWORD A, DWORD B) PURE;
};


class CTestClass: public CInterfacedObject, public IAdder, public IMultiplier
{
public:
   // IUnknown
   STDMETHODIMPL_(ULONG,AddRef)(void) {return CInterfacedObject::AddRef();};
   STDMETHODIMPL_(ULONG,Release)(void) {return CInterfacedObject::Release();};
   STDMETHODIMPL(QueryInterface)(const _GUID& IID, void** Obj) {
	 QUERY_INTERFACE(IAdder)
	 QUERY_INTERFACE(IMultiplier)
	 QUERY_INTERFACE_END
   };

   // ITestInterface
   STDMETHODIMPL_(DWORD,Add)(DWORD A, DWORD B)
   {
	  return A + B;
   }

   // IMultiplier
   STDMETHODIMPL_(DWORD,Times)(DWORD A, DWORD B)
   {
	  return A * B;
   }
};



DllExport long __stdcall GetInstance(IAdder** inst);


long __stdcall GetInstance(IAdder** inst)
{
   SIAdder instance = new CTestClass;
   *inst = (IAdder*)instance;
   instance->AddRef();
   return 0;
}


#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
