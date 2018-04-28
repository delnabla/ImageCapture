/******************************************************************************
 * Copyright (C)
 * File name:       ECOAppSysTypeDefine.h
 * Version:         
 * Description:    base type define in ECOAppSys Team
 * Other:
 * History:
 ******************************************************************************/

#ifndef _ECOAPPSYS_TYPE_DEFINE_H_
#define _ECOAPPSYS_TYPE_DEFINE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


/** platform define */
#ifdef WIN32
#define ECO_WIN32
#endif

	////#define ECO_WIN32


/*
 * ���η�����(��Ҫ����ƽ̨�ص���嶨��)
 */

#define CONST   const
#define STATIC  static
#define INLINE  STATIC __inline
#define EXTERN  extern


/*
*  ��������(��Ҫ����ƽ̨�ص���嶨��)
*/
typedef char               S8;              /*8-bit*/
typedef short              S16;            /*16-bit*/
typedef int                S32;            /*32-bit*/

typedef unsigned char         U8;              /*8-bit*/
typedef unsigned short        U16;            /*16-bit*/
typedef unsigned int          U32;            /*32-bit*/

typedef float                 FLOAT;      

typedef struct
{
    U8 *ptr;
    S32 length;
    S32 fill;
} ECOAppSysBD;

#define MX_SUCCESS          (1)
#define MX_FAIL                 (0)

/*
* ���ݷ�Χ
*/
#define INT8_MAX        (+127)
#define INT16_MAX       (+32767)
#define INT32_MAX       (+2147483647L)

#define INT8_MIN        (-INT8_MAX - 1)
#define INT16_MIN       (-INT16_MAX - 1)
#define INT32_MIN       (-INT32_MAX - 1)

#define UINT8_MAX       (0xffU)
#define UINT16_MAX      (0xffffU)
#define UINT32_MAX      (0xffffffffUL)


/*
*  ������������
*/

/* ���ָ�����ֵ��ָ����PTR_GRID���� (�����ƽ̨����) */
#define PTR_GRID        4


/* ����ָ��/��С */
#define GRID_PTR(p)     ((void*)(((Address)(p)+(PTR_GRID-1))/PTR_GRID*PTR_GRID))
#define GRID_SIZE(n)    ((Size)(((Size)(n)+(PTR_GRID-1))/PTR_GRID*PTR_GRID))

/* �ж�ָ���Ƿ���Ч */
#define IS_PTR_VALID(p) (NULL!=(p))

/* ָ������ת������Ҫ���ڵ�����ָ���������ָ��ת�� */
#define POINTER_CONVERT(t,p)    ((t)(p))

/* ������Ԫ�ظ��� */
#define ARRAY_SIZE(ar)  (sizeof(ar)/sizeof(*(ar)))


/* ����Դ�ֵ�����Сֵ */
#define MAX(a,b)        (((a)>(b))?(a):(b))
#define MIN(a,b)        (((a)<(b))?(a):(b))


/* �ṹ��Ա��һЩ���ò��� */
#define MEMBER_OFFSET(s,m)   ((Size)&(((s *)0)->m))
#define SIZEOF_MEMBER(s,m)   (sizeof(((s *)0)->m))


/* �������ڴ���� */
#define ECOAppSysMalloc(A)                    (malloc(A))
#define ECOAppSysFree(A)                       (free(A))
#define ECOAppSysMemZero(p,n)            ((void)memset((p),0,(Size)(n)))
#define ECOAppSysMemCopy(d,s,n)         ((void)memcpy((d),(s),(Size)(n)))
#define ECOAppSysMemMove(d,s,n)	   ((void)memmove((d),(s),(Size)(n)))
#define ECOAppSysMemComp(d,s,n)	   ((Cmp)memcmp((d),(s),(Size)(n)))


/* ��ȡbit������� */
#define GET_BITFIELD(field, mask, shift)    (((field) & (mask)) >> (shift))

/* ����bit������� */
#define SET_BITFIELD(field, val, mask, shift) \
{ \
(field) &= ~(mask); \
(field) |= (((val) << (shift)) & (mask)); \
}

#ifdef __cplusplus
}
#endif

#endif

