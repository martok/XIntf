#ifndef interfacesH
#define interfacesH
/*******************************************************************************
Cross-Platform COM-style interface support library
*******************************************************************************/

#include <string>

// GUID
#define MAKEGUID(a,b,c,d1,d2,d3,d4,d5,d6,d7,d8)   { 0x##a, 0x##b, 0x##c, { 0x##d1, 0x##d2, 0x##d3, 0x##d4, 0x##d5, 0x##d6, 0x##d7, 0x##d8 } }

// Smart Interfaces
#define DECLARE_GUID(iface,guid) \
	static const GUID IID_##iface = guid;\
	typedef interface iface iface;\
	SMART_INTERFACE(iface)
#define DECLARE_IID(iface,guid)           static const GUID IID_##iface = guid
#define SMART_INTERFACE(i) \
	typedef SmartInterface<i> S##i;\
	template<> const GUID& S##i::uuid = IID_##i;

#ifdef __GNUC__
  // COMPATIBILITY WITH MSVC COM INTERFACE MEMORY LAYOUT
  #ifdef __i386__
	#define STDMETHODCALLTYPE             __attribute__ ((stdcall))
  #else
	#define STDMETHODCALLTYPE
  #endif

  #define interface   					  struct
  
  #define DECLARE_INTERFACE_(iface,ipar)  interface iface: public ipar
  #define DECLARE_INTERFACE(iface)        interface iface
  #define STDMETHOD(method)               STDMETHOD_(HRESULT,method)
  #define STDMETHOD_(return,method)       virtual return STDMETHODCALLTYPE method
  #define PURE 							  = 0
#endif

// SHORTHANDS FOR IMPLEMENTING INTERFACES
#define STDMETHODIMPL(method)             STDMETHODIMPL_(HRESULT,method)
#define STDMETHODIMPL_(return,method)     return STDMETHODCALLTYPE method

#define QUERY_INTERFACE(iid)              if (IID==IID_##iid) { *Obj = (iid*)this; this->AddRef(); } else
#define QUERY_INTERFACE_END               return CInterfacedObject::QueryInterface(IID, Obj); return NO_ERROR;

#define SSupports(intf, iid, var)         (!!intf && (S_OK==intf->QueryInterface(iid, (void**)&var)) && !!var)

#ifdef __GNUC__
  // COMPATIBILITY WITH MSVC/WINDOWS/COM
  #include "wfund.h"

  #define InterlockedIncrement(ptr)         __sync_add_and_fetch(ptr, 1)
  #define InterlockedDecrement(ptr)         __sync_sub_and_fetch(ptr, 1)

  DECLARE_IID(IUnknown, MAKEGUID(00000000,0000,0000,C0,00,00,00,00,00,00,64));
  DECLARE_INTERFACE(IUnknown)
  {
	STDMETHOD_(ULONG,AddRef)(void) PURE;
	STDMETHOD_(ULONG,Release)(void) PURE;
	STDMETHOD(QueryInterface)(const _GUID& IID, void** Obj) PURE;
  };
#else
  #include <windows.h>
#endif


class CInterfacedObject : public IUnknown
{
private:
   long refCount;
public:
   CInterfacedObject(): refCount(0) {};

   STDMETHODIMPL_(ULONG,AddRef)(void) {
	  return InterlockedIncrement(&refCount);
   }
   STDMETHODIMPL_(ULONG,Release)(void) {
      long n = 0;
	  if ((n = InterlockedDecrement(&refCount)) <=0) {
		delete this;
		return 0;
	  }
	  return n;
   }
   STDMETHODIMPL(QueryInterface)(const _GUID& IID, void** Obj) {
      QUERY_INTERFACE(IUnknown)
      return E_NOINTERFACE; return NO_ERROR;
   }
};

template <class T> class SmartInterface
{
private:
  T* intf;
public:    
	static const GUID& uuid;
	// Constructor/Destructor
	SmartInterface<T>() : intf(0)  {} // empty
	SmartInterface<T>(const SmartInterface<T> &rhs) : intf(0) // from smart
	{
		if (rhs.intf != 0)
			rhs.intf->AddRef();
		intf = rhs.intf;
	}
	template <class OTHERT>
	SmartInterface<T>(const SmartInterface<OTHERT> &rhs) : intf(0) // from another smart
	{
		const GUID& guid = SmartInterface<T>::uuid;
		rhs->QueryInterface(guid, (void**)(&intf));   
		if (intf != 0)
			intf->AddRef();
	}
	SmartInterface<T>(T* rhs) // from intf
	{
		if (rhs != 0)
			rhs->AddRef();
		intf = rhs;
	}
	~SmartInterface<T>()
	{
		if (intf != 0)
		{
			intf->Release();
			intf = 0;
		}
	}

	// Assignment
	SmartInterface<T>& operator =(SmartInterface<T> &rhs)  // smart = smart
	{
		if (rhs.intf != 0)
			rhs.intf->AddRef();

		if (intf != 0)
			intf->Release();

		intf = rhs.intf;
		return *this;
	}

	SmartInterface<T>& operator =(T *rhs) // smart = intf
	{
		if (rhs != 0)
			rhs->AddRef();

		if (intf != 0)
			intf->Release();

		intf = rhs;
		return *this;
	}

	// Using

	T* operator->() const
	{
		return intf;
	}

	operator T*() const
	{
		return intf;
	}

	T& operator *()
	{
		return *intf;
	}

	T** operator &()
	{
		return &intf;
	}

	// Comparision
	bool operator ! () const
	{
		return (intf == 0);
	};

	int Release()
	{
		if (intf)
		{
			intf->Release();
			intf = 0;
		}
	}

};

SMART_INTERFACE(IUnknown)

 //---------------------------------------------------------------------------
#endif
