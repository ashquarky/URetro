/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include <malloc.h>
#include <string.h>
#include "dynamic_libs/os_functions.h"
#include "common/common.h"

#define MEMORY_ARENA_1          0
#define MEMORY_ARENA_2          1
#define MEMORY_ARENA_3          2
#define MEMORY_ARENA_4          3
#define MEMORY_ARENA_5          4
#define MEMORY_ARENA_6          5
#define MEMORY_ARENA_7          6
#define MEMORY_ARENA_8          7
#define MEMORY_ARENA_FG_BUCKET  8

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Memory functions
//! This is the only place where those are needed so lets keep them more or less private
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern unsigned int * pMEMAllocFromDefaultHeapEx;
extern unsigned int * pMEMAllocFromDefaultHeap;
extern unsigned int * pMEMFreeToDefaultHeap;

//!-------------------------------------------------------------------------------------------
//! wraps
//!-------------------------------------------------------------------------------------------
void *__wrap_malloc(size_t size)
{
    // pointer to a function resolve
	return ((void * (*)(size_t))(*pMEMAllocFromDefaultHeap))(size);
}

void *__wrap_memalign(size_t align, size_t size)
{
    if (align < 4)
        align = 4;

    // pointer to a function resolve
    return ((void * (*)(size_t, size_t))(*pMEMAllocFromDefaultHeapEx))(size, align);
}

void __wrap_free(void *p)
{
    // pointer to a function resolve
    if(p != 0)
        ((void (*)(void *))(*pMEMFreeToDefaultHeap))(p);
}

void *__wrap_calloc(size_t n, size_t size)
{
    void *p = __wrap_malloc(n * size);
	if (p != 0) {
		memset(p, 0, n * size);
	}
	return p;
}

size_t __wrap_malloc_usable_size(void *p)
{
    //! TODO: this is totally wrong and needs to be addressed
	return 0x7FFFFFFF;
}

void *__wrap_realloc(void *p, size_t size)
{
    void *new_ptr = __wrap_malloc(size);
	if (new_ptr != 0)
	{
		memcpy(new_ptr, p, __wrap_malloc_usable_size(p) < size ? __wrap_malloc_usable_size(p) : size);
		__wrap_free(p);
	}
	return new_ptr;
}

//!-------------------------------------------------------------------------------------------
//! reent versions
//!-------------------------------------------------------------------------------------------
void *__wrap__malloc_r(struct _reent *r, size_t size)
{
	return __wrap_malloc(size);
}

void *__wrap__calloc_r(struct _reent *r, size_t n, size_t size)
{
    return __wrap_calloc(n, size);
}

void *__wrap__memalign_r(struct _reent *r, size_t align, size_t size)
{
    return __wrap_memalign(align, size);
}

void __wrap__free_r(struct _reent *r, void *p)
{
    __wrap_free(p);
}

size_t __wrap__malloc_usable_size_r(struct _reent *r, void *p)
{
    return __wrap_malloc_usable_size(p);
}

void *__wrap__realloc_r(struct _reent *r, void *p, size_t size)
{
    return __wrap_realloc(p, size);
}

//!-------------------------------------------------------------------------------------------
//! some wrappers
//!-------------------------------------------------------------------------------------------
void * MEM2_alloc(unsigned int size, unsigned int align)
{
    return __wrap_memalign(align, size);
}

void MEM2_free(void *ptr)
{
    __wrap_free(ptr);
}