#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------- */
/* Configuration                                                             */
/* ------------------------------------------------------------------------- */

#ifndef DYNARR_INITIAL_CAPACITY
#define DYNARR_INITIAL_CAPACITY 8
#endif

#ifndef DYNARR_GROWTH_FACTOR
#define DYNARR_GROWTH_FACTOR 2
#endif

/* ------------------------------------------------------------------------- */
/* DEFINE_DYNAMIC_ARRAY_FUNCS                                                */
/*                                                                           */
/* Generates a complete set of functions for a dynamic array of `ElemType`   */
/* whose container struct is `ArrType` with the conventional fields:         */
/*     ElemType *data;                                                       */
/*     size_t    count;                                                      */
/*     size_t    capacity;                                                   */
/*                                                                           */
/* `prefix` is used to namespace the generated functions, e.g.               */
/*     DEFINE_DYNAMIC_ARRAY_FUNCS(PointArray, Point, point_array)            */
/* yields:                                                                   */
/*     point_array_init, point_array_free, point_array_reserve,              */
/*     point_array_push, point_array_pop, point_array_at,                    */
/*     point_array_clear, point_array_shrink_to_fit                          */
/*                                                                           */
/* An optional element-destructor `elem_free_fn` (or NULL) is invoked on     */
/* every element that is being released (pop, clear, free). This lets you    */
/* compose nested arrays cleanly (e.g. a Lanelet owns two PointArrays).      */
/* ------------------------------------------------------------------------- */

#define DECLARE_DYNAMIC_ARRAY_FUNCS(ArrType, ElemType, prefix)                                     \
    void prefix##_init(ArrType* arr);                                                              \
    void prefix##_free(ArrType* arr);                                                              \
    bool prefix##_reserve(ArrType* arr, size_t new_capacity);                                      \
    bool prefix##_push(ArrType* arr, ElemType value);                                              \
    bool prefix##_pop(ArrType* arr, ElemType* out);                                                \
    ElemType* prefix##_at(ArrType* arr, size_t index);                                             \
    void prefix##_clear(ArrType* arr);                                                             \
    bool prefix##_shrink_to_fit(ArrType* arr);

#define DEFINE_DYNAMIC_ARRAY_FUNCS(ArrType, ElemType, prefix, elem_free_fn)                        \
                                                                                                   \
    /* Initialise to a known-empty state. Always safe to call on a fresh                           \
     * (possibly garbage-initialised) struct before first use. */                                  \
    void prefix##_init(ArrType* arr) {                                                             \
        assert(arr != NULL);                                                                       \
        arr->data = NULL;                                                                          \
        arr->count = 0;                                                                            \
        arr->capacity = 0;                                                                         \
    }                                                                                              \
                                                                                                   \
    /* Release every element via the optional destructor, free the buffer,                         \
     * and reset the struct so it is safe to reuse or free again. */                               \
    void prefix##_free(ArrType* arr) {                                                             \
        if (arr == NULL)                                                                           \
            return;                                                                                \
        void (*free_fn)(ElemType*) = (elem_free_fn);                                               \
        if (free_fn != NULL) {                                                                     \
            for (size_t i = 0; i < arr->count; ++i) {                                              \
                free_fn(&arr->data[i]);                                                            \
            }                                                                                      \
        }                                                                                          \
        free(arr->data);                                                                           \
        arr->data = NULL;                                                                          \
        arr->count = 0;                                                                            \
        arr->capacity = 0;                                                                         \
    }                                                                                              \
                                                                                                   \
    /* Ensure capacity >= new_capacity. Returns false on allocation failure                        \
     * and leaves the array unchanged. Detects size_t overflow. */                                 \
    bool prefix##_reserve(ArrType* arr, size_t new_capacity) {                                     \
        assert(arr != NULL);                                                                       \
        if (new_capacity <= arr->capacity)                                                         \
            return true;                                                                           \
        if (new_capacity > SIZE_MAX / sizeof(ElemType))                                            \
            return false;                                                                          \
        ElemType* new_data = (ElemType*)realloc(arr->data, new_capacity * sizeof(ElemType));       \
        if (new_data == NULL)                                                                      \
            return false;                                                                          \
        arr->data = new_data;                                                                      \
        arr->capacity = new_capacity;                                                              \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* Append a copy of `value`. Grows geometrically. Returns false on                             \
     * allocation failure (array is left unchanged in that case). */                               \
    bool prefix##_push(ArrType* arr, ElemType value) {                                             \
        assert(arr != NULL);                                                                       \
        if (arr->count == arr->capacity) {                                                         \
            size_t new_cap = (arr->capacity == 0) ? DYNARR_INITIAL_CAPACITY                        \
                                                  : arr->capacity * DYNARR_GROWTH_FACTOR;          \
            /* Guard against overflow in the growth multiplication. */                             \
            if (new_cap < arr->capacity)                                                           \
                return false;                                                                      \
            if (!prefix##_reserve(arr, new_cap))                                                   \
                return false;                                                                      \
        }                                                                                          \
        arr->data[arr->count++] = value;                                                           \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* Remove the last element. If `out` is non-NULL the value is moved                            \
     * into *out and the destructor is NOT invoked (caller takes ownership).                       \
     * If `out` is NULL the destructor (if any) is invoked. */                                     \
    bool prefix##_pop(ArrType* arr, ElemType* out) {                                               \
        assert(arr != NULL);                                                                       \
        if (arr->count == 0)                                                                       \
            return false;                                                                          \
        --arr->count;                                                                              \
        if (out != NULL) {                                                                         \
            *out = arr->data[arr->count];                                                          \
        } else {                                                                                   \
            void (*free_fn)(ElemType*) = (elem_free_fn);                                           \
            if (free_fn != NULL)                                                                   \
                free_fn(&arr->data[arr->count]);                                                   \
        }                                                                                          \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* Bounds-checked element access. Returns NULL on out-of-range. */                             \
    ElemType* prefix##_at(ArrType* arr, size_t index) {                                            \
        assert(arr != NULL);                                                                       \
        if (index >= arr->count)                                                                   \
            return NULL;                                                                           \
        return &arr->data[index];                                                                  \
    }                                                                                              \
                                                                                                   \
    /* Destruct all elements but keep the allocated buffer. */                                     \
    void prefix##_clear(ArrType* arr) {                                                            \
        assert(arr != NULL);                                                                       \
        void (*free_fn)(ElemType*) = (elem_free_fn);                                               \
        if (free_fn != NULL) {                                                                     \
            for (size_t i = 0; i < arr->count; ++i) {                                              \
                free_fn(&arr->data[i]);                                                            \
            }                                                                                      \
        }                                                                                          \
        arr->count = 0;                                                                            \
    }                                                                                              \
                                                                                                   \
    /* Shrink capacity to count. Returns false on allocation failure                               \
     * (array is left intact in that case). */                                                     \
    bool prefix##_shrink_to_fit(ArrType* arr) {                                                    \
        assert(arr != NULL);                                                                       \
        if (arr->capacity == arr->count)                                                           \
            return true;                                                                           \
        if (arr->count == 0) {                                                                     \
            free(arr->data);                                                                       \
            arr->data = NULL;                                                                      \
            arr->capacity = 0;                                                                     \
            return true;                                                                           \
        }                                                                                          \
        ElemType* new_data = (ElemType*)realloc(arr->data, arr->count * sizeof(ElemType));         \
        if (new_data == NULL)                                                                      \
            return false;                                                                          \
        arr->data = new_data;                                                                      \
        arr->capacity = arr->count;                                                                \
        return true;                                                                               \
    }                                                                                              \
    /* Force a trailing semicolon at the call site for readability. */                             \
    typedef int prefix##_force_semicolon_##__LINE__

#endif /* DYNAMIC_ARRAY_H */