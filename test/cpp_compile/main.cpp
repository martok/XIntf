//---------------------------------------------------------------------------

#include <iostream>
#include "../../c++/xintf.h"

#pragma hdrstop

//---------------------------------------------------------------------------

/* Interface declaration */

									  //39000AE7-F922-4041-AB6C-AE20FB802C3A
DECLARE_GUID(ITestInterface, MAKEGUID(39000AE7,F922,4041,AB,6C,AE,20,FB,80,2C,3A));
DECLARE_INTERFACE_(ITestInterface, IUnknown)
{
  STDMETHOD_(DWORD,Add)(DWORD A, DWORD B) PURE;
};

/* Implement interface and IUnknown via TInterfacedObject */
class CTestClass: public CInterfacedObject, public ITestInterface
{
public:
   // IUnknown
   STDMETHODIMPL_(ULONG,AddRef)(void) {return CInterfacedObject::AddRef();};
   STDMETHODIMPL_(ULONG,Release)(void) {return CInterfacedObject::Release();};
   STDMETHODIMPL(QueryInterface)(const _GUID& IID, void** Obj) {
	 QUERY_INTERFACE(ITestInterface)
	 QUERY_INTERFACE_END
   };

   // ITestInterface
   STDMETHODIMPL_(DWORD,Add)(DWORD A, DWORD B)
   {
	  return A + B;
   }
};

/* Pass in as IUnknown and try getting specific one just to test if that works */
void TestAdder(SIUnknown intf)
{
   SITestInterface iTest = 0;
   if (SSupports(intf, IID_ITestInterface, iTest)) {
	  DWORD s = iTest->Add(42,23);
	  std::cout<< "Sum of 42 + 23 = " << s << "\n";
   }
}

/* Construct an object, store refcounted interface pointer
   Automatically frees adder at function exit (scope end)
*/
void RunTest()
{
   SITestInterface adder = new CTestClass;
   TestAdder((SIUnknown)adder);
}

#pragma argsused
int main(int argc, char* argv[])
{
	RunTest();
	return 0;
}
//---------------------------------------------------------------------------
