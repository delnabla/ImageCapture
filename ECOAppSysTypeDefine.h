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
 * 修饰符定义(需要根据平台特点具体定义)
 */

#define CONST   const
#define STATIC  static
#define INLINE  STATIC __inline
#define EXTERN  extern


/*
*  数据类型(需要根据平台特点具体定义)
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
* 数据范围
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
*  其他基本操作
*/

/* 最大指针对齐值，指针以PTR_GRID对齐 (须根据平台定制) */
#define PTR_GRID        4


/* 对齐指针/大小 */
#define GRID_PTR(p)     ((void*)(((Address)(p)+(PTR_GRID-1))/PTR_GRID*PTR_GRID))
#define GRID_SIZE(n)    ((Size)(((Size)(n)+(PTR_GRID-1))/PTR_GRID*PTR_GRID))

/* 判断指针是否有效 */
#define IS_PTR_VALID(p) (NULL!=(p))

/* 指针类型转换，主要用于低类型指针向高类型指针转换 */
#define POINTER_CONVERT(t,p)    ((t)(p))

/* 求数组元素个数 */
#define ARRAY_SIZE(ar)  (sizeof(ar)/sizeof(*(ar)))


/* 求相对大值、相对小值 */
#define MAX(a,b)        (((a)>(b))?(a):(b))
#define MIN(a,b)        (((a)<(b))?(a):(b))


/* 结构成员的一些常用操作 */
#define MEMBER_OFFSET(s,m)   ((Size)&(((s *)0)->m))
#define SIZEOF_MEMBER(s,m)   (sizeof(((s *)0)->m))


/* 基本的内存操作 */
#define ECOAppSysMalloc(A)                    (malloc(A))
#define ECOAppSysFree(A)                       (free(A))
#define ECOAppSysMemZero(p,n)            ((void)memset((p),0,(Size)(n)))
#define ECOAppSysMemCopy(d,s,n)         ((void)memcpy((d),(s),(Size)(n)))
#define ECOAppSysMemMove(d,s,n)	   ((void)memmove((d),(s),(Size)(n)))
#define ECOAppSysMemComp(d,s,n)	   ((Cmp)memcmp((d),(s),(Size)(n)))


/* 获取bit域的数据 */
#define GET_BITFIELD(field, mask, shift)    (((field) & (mask)) >> (shift))

/* 设置bit域的数据 */
#define SET_BITFIELD(field, val, mask, shift) \
{ \
(field) &= ~(mask); \
(field) |= (((val) << (shift)) & (mask)); \
}

#ifdef __cplusplus
}
#endif

#endif

