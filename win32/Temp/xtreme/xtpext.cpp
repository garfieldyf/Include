///////////////////////////////////////////////////////////////////////////////
// xtpext.cpp : ʵ���ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/10/24

#include "stdafx.h"
#include "xtpext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace XTP {

///////////////////////////////////////////////////////////////////////////////
// CInplaceSpinButton ���ʵ��
//

BEGIN_MESSAGE_MAP(CXTPPropertyGridItemSpin::CInplaceSpinButton, CSpinButtonCtrl)
    ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDelTapos)
END_MESSAGE_MAP()

}  // namespace XTP