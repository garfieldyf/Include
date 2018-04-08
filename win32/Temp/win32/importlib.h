///////////////////////////////////////////////////////////////////////////////
// importlib.h : Í·ÎÄ¼þ
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/03/28

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IMPORTLIB_H__
#define __IMPORTLIB_H__

// ADO import
#import "C:\Program Files\Common Files\System\ado\MSADO15.DLL" rename_namespace("ADODB") auto_rename

// XML import
#import "MSXML4.DLL" rename_namespace("XML")

// WebBrowser import
#import "MSHTML.TLB" rename_namespace("Html") auto_rename exclude("wireHDC", "_RemotableHandle", "__MIDL_IWinTypes_0009", "tagRECT", "tagPOINT", "wireHWND", "_userHBITMAP", "__MIDL_IWinTypes_0007", "_userBITMAP", "_userHGLOBAL", "__MIDL_IWinTypes_0003", "_FLAGGED_BYTE_BLOB", "tagSIZE", "IEnumUnknown")

// Html import
#import "SHDOCVW.DLL" rename_namespace("Html") auto_rename exclude("OLECMDID", "OLECMDF", "OLECMDEXECOPT", "tagREADYSTATE")

// Windows Media Player import
#import "wmp.dll" rename_namespace("WMP") rename("min", "Min") rename("max", "Max")

// 0ffice 2003 import
#import "C:\Program Files\Common Files\Microsoft Shared\Office11\MSO.DLL" rename_namespace("Office") auto_rename raw_interfaces_only no_implementation
#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" rename_namespace("VBE6") raw_interfaces_only no_implementation
#import "D:\Microsoft Office\OFFICE11\EXCEL.EXE" rename_namespace("Excel") auto_rename exclude("IFont", "IPicture")
#import "D:\Microsoft Office\OFFICE11\MSWORD.OLB" rename_namespace("Word") auto_rename

// Office 2007 import
#import "C:\Program Files\Common Files\Microsoft Shared\Office12\MSO.DLL" rename_namespace("Office") auto_rename raw_interfaces_only no_implementation
#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" rename_namespace("VBE6") raw_interfaces_only no_implementation
#import "D:\Microsoft Office\Office12\EXCEL.EXE" rename_namespace("Excel") auto_rename exclude("IFont", "IPicture")
#import "D:\Microsoft Office\Office12\MSWORD.OLB" rename_namespace("Word") auto_rename

#endif  // __IMPORTLIB_H__