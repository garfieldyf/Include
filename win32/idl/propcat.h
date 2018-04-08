/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 7.00.0500 */
/* Compiler settings for propcat.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/

#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif // __REQUIRED_RPCNDR_H_VERSION__

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
    #error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __propcat_h__
#define __propcat_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICategorizeProperties_FWD_DEFINED__
#define __ICategorizeProperties_FWD_DEFINED__
typedef interface ICategorizeProperties ICategorizeProperties;
#endif  /* __ICategorizeProperties_FWD_DEFINED__ */

/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifndef __ICategorizeProperties_INTERFACE_DEFINED__
#define __ICategorizeProperties_INTERFACE_DEFINED__

/* interface ICategorizeProperties */
/* [unique][uuid][local][object] */ 

typedef /* [public] */ int PROPCAT;

#define PROPCAT_Nil         (-1)

#define PROPCAT_Misc        (-2)

#define PROPCAT_Font        (-3)

#define PROPCAT_Position    (-4)

#define PROPCAT_Appearance  (-5)

#define PROPCAT_Behavior    (-6)

#define PROPCAT_Data        (-7)

#define PROPCAT_List        (-8)

#define PROPCAT_Text        (-9)

#define PROPCAT_Scale       (-10)

#define PROPCAT_DDE         (-11)


EXTERN_C const IID IID_ICategorizeProperties;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4D07FC10-F931-11CE-B001-00AA006884E5")
    ICategorizeProperties : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MapPropertyToCategory( 
            /* [in] */ DISPID dispid,
            /* [out] */ PROPCAT* pPropcat) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetCategoryName( 
            /* [in] */ PROPCAT propcat,
            /* [in] */ LCID lcid,
            /* [out] */ BSTR* pbstrName) = 0;
    };

#else   /* C style interface */

    typedef struct ICategorizePropertiesVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICategorizeProperties * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);

        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICategorizeProperties * This);

        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICategorizeProperties * This);

        HRESULT ( STDMETHODCALLTYPE *MapPropertyToCategory )( 
            ICategorizeProperties * This,
            /* [in] */ DISPID dispid,
            /* [out] */ PROPCAT *pPropcat);

        HRESULT ( STDMETHODCALLTYPE *GetCategoryName )( 
            ICategorizeProperties * This,
            /* [in] */ PROPCAT propcat,
            /* [in] */ LCID lcid,
            /* [out] */ BSTR *pbstrName);

        END_INTERFACE
    } ICategorizePropertiesVtbl;

    interface ICategorizeProperties
    {
        CONST_VTBL struct ICategorizePropertiesVtbl *lpVtbl;
    };

#ifdef COBJMACROS

#define ICategorizeProperties_QueryInterface(This,riid,ppvObject) \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICategorizeProperties_AddRef(This) \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICategorizeProperties_Release(This) \
    ( (This)->lpVtbl -> Release(This) ) 

#define ICategorizeProperties_MapPropertyToCategory(This,dispid,pPropcat) \
    ( (This)->lpVtbl -> MapPropertyToCategory(This,dispid,pPropcat) ) 

#define ICategorizeProperties_GetCategoryName(This,propcat,lcid,pbstrName) \
    ( (This)->lpVtbl -> GetCategoryName(This,propcat,lcid,pbstrName) ) 

#endif /* COBJMACROS */

#endif  /* C style interface */

#endif  /* __ICategorizeProperties_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif