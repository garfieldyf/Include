///////////////////////////////////////////////////////////////////////////////
// glutilx.h : ͷ�ļ�
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/8/16

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __GLUTILX_H__
#define __GLUTILX_H__

///////////////////////////////////////////////////////////////////////////////
// ���ݿ�ļ���
// ������ж��������� x, y, z ����С�� x, y, z
// ��: 
/*
    typedef std::vector<POINT> CPoints;

    CPoints points;
    const CPoints::const_iterator itor = points.begin();
    POINT ptMax = *itor++, ptMin = *itor++;

    for (; itor != points.end(); ++itor)
    {
        const POINT& point = *itor;

        // ���� x, y, z
        ptMax.x = max(ptMax.x, point.x);
        ptMax.y = max(ptMax.y, point.y);
        ptMax.z = max(ptMax.z, point.z);

        // ��С�� x, y, z
        ptMin.x = min(ptMin.x, point.x);
        ptMin.y = min(ptMin.y, point.y);
        ptMin.z = min(ptMin.z, point.z);
    }
*/

///////////////////////////////////////////////////////////////////////////////
// ��Ƭ�ķ��߼���
// ȡ��Ƭ�����㹹������ v1, ȡ��Ƭ���������㹹������ v2;
// ��� v1 X v2 ( �� v1 �� v2 �Ĳ�� )
// ������淶��
// ��:
/*
typedef std::vector<POINT> CPoints;
typedef std::vector<unsigned int> CFace;

CPoints points;
CFace face;

// �ж��Ƿ�Ϊ��Ƭ
if (face.size() > 2)
{
    unsigned int index1 = face[0];
    unsigned int index2 = face[1];
    unsigned int index3 = face[2];

    double v1[3] = { 0 }, v2[3] = { 0 };

    // ���� v1
    v1[0] = points[index2].x - points[index1].x;
    v1[1] = points[index2].y - points[index1].y;
    v1[2] = points[index2].z - points[index1].z;

    // ���� v2
    v2[0] = points[index3].x - points[index2].x;
    v2[1] = points[index3].y - points[index2].y;
    v2[2] = points[index3].z - points[index2].z;

    // ������
    double normal[3] = { 0 };
    normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    normal[2] = v1[0] * v2[1] - v1[1] * v2[0];

    // �淶��
    double d = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (IsEqual(d, 0.0))
        d = 1.0;

    normal[0] /= d;
    normal[1] /= d;
    normal[2] /= d;
}
*/

///////////////////////////////////////////////////////////////////////////////
// ����ķ��߼���
// ����ʹ���˸ö������Ƭ�ķ���ƽ��ֵ
// ������淶��
// ��:
/*
    std::pair<unsigned int, CNormal*> CNormalPair;
    std::list<CNormalPair> CNormalPairs;

    struct Compare
    {
        bool operator()(const CNormalPair& _Arg1, const CNormalPair& _Arg2) const
        {
            return (_Arg1.first < _Arg2.first);
        }
    };

    // ����
    CNormalPairs.sort(Compare());
*/
#endif  // __GLUTILX_H__