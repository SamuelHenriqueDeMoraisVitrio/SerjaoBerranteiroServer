

#ifndef CJSON_PATH_H
#define CJSON_PATH_H


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef cJSON__h

/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef cJSON__h
#define cJSON__h

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

/* When compiling for windows, we specify a specific calling convention to avoid issues where we are being called from a project with a different default calling convention.  For windows you have 3 define options:

CJSON_HIDE_SYMBOLS - Define this in the case where you don't want to ever dllexport symbols
CJSON_EXPORT_SYMBOLS - Define this on library build when you want to dllexport symbols (default)
CJSON_IMPORT_SYMBOLS - Define this if you want to dllimport symbol

For *nix builds that support visibility attribute, you can define similar behavior by

setting default visibility to hidden by adding
-fvisibility=hidden (for gcc)
or
-xldscope=hidden (for sun cc)
to CFLAGS

then using the CJSON_API_VISIBILITY flag to "export" the same symbols the way CJSON_EXPORT_SYMBOLS does

*/

#define CJSON_CDECL __cdecl
#define CJSON_STDCALL __stdcall

/* export symbols by default, this is necessary for copy pasting the C and header file */
#if !defined(CJSON_HIDE_SYMBOLS) && !defined(CJSON_IMPORT_SYMBOLS) && !defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_EXPORT_SYMBOLS
#endif

#if defined(CJSON_HIDE_SYMBOLS)
#define CJSON_PUBLIC(type)   type CJSON_STDCALL
#elif defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllexport) type CJSON_STDCALL
#elif defined(CJSON_IMPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllimport) type CJSON_STDCALL
#endif
#else /* !__WINDOWS__ */
#define CJSON_CDECL
#define CJSON_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define CJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define CJSON_PUBLIC(type) type
#endif
#endif

/* project version */
#define CJSON_VERSION_MAJOR 1
#define CJSON_VERSION_MINOR 7
#define CJSON_VERSION_PATCH 17

#include <stddef.h>

/* cJSON Types: */
#define cJSON_Invalid (0)
#define cJSON_False  (1 << 0)
#define cJSON_True   (1 << 1)
#define cJSON_NULL   (1 << 2)
#define cJSON_Number (1 << 3)
#define cJSON_String (1 << 4)
#define cJSON_Array  (1 << 5)
#define cJSON_Object (1 << 6)
#define cJSON_Raw    (1 << 7) /* raw json */

#define cJSON_IsReference 256
#define cJSON_StringIsConst 512

/* The cJSON structure: */
typedef struct cJSON
{
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct cJSON *next;
    struct cJSON *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct cJSON *child;

    /* The type of the item, as above. */
    int type;

    /* The item's string, if type==cJSON_String  and type == cJSON_Raw */
    char *valuestring;
    /* writing to valueint is DEPRECATED, use cJSON_SetNumberValue instead */
    int valueint;
    /* The item's number, if type==cJSON_Number */
    double valuedouble;

    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
    char *string;
} cJSON;

typedef struct cJSON_Hooks
{
      /* malloc/free are CDECL on Windows regardless of the default calling convention of the compiler, so ensure the hooks allow passing those functions directly. */
      void *(CJSON_CDECL *malloc_fn)(size_t sz);
      void (CJSON_CDECL *free_fn)(void *ptr);
} cJSON_Hooks;

typedef int cJSON_bool;

/* Limits how deeply nested arrays/objects can be before cJSON rejects to parse them.
 * This is to prevent stack overflows. */
#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 1000
#endif

/* returns the version of cJSON as a string */
CJSON_PUBLIC(const char*) cJSON_Version(void);

/* Supply malloc, realloc and free functions to cJSON */
CJSON_PUBLIC(void) cJSON_InitHooks(cJSON_Hooks* hooks);

/* Memory Management: the caller is always responsible to free the results from all variants of cJSON_Parse (with cJSON_Delete) and cJSON_Print (with stdlib free, cJSON_Hooks.free_fn, or cJSON_free as appropriate). The exception is cJSON_PrintPreallocated, where the caller has full responsibility of the buffer. */
/* Supply a block of JSON, and this returns a cJSON object you can interrogate. */
CJSON_PUBLIC(cJSON *) cJSON_Parse(const char *value);
CJSON_PUBLIC(cJSON *) cJSON_ParseWithLength(const char *value, size_t buffer_length);
/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain a pointer to the error so will match cJSON_GetErrorPtr(). */
CJSON_PUBLIC(cJSON *) cJSON_ParseWithOpts(const char *value, const char **return_parse_end, cJSON_bool require_null_terminated);
CJSON_PUBLIC(cJSON *) cJSON_ParseWithLengthOpts(const char *value, size_t buffer_length, const char **return_parse_end, cJSON_bool require_null_terminated);

/* Render a cJSON entity to text for transfer/storage. */
CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item);
/* Render a cJSON entity to text for transfer/storage without any formatting. */
CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item);
/* Render a cJSON entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON *item, int prebuffer, cJSON_bool fmt);
/* Render a cJSON entity to text using a buffer already allocated in memory with given length. Returns 1 on success and 0 on failure. */
/* NOTE: cJSON is not always 100% accurate in estimating how much memory it will use, so to be safe allocate 5 bytes more than you actually need */
CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buffer, const int length, const cJSON_bool format);
/* Delete a cJSON entity and all subentities. */
CJSON_PUBLIC(void) cJSON_Delete(cJSON *item);

/* Returns the number of items in an array (or object). */
CJSON_PUBLIC(int) cJSON_GetArraySize(const cJSON *array);
/* Retrieve item number "index" from array "array". Returns NULL if unsuccessful. */
CJSON_PUBLIC(cJSON *) cJSON_GetArrayItem(const cJSON *array, int index);
/* Get item "string" from object. Case insensitive. */
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItem(const cJSON * const object, const char * const string);
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItemCaseSensitive(const cJSON * const object, const char * const string);
CJSON_PUBLIC(cJSON_bool) cJSON_HasObjectItem(const cJSON *object, const char *string);
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
CJSON_PUBLIC(const char *) cJSON_GetErrorPtr(void);

/* Check item type and return its value */
CJSON_PUBLIC(char *) cJSON_GetStringValue(const cJSON * const item);
CJSON_PUBLIC(double) cJSON_GetNumberValue(const cJSON * const item);

/* These functions check the type of an item */
CJSON_PUBLIC(cJSON_bool) cJSON_IsInvalid(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsFalse(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsTrue(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsBool(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsNull(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsNumber(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsString(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsArray(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsObject(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsRaw(const cJSON * const item);

/* These calls create a cJSON item of the appropriate type. */
CJSON_PUBLIC(cJSON *) cJSON_CreateNull(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateTrue(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateFalse(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateBool(cJSON_bool boolean);
CJSON_PUBLIC(cJSON *) cJSON_CreateNumber(double num);
CJSON_PUBLIC(cJSON *) cJSON_CreateString(const char *string);
/* raw json */
CJSON_PUBLIC(cJSON *) cJSON_CreateRaw(const char *raw);
CJSON_PUBLIC(cJSON *) cJSON_CreateArray(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateObject(void);

/* Create a string where valuestring references a string so
 * it will not be freed by cJSON_Delete */
CJSON_PUBLIC(cJSON *) cJSON_CreateStringReference(const char *string);
/* Create an object/array that only references it's elements so
 * they will not be freed by cJSON_Delete */
CJSON_PUBLIC(cJSON *) cJSON_CreateObjectReference(const cJSON *child);
CJSON_PUBLIC(cJSON *) cJSON_CreateArrayReference(const cJSON *child);

/* These utilities create an Array of count items.
 * The parameter count cannot be greater than the number of elements in the number array, otherwise array access will be out of bounds.*/
CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count);
CJSON_PUBLIC(cJSON *) cJSON_CreateFloatArray(const float *numbers, int count);
CJSON_PUBLIC(cJSON *) cJSON_CreateDoubleArray(const double *numbers, int count);
CJSON_PUBLIC(cJSON *) cJSON_CreateStringArray(const char *const *strings, int count);

/* Append item to the specified array/object. */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToArray(cJSON *array, cJSON *item);
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the cJSON object.
 * WARNING: When this function was used, make sure to always check that (item->type & cJSON_StringIsConst) is zero before
 * writing to `item->string` */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON. */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item);

/* Remove/Detach items from Arrays/Objects. */
CJSON_PUBLIC(cJSON *) cJSON_DetachItemViaPointer(cJSON *parent, cJSON * const item);
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromArray(cJSON *array, int which);
CJSON_PUBLIC(void) cJSON_DeleteItemFromArray(cJSON *array, int which);
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObject(cJSON *object, const char *string);
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObjectCaseSensitive(cJSON *object, const char *string);
CJSON_PUBLIC(void) cJSON_DeleteItemFromObject(cJSON *object, const char *string);
CJSON_PUBLIC(void) cJSON_DeleteItemFromObjectCaseSensitive(cJSON *object, const char *string);

/* Update array items. */
CJSON_PUBLIC(cJSON_bool) cJSON_InsertItemInArray(cJSON *array, int which, cJSON *newitem); /* Shifts pre-existing items to the right. */
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemViaPointer(cJSON * const parent, cJSON * const item, cJSON * replacement);
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem);
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObjectCaseSensitive(cJSON *object,const char *string,cJSON *newitem);

/* Duplicate a cJSON item */
CJSON_PUBLIC(cJSON *) cJSON_Duplicate(const cJSON *item, cJSON_bool recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
 * need to be released. With recurse!=0, it will duplicate any children connected to the item.
 * The item->next and ->prev pointers are always zero on return from Duplicate. */
/* Recursively compare two cJSON items for equality. If either a or b is NULL or invalid, they will be considered unequal.
 * case_sensitive determines if object keys are treated case sensitive (1) or case insensitive (0) */
CJSON_PUBLIC(cJSON_bool) cJSON_Compare(const cJSON * const a, const cJSON * const b, const cJSON_bool case_sensitive);

/* Minify a strings, remove blank characters(such as ' ', '\t', '\r', '\n') from strings.
 * The input pointer json cannot point to a read-only address area, such as a string constant, 
 * but should point to a readable and writable address area. */
CJSON_PUBLIC(void) cJSON_Minify(char *json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
CJSON_PUBLIC(cJSON*) cJSON_AddNullToObject(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cJSON_AddTrueToObject(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cJSON_AddFalseToObject(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cJSON_AddBoolToObject(cJSON * const object, const char * const name, const cJSON_bool boolean);
CJSON_PUBLIC(cJSON*) cJSON_AddNumberToObject(cJSON * const object, const char * const name, const double number);
CJSON_PUBLIC(cJSON*) cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string);
CJSON_PUBLIC(cJSON*) cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw);
CJSON_PUBLIC(cJSON*) cJSON_AddObjectToObject(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cJSON_AddArrayToObject(cJSON * const object, const char * const name);

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define cJSON_SetIntValue(object, number) ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
/* helper for the cJSON_SetNumberValue macro */
CJSON_PUBLIC(double) cJSON_SetNumberHelper(cJSON *object, double number);
#define cJSON_SetNumberValue(object, number) ((object != NULL) ? cJSON_SetNumberHelper(object, (double)number) : (number))
/* Change the valuestring of a cJSON_String object, only takes effect when type of object is cJSON_String */
CJSON_PUBLIC(char*) cJSON_SetValuestring(cJSON *object, const char *valuestring);

/* If the object is not a boolean type this does nothing and returns cJSON_Invalid else it returns the new type*/
#define cJSON_SetBoolValue(object, boolValue) ( \
    (object != NULL && ((object)->type & (cJSON_False|cJSON_True))) ? \
    (object)->type=((object)->type &(~(cJSON_False|cJSON_True)))|((boolValue)?cJSON_True:cJSON_False) : \
    cJSON_Invalid\
)

/* Macro for iterating over an array or object */
#define cJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)

/* malloc/free objects using the malloc/free functions that have been set with cJSON_InitHooks */
CJSON_PUBLIC(void *) cJSON_malloc(size_t size);
CJSON_PUBLIC(void) cJSON_free(void *object);

#ifdef __cplusplus
}
#endif

#endif
#undef cJSON__h
#endif


char *private_cxpathjson_read_file(const char *filename);

int private_cxpathjson_write_file(const char *filename, const char *value);





int private_cxpathjson_get_json_type(cJSON *element);


const char * private_cxpathjson_convert_json_type_to_str(cJSON *element);





typedef struct CxpathJson{

    cJSON * element;
    bool raise_runtime_errors;
    char *error_message;
    cJSON *path_list;
     char *error_path_str;
    int error_code;

    struct CxpathJson *private_root;
    struct CxpathJson **childs;
    int size;

}CxpathJson;



#define CXPATHJSON_NOT_FOUND (-1)

#define CXPATHJSON_NULL 1

#define CXPATHJSON_STRING 2
#define CXPATHJSON_BOOL 3
#define CXPATHJSON_NUMBER 4
#define CXPATHJSON_OBJECT 5
#define CXPATHJSON_ARRAY 6

#define CXPATHJSON_NOT_FOUND_TEXT "not found"
#define CXPATHJSON_NULL_TEXT "Null"

#define CXPATHJSON_STRING_TEXT  "String"
#define CXPATHJSON_BOOL_TEXT "Boolean"
#define CXPATHJSON_NUMBER_TEXT "Number"
#define CXPATHJSON_OBJECT_TEXT "Object"
#define CXPATHJSON_ARRAY_TEXT "Array"


int CxpathJson_type(CxpathJson * self, const char *format, ...);

bool CxpathJson_exist(CxpathJson * self, const char *format, ...);

bool CxpathJson_its_not_null(CxpathJson * self, const char *format, ...);

bool CxpathJson_its_iterable(CxpathJson * self, const char *format, ...);

const char * CxpathJson_type_str(CxpathJson * self, const char *format, ...);




CxpathJson * private_newCxpathJson();

CxpathJson * private_CxpathJson_get_root(CxpathJson *self);

CxpathJson * private_CxpathJson_construct_child(CxpathJson  *self,cJSON *element);


CxpathJson * newCxpathJson_from_cJSON_getting_ownership(cJSON *element);

CxpathJson * newCxpathJsonObject();

CxpathJson * newCxpathJsonArray();

CxpathJson * newCxpathJson_from_string( const char *data);

CxpathJson * newCxpathJson_from_file(const char *path);


void CxpathJson_free(CxpathJson * self);


#define CXPATH_ERROR_NUM_RETURN (-1)
#define  CXPATHJSON_OK_CODE 0
#define CXPATHJSON_GENERIC_ERROR 1

#define  CXPATHJSON_FILE_NOT_FOUND_CODE 2
#define  CXPATHJSON_STRING_NOT_PARSABLE_CODE 3
#define  CXPATHJSON_ARG_PATH_NOT_VALID_CODE 4
#define  CXPATHJSON_ELEMENT_PATH_NOT_EXIST_CODE 5
#define  CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_OBJECT_CODE 6

#define  CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ITERABLE_CODE 7

#define  CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ARRAY_CODE 8
#define  CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE 9
#define  CXPATHJSON_PRINT_PROBLEM_CODE 10
#define  CXPATHJSON_IO_PROBLEM_CODE 12
#define XPATH_JSON_ELEMENT_NOT_HAVE_KEY_CODE 13
#define XPATH_JSON_ELEMENT_NOT_HAVE_KEY_MESSAGE "element does not have a key"

#define  CJSON_PATH_APPEND_ITS_NOT_VALID_ON_READ_OPERATIONS_CODE 13

#define  PRIVATE_CXPATHJSON_FILE_NOT_FOUND_MESSAGE "file %s  not exist"
#define PRIVATE_CPATHJSON_ON_PATH_FORMAT "%s on path %s"
#define PRIVATE_CXPATHJSON_ARG_PATH_NOT_VALID_MESSAGE "path %s its not well formated"

#define  PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE "element has type %s instead of %s"
#define  PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ARRAY_MESSAGE "middle element  its not array"
#define  PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ITERABLE_MESSAGE "Middle element its not iterable"
#define  PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_OBJECT_MESSAGE  "middle element its not a object"
#define  PRIVATE_CXPATHJSON_ELEMENT_PATH_NOT_EXIST_MESSAGE "Element not exist"
#define  PRIVATE_CXPATHJSON_STRING_NOT_PARSABLE_MESSAGE "string %s its not a valid json"
#define CXPATHJSON_IO_PROBLEM_MESSAGE "impossible to write in %s"

#define  CJSON_PATH_APPEND_ITS_NOT_VALID_ON_READ_OPERATIONS_MESSAGE "Append its not valid on read operations"

int CxpathJson_get_error_code(CxpathJson * self);

bool CxpathJson_has_errors(CxpathJson * self);

bool CxpathJson_is_ok(CxpathJson * self);

char * CxpathJson_get_error_message(CxpathJson * self);

char * CxpathJson_get_error_path(CxpathJson * self);


void CxpathJson_raise_errror(CxpathJson * self, int error_code, cJSON *path_list, const char *format, ...);


void CxpathJson_clear_errors(CxpathJson * self);



cJSON * private_CxpathJson_cJSON_by_cjson_path_list(CxpathJson * self, cJSON *path_list);


cJSON * private_CxpathJson_get_cJSON_by_vargs(CxpathJson * self, const char *format, va_list args);


cJSON * CxpathJson_get_cJSON(CxpathJson * self, const char *format, ...);


char * CxpathJson_get_str(CxpathJson * self, const char *format, ...);

char * CxpathJson_get_key(CxpathJson * self, const char *format, ...);

CxpathJson  * CxpathJson_get_object(CxpathJson * self, const char *format, ...);


CxpathJson  * CxpathJson_get_array(CxpathJson * self, const char *format, ...);


double CxpathJson_get_double(CxpathJson * self, const char *format, ...);

int CxpathJson_get_int(CxpathJson * self, const char *format, ...);

bool CxpathJson_get_bool(CxpathJson * self, const char *format, ...);


int CxpathJson_get_size(CxpathJson * self, const char *format, ...);




int private_CxpathJson_verifiy_if_insertion_is_possible(CxpathJson *self, cJSON *path_list);

void private_cxpathjson_set_cjson_by_path_list(CxpathJson *self, cJSON *value, cJSON *path_list);

void private_CxpathJson_set_cjson_by_va_arg_getting_ownership(CxpathJson *self, cJSON *value, const char *format, va_list args);


void CxpathJson_set_cjson_getting_ownership(CxpathJson *self, cJSON *value, const char *format, ...);

void CxpathJson_set_xpathJson_getting_onwership(CxpathJson *self, CxpathJson *value, const char *format, ...);

void CxpathJson_set_xpath_by_reference(CxpathJson *self, CxpathJson *value, const char *format, ...);

void CxpathJson_set_str(CxpathJson *self, const char *value, const char *format, ...);

void CxpathJson_set_str_getting_ownership(CxpathJson *self,  char *value, const char *format, ...);

void CxpathJson_set_double(CxpathJson *self, double value, const char *format, ...);

void CxpathJson_set_int(CxpathJson *self, int value, const char *format, ...);

void CxpathJson_set_bool(CxpathJson *self, bool value, const char *format, ...);


void CxpathJson_set_default_cjson_getting_ownership(CxpathJson *self, cJSON *value, const char *format, ...);

void CxpathJson_set_default_str_getting_onwership(CxpathJson *self,  char *value, const char *format, ...);


void CxpathJson_set_default_str(CxpathJson *self, const char *value, const char *format, ...);

void CxpathJson_set_default_double(CxpathJson *self, double value, const char *format, ...);

void CxpathJson_set_default_int(CxpathJson *self, int value, const char *format, ...);

void CxpathJson_set_default_bool(CxpathJson *self, bool value, const char *format, ...);





char *CxpathJson_dump_to_string(CxpathJson  *self, bool ident);


void CxpathJson_dump_to_file(CxpathJson  *self, const char *filename, bool ident);







void CxpathJson_destroy(CxpathJson *self,const char *format, ...);






int private_cxpathjson_convert_index(int index, int size);


bool private_cxpathjson_path_is_append(cJSON *current_path);


#ifndef CXPATHJSON_APPEND_KEY
#define CXPATHJSON_APPEND_KEY "$append"
#endif

int private_cxpathjson_validate_path_all(cJSON *path);

int private_cxpathjson_validate_path_read_only(cJSON *path);

void private_cxpathjson_replace_comas(char *result);


typedef struct {


    int (*get_error_code)(CxpathJson * self);
    bool (*is_ok)(CxpathJson * self);

    bool (*has_errors)(CxpathJson * self);
    char * (*get_error_message)(CxpathJson * self);
    char * (*get_error_path)(CxpathJson * self);
    void (*raise_errror)(CxpathJson * self, int error_code, cJSON *path_list, const char *format, ...);
    void (*clear_errors)(CxpathJson * self);


}CxpathJsonErrorNamespace;

CxpathJsonErrorNamespace newCxpathJsonErrorNamespace();


#define CXPATHJSON_NOT_FOUND (-1)

#define CXPATHJSON_NULL 1

#define CXPATHJSON_STRING 2
#define CXPATHJSON_BOOL 3
#define CXPATHJSON_NUMBER 4
#define CXPATHJSON_OBJECT 5
#define CXPATHJSON_ARRAY 6
typedef struct{

     int NOT_FOUND;
     int NULL_TYPE;
     int STRING_TYPE;
     int BOOL_TYPE;
     int NUMBER_TYPE;
     int OBJECT_TYPE;
     int ARRAY_TYPE;


    int (*type)(CxpathJson * self, const char *format, ...);
    bool (*exist)(CxpathJson * self, const char *format, ...);
    bool (*its_not_null)(CxpathJson * self, const char *format, ...);
    const char * (*type_str)(CxpathJson * self, const char *format, ...);
}CxpathJsonTypeNamespace;


CxpathJsonTypeNamespace newCxpathJsonTypeNamespace(void);



typedef struct {



    char *(*dump_to_string)(CxpathJson  *self, bool ident);
    void (*dump_to_file)(CxpathJson  *self,const char *filename, bool ident);
    

    CxpathJson * (*new_from_cJSON)(cJSON *element);
    CxpathJson * (*newJsonObject)(void);
    CxpathJson * (*newJsonArray)(void);
    CxpathJson * (*new_from_string)( const char *data);
    CxpathJson * (*new_from_file)(const char *path);
    void (*free)(CxpathJson * self);

    cJSON * (*get_cJSON)(CxpathJson * self, const char *format, ...);
     char * (*get_str)(CxpathJson * self, const char *format, ...);
    double (*get_double)(CxpathJson * self, const char *format, ...);
    int (*get_int)(CxpathJson * self, const char *format, ...);
    bool (*get_bool)(CxpathJson * self, const char *format, ...);
    int (*size)(CxpathJson * self, const char *format, ...);
    CxpathJson  * (*get_object)(CxpathJson * self, const char *format, ...);
    CxpathJson  * (*get_array)(CxpathJson * self, const char *format, ...);
    char * (*get_key)(CxpathJson * self, const char *format, ...);



    void (*set_cjson_getting_ownership)(CxpathJson *self, cJSON *value, const char *format, ...);
    void (*set_str)(CxpathJson *self, const char *value, const char *format, ...);
    void (*set_str_getting_ownership)(CxpathJson *self, char *value, const char *format, ...);
    void (*set_double)(CxpathJson *self, double value, const char *format, ...);
    void (*set_int)(CxpathJson *self, int value, const char *format, ...);
    void (*set_bool)(CxpathJson *self, bool value, const char *format, ...);
    void (*set_xpathJson_getting_onwership)(CxpathJson *self, CxpathJson *value, const char *format, ...);
    void (*set_xpath_by_reference)(CxpathJson *self, CxpathJson *value, const char *format, ...);

    void (*set_default_cjson_getting_ownership)(CxpathJson *self, cJSON *value, const char *format, ...);
    void (*set_default_str_getting_onwership)(CxpathJson *self,  char *value, const char *format, ...);
    void (*set_default_str)(CxpathJson *self, const char *value, const char *format, ...);
    void (*set_default_double)(CxpathJson *self, double value, const char *format, ...);
    void (*set_default_int)(CxpathJson *self, int value, const char *format, ...);
    void (*set_default_bool)(CxpathJson *self, bool value, const char *format, ...);




    void (*destroy)(CxpathJson *self,const char *format, ...);


    CxpathJsonErrorNamespace errors;
    CxpathJsonTypeNamespace  types;
}CxpathJsonNamespace;

CxpathJsonNamespace newCxpathJsonNamespace();





#ifndef cJSON__h

/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

/* cJSON */
/* JSON parser in C. */

/* disable warnings about old C89 functions in MSVC */
#if !defined(_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif
#if defined(_MSC_VER)
#pragma warning (push)
/* disable warning about single line comments in system headers */
#pragma warning (disable : 4001)
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <float.h>

#ifdef ENABLE_LOCALES
#include <locale.h>
#endif

#if defined(_MSC_VER)
#pragma warning (pop)
#endif
#ifdef __GNUC__
#pragma GCC visibility pop
#endif


/* define our own boolean type */
#ifdef true
#undef true
#endif
#define true ((cJSON_bool)1)

#ifdef false
#undef false
#endif
#define false ((cJSON_bool)0)

/* define isnan and isinf for ANSI C, if in C99 or above, isnan and isinf has been defined in math.h */
#ifndef isinf
#define isinf(d) (isnan((d - d)) && !isnan(d))
#endif
#ifndef isnan
#define isnan(d) (d != d)
#endif

#ifndef NAN
#ifdef _WIN32
#define NAN sqrt(-1.0)
#else
#define NAN 0.0/0.0
#endif
#endif

typedef struct {
    const unsigned char *json;
    size_t position;
} error;
static error global_error = { NULL, 0 };

CJSON_PUBLIC(const char *) cJSON_GetErrorPtr(void)
{
    return (const char*) (global_error.json + global_error.position);
}

CJSON_PUBLIC(char *) cJSON_GetStringValue(const cJSON * const item)
{
    if (!cJSON_IsString(item))
    {
        return NULL;
    }

    return item->valuestring;
}

CJSON_PUBLIC(double) cJSON_GetNumberValue(const cJSON * const item)
{
    if (!cJSON_IsNumber(item))
    {
        return (double) NAN;
    }

    return item->valuedouble;
}

/* This is a safeguard to prevent copy-pasters from using incompatible C and header files */
#if (CJSON_VERSION_MAJOR != 1) || (CJSON_VERSION_MINOR != 7) || (CJSON_VERSION_PATCH != 17)
    #error cJSON.h and cJSON.c have different versions. Make sure that both have the same.
#endif

CJSON_PUBLIC(const char*) cJSON_Version(void)
{
    static char version[15];
    sprintf(version, "%i.%i.%i", CJSON_VERSION_MAJOR, CJSON_VERSION_MINOR, CJSON_VERSION_PATCH);

    return version;
}

/* Case insensitive string comparison, doesn't consider two NULL pointers equal though */
static int case_insensitive_strcmp(const unsigned char *string1, const unsigned char *string2)
{
    if ((string1 == NULL) || (string2 == NULL))
    {
        return 1;
    }

    if (string1 == string2)
    {
        return 0;
    }

    for(; tolower(*string1) == tolower(*string2); (void)string1++, string2++)
    {
        if (*string1 == '\0')
        {
            return 0;
        }
    }

    return tolower(*string1) - tolower(*string2);
}

typedef struct internal_hooks
{
    void *(CJSON_CDECL *allocate)(size_t size);
    void (CJSON_CDECL *deallocate)(void *pointer);
    void *(CJSON_CDECL *reallocate)(void *pointer, size_t size);
} internal_hooks;

#if defined(_MSC_VER)
/* work around MSVC error C2322: '...' address of dllimport '...' is not static */
static void * CJSON_CDECL internal_malloc(size_t size)
{
    return malloc(size);
}
static void CJSON_CDECL internal_free(void *pointer)
{
    free(pointer);
}
static void * CJSON_CDECL internal_realloc(void *pointer, size_t size)
{
    return realloc(pointer, size);
}
#else
#define internal_malloc malloc
#define internal_free free
#define internal_realloc realloc
#endif

/* strlen of character literals resolved at compile time */
#define static_strlen(string_literal) (sizeof(string_literal) - sizeof(""))

static internal_hooks global_hooks = { internal_malloc, internal_free, internal_realloc };

static unsigned char* cJSON_strdup(const unsigned char* string, const internal_hooks * const hooks)
{
    size_t length = 0;
    unsigned char *copy = NULL;

    if (string == NULL)
    {
        return NULL;
    }

    length = strlen((const char*)string) + sizeof("");
    copy = (unsigned char*)hooks->allocate(length);
    if (copy == NULL)
    {
        return NULL;
    }
    memcpy(copy, string, length);

    return copy;
}

CJSON_PUBLIC(void) cJSON_InitHooks(cJSON_Hooks* hooks)
{
    if (hooks == NULL)
    {
        /* Reset hooks */
        global_hooks.allocate = malloc;
        global_hooks.deallocate = free;
        global_hooks.reallocate = realloc;
        return;
    }

    global_hooks.allocate = malloc;
    if (hooks->malloc_fn != NULL)
    {
        global_hooks.allocate = hooks->malloc_fn;
    }

    global_hooks.deallocate = free;
    if (hooks->free_fn != NULL)
    {
        global_hooks.deallocate = hooks->free_fn;
    }

    /* use realloc only if both free and malloc are used */
    global_hooks.reallocate = NULL;
    if ((global_hooks.allocate == malloc) && (global_hooks.deallocate == free))
    {
        global_hooks.reallocate = realloc;
    }
}

/* Internal constructor. */
static cJSON *cJSON_New_Item(const internal_hooks * const hooks)
{
    cJSON* node = (cJSON*)hooks->allocate(sizeof(cJSON));
    if (node)
    {
        memset(node, '\0', sizeof(cJSON));
    }

    return node;
}

/* Delete a cJSON structure. */
CJSON_PUBLIC(void) cJSON_Delete(cJSON *item)
{
    cJSON *next = NULL;
    while (item != NULL)
    {
        next = item->next;
        if (!(item->type & cJSON_IsReference) && (item->child != NULL))
        {
            cJSON_Delete(item->child);
        }
        if (!(item->type & cJSON_IsReference) && (item->valuestring != NULL))
        {
            global_hooks.deallocate(item->valuestring);
        }
        if (!(item->type & cJSON_StringIsConst) && (item->string != NULL))
        {
            global_hooks.deallocate(item->string);
        }
        global_hooks.deallocate(item);
        item = next;
    }
}

/* get the decimal point character of the current locale */
static unsigned char get_decimal_point(void)
{
#ifdef ENABLE_LOCALES
    struct lconv *lconv = localeconv();
    return (unsigned char) lconv->decimal_point[0];
#else
    return '.';
#endif
}

typedef struct
{
    const unsigned char *content;
    size_t length;
    size_t offset;
    size_t depth; /* How deeply nested (in arrays/objects) is the input at the current offset. */
    internal_hooks hooks;
} parse_buffer;

/* check if the given size is left to read in a given parse buffer (starting with 1) */
#define can_read(buffer, size) ((buffer != NULL) && (((buffer)->offset + size) <= (buffer)->length))
/* check if the buffer can be accessed at the given index (starting with 0) */
#define can_access_at_index(buffer, index) ((buffer != NULL) && (((buffer)->offset + index) < (buffer)->length))
#define cannot_access_at_index(buffer, index) (!can_access_at_index(buffer, index))
/* get a pointer to the buffer at the position */
#define buffer_at_offset(buffer) ((buffer)->content + (buffer)->offset)

/* Parse the input text to generate a number, and populate the result into item. */
static cJSON_bool parse_number(cJSON * const item, parse_buffer * const input_buffer)
{
    double number = 0;
    unsigned char *after_end = NULL;
    unsigned char number_c_string[64];
    unsigned char decimal_point = get_decimal_point();
    size_t i = 0;

    if ((input_buffer == NULL) || (input_buffer->content == NULL))
    {
        return false;
    }

    /* copy the number into a temporary buffer and replace '.' with the decimal point
     * of the current locale (for strtod)
     * This also takes care of '\0' not necessarily being available for marking the end of the input */
    for (i = 0; (i < (sizeof(number_c_string) - 1)) && can_access_at_index(input_buffer, i); i++)
    {
        switch (buffer_at_offset(input_buffer)[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '+':
            case '-':
            case 'e':
            case 'E':
                number_c_string[i] = buffer_at_offset(input_buffer)[i];
                break;

            case '.':
                number_c_string[i] = decimal_point;
                break;

            default:
                goto loop_end;
        }
    }
loop_end:
    number_c_string[i] = '\0';

    number = strtod((const char*)number_c_string, (char**)&after_end);
    if (number_c_string == after_end)
    {
        return false; /* parse_error */
    }

    item->valuedouble = number;

    /* use saturation in case of overflow */
    if (number >= INT_MAX)
    {
        item->valueint = INT_MAX;
    }
    else if (number <= (double)INT_MIN)
    {
        item->valueint = INT_MIN;
    }
    else
    {
        item->valueint = (int)number;
    }

    item->type = cJSON_Number;

    input_buffer->offset += (size_t)(after_end - number_c_string);
    return true;
}

/* don't ask me, but the original cJSON_SetNumberValue returns an integer or double */
CJSON_PUBLIC(double) cJSON_SetNumberHelper(cJSON *object, double number)
{
    if (number >= INT_MAX)
    {
        object->valueint = INT_MAX;
    }
    else if (number <= (double)INT_MIN)
    {
        object->valueint = INT_MIN;
    }
    else
    {
        object->valueint = (int)number;
    }

    return object->valuedouble = number;
}

CJSON_PUBLIC(char*) cJSON_SetValuestring(cJSON *object, const char *valuestring)
{
    char *copy = NULL;
    /* if object's type is not cJSON_String or is cJSON_IsReference, it should not set valuestring */
    if ((object == NULL) || !(object->type & cJSON_String) || (object->type & cJSON_IsReference))
    {
        return NULL;
    }
    /* return NULL if the object is corrupted */
    if (object->valuestring == NULL)
    {
        return NULL;
    }
    if (strlen(valuestring) <= strlen(object->valuestring))
    {
        strcpy(object->valuestring, valuestring);
        return object->valuestring;
    }
    copy = (char*) cJSON_strdup((const unsigned char*)valuestring, &global_hooks);
    if (copy == NULL)
    {
        return NULL;
    }
    if (object->valuestring != NULL)
    {
        cJSON_free(object->valuestring);
    }
    object->valuestring = copy;

    return copy;
}

typedef struct
{
    unsigned char *buffer;
    size_t length;
    size_t offset;
    size_t depth; /* current nesting depth (for formatted printing) */
    cJSON_bool noalloc;
    cJSON_bool format; /* is this print a formatted print */
    internal_hooks hooks;
} printbuffer;

/* realloc printbuffer if necessary to have at least "needed" bytes more */
static unsigned char* ensure(printbuffer * const p, size_t needed)
{
    unsigned char *newbuffer = NULL;
    size_t newsize = 0;

    if ((p == NULL) || (p->buffer == NULL))
    {
        return NULL;
    }

    if ((p->length > 0) && (p->offset >= p->length))
    {
        /* make sure that offset is valid */
        return NULL;
    }

    if (needed > INT_MAX)
    {
        /* sizes bigger than INT_MAX are currently not supported */
        return NULL;
    }

    needed += p->offset + 1;
    if (needed <= p->length)
    {
        return p->buffer + p->offset;
    }

    if (p->noalloc) {
        return NULL;
    }

    /* calculate new buffer size */
    if (needed > (INT_MAX / 2))
    {
        /* overflow of int, use INT_MAX if possible */
        if (needed <= INT_MAX)
        {
            newsize = INT_MAX;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        newsize = needed * 2;
    }

    if (p->hooks.reallocate != NULL)
    {
        /* reallocate with realloc if available */
        newbuffer = (unsigned char*)p->hooks.reallocate(p->buffer, newsize);
        if (newbuffer == NULL)
        {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;

            return NULL;
        }
    }
    else
    {
        /* otherwise reallocate manually */
        newbuffer = (unsigned char*)p->hooks.allocate(newsize);
        if (!newbuffer)
        {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;

            return NULL;
        }

        memcpy(newbuffer, p->buffer, p->offset + 1);
        p->hooks.deallocate(p->buffer);
    }
    p->length = newsize;
    p->buffer = newbuffer;

    return newbuffer + p->offset;
}

/* calculate the new length of the string in a printbuffer and update the offset */
static void update_offset(printbuffer * const buffer)
{
    const unsigned char *buffer_pointer = NULL;
    if ((buffer == NULL) || (buffer->buffer == NULL))
    {
        return;
    }
    buffer_pointer = buffer->buffer + buffer->offset;

    buffer->offset += strlen((const char*)buffer_pointer);
}

/* securely comparison of floating-point variables */
static cJSON_bool compare_double(double a, double b)
{
    double maxVal = fabs(a) > fabs(b) ? fabs(a) : fabs(b);
    return (fabs(a - b) <= maxVal * DBL_EPSILON);
}

/* Render the number nicely from the given item into a string. */
static cJSON_bool print_number(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    double d = item->valuedouble;
    int length = 0;
    size_t i = 0;
    unsigned char number_buffer[26] = {0}; /* temporary buffer to print the number into */
    unsigned char decimal_point = get_decimal_point();
    double test = 0.0;

    if (output_buffer == NULL)
    {
        return false;
    }

    /* This checks for NaN and Infinity */
    if (isnan(d) || isinf(d))
    {
        length = sprintf((char*)number_buffer, "null");
    }
	else if(d == (double)item->valueint)
	{
		length = sprintf((char*)number_buffer, "%d", item->valueint);
	}
    else
    {
        /* Try 15 decimal places of precision to avoid nonsignificant nonzero digits */
        length = sprintf((char*)number_buffer, "%1.15g", d);

        /* Check whether the original double can be recovered */
        if ((sscanf((char*)number_buffer, "%lg", &test) != 1) || !compare_double((double)test, d))
        {
            /* If not, print with 17 decimal places of precision */
            length = sprintf((char*)number_buffer, "%1.17g", d);
        }
    }

    /* sprintf failed or buffer overrun occurred */
    if ((length < 0) || (length > (int)(sizeof(number_buffer) - 1)))
    {
        return false;
    }

    /* reserve appropriate space in the output */
    output_pointer = ensure(output_buffer, (size_t)length + sizeof(""));
    if (output_pointer == NULL)
    {
        return false;
    }

    /* copy the printed number to the output and replace locale
     * dependent decimal point with '.' */
    for (i = 0; i < ((size_t)length); i++)
    {
        if (number_buffer[i] == decimal_point)
        {
            output_pointer[i] = '.';
            continue;
        }

        output_pointer[i] = number_buffer[i];
    }
    output_pointer[i] = '\0';

    output_buffer->offset += (size_t)length;

    return true;
}

/* parse 4 digit hexadecimal number */
static unsigned parse_hex4(const unsigned char * const input)
{
    unsigned int h = 0;
    size_t i = 0;

    for (i = 0; i < 4; i++)
    {
        /* parse digit */
        if ((input[i] >= '0') && (input[i] <= '9'))
        {
            h += (unsigned int) input[i] - '0';
        }
        else if ((input[i] >= 'A') && (input[i] <= 'F'))
        {
            h += (unsigned int) 10 + input[i] - 'A';
        }
        else if ((input[i] >= 'a') && (input[i] <= 'f'))
        {
            h += (unsigned int) 10 + input[i] - 'a';
        }
        else /* invalid */
        {
            return 0;
        }

        if (i < 3)
        {
            /* shift left to make place for the next nibble */
            h = h << 4;
        }
    }

    return h;
}

/* converts a UTF-16 literal to UTF-8
 * A literal can be one or two sequences of the form \uXXXX */
static unsigned char utf16_literal_to_utf8(const unsigned char * const input_pointer, const unsigned char * const input_end, unsigned char **output_pointer)
{
    long unsigned int codepoint = 0;
    unsigned int first_code = 0;
    const unsigned char *first_sequence = input_pointer;
    unsigned char utf8_length = 0;
    unsigned char utf8_position = 0;
    unsigned char sequence_length = 0;
    unsigned char first_byte_mark = 0;

    if ((input_end - first_sequence) < 6)
    {
        /* input ends unexpectedly */
        goto fail;
    }

    /* get the first utf16 sequence */
    first_code = parse_hex4(first_sequence + 2);

    /* check that the code is valid */
    if (((first_code >= 0xDC00) && (first_code <= 0xDFFF)))
    {
        goto fail;
    }

    /* UTF16 surrogate pair */
    if ((first_code >= 0xD800) && (first_code <= 0xDBFF))
    {
        const unsigned char *second_sequence = first_sequence + 6;
        unsigned int second_code = 0;
        sequence_length = 12; /* \uXXXX\uXXXX */

        if ((input_end - second_sequence) < 6)
        {
            /* input ends unexpectedly */
            goto fail;
        }

        if ((second_sequence[0] != '\\') || (second_sequence[1] != 'u'))
        {
            /* missing second half of the surrogate pair */
            goto fail;
        }

        /* get the second utf16 sequence */
        second_code = parse_hex4(second_sequence + 2);
        /* check that the code is valid */
        if ((second_code < 0xDC00) || (second_code > 0xDFFF))
        {
            /* invalid second half of the surrogate pair */
            goto fail;
        }


        /* calculate the unicode codepoint from the surrogate pair */
        codepoint = 0x10000 + (((first_code & 0x3FF) << 10) | (second_code & 0x3FF));
    }
    else
    {
        sequence_length = 6; /* \uXXXX */
        codepoint = first_code;
    }

    /* encode as UTF-8
     * takes at maximum 4 bytes to encode:
     * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
    if (codepoint < 0x80)
    {
        /* normal ascii, encoding 0xxxxxxx */
        utf8_length = 1;
    }
    else if (codepoint < 0x800)
    {
        /* two bytes, encoding 110xxxxx 10xxxxxx */
        utf8_length = 2;
        first_byte_mark = 0xC0; /* 11000000 */
    }
    else if (codepoint < 0x10000)
    {
        /* three bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx */
        utf8_length = 3;
        first_byte_mark = 0xE0; /* 11100000 */
    }
    else if (codepoint <= 0x10FFFF)
    {
        /* four bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx 10xxxxxx */
        utf8_length = 4;
        first_byte_mark = 0xF0; /* 11110000 */
    }
    else
    {
        /* invalid unicode codepoint */
        goto fail;
    }

    /* encode as utf8 */
    for (utf8_position = (unsigned char)(utf8_length - 1); utf8_position > 0; utf8_position--)
    {
        /* 10xxxxxx */
        (*output_pointer)[utf8_position] = (unsigned char)((codepoint | 0x80) & 0xBF);
        codepoint >>= 6;
    }
    /* encode first byte */
    if (utf8_length > 1)
    {
        (*output_pointer)[0] = (unsigned char)((codepoint | first_byte_mark) & 0xFF);
    }
    else
    {
        (*output_pointer)[0] = (unsigned char)(codepoint & 0x7F);
    }

    *output_pointer += utf8_length;

    return sequence_length;

fail:
    return 0;
}

/* Parse the input text into an unescaped cinput, and populate item. */
static cJSON_bool parse_string(cJSON * const item, parse_buffer * const input_buffer)
{
    const unsigned char *input_pointer = buffer_at_offset(input_buffer) + 1;
    const unsigned char *input_end = buffer_at_offset(input_buffer) + 1;
    unsigned char *output_pointer = NULL;
    unsigned char *output = NULL;

    /* not a string */
    if (buffer_at_offset(input_buffer)[0] != '\"')
    {
        goto fail;
    }

    {
        /* calculate approximate size of the output (overestimate) */
        size_t allocation_length = 0;
        size_t skipped_bytes = 0;
        while (((size_t)(input_end - input_buffer->content) < input_buffer->length) && (*input_end != '\"'))
        {
            /* is escape sequence */
            if (input_end[0] == '\\')
            {
                if ((size_t)(input_end + 1 - input_buffer->content) >= input_buffer->length)
                {
                    /* prevent buffer overflow when last input character is a backslash */
                    goto fail;
                }
                skipped_bytes++;
                input_end++;
            }
            input_end++;
        }
        if (((size_t)(input_end - input_buffer->content) >= input_buffer->length) || (*input_end != '\"'))
        {
            goto fail; /* string ended unexpectedly */
        }

        /* This is at most how much we need for the output */
        allocation_length = (size_t) (input_end - buffer_at_offset(input_buffer)) - skipped_bytes;
        output = (unsigned char*)input_buffer->hooks.allocate(allocation_length + sizeof(""));
        if (output == NULL)
        {
            goto fail; /* allocation failure */
        }
    }

    output_pointer = output;
    /* loop through the string literal */
    while (input_pointer < input_end)
    {
        if (*input_pointer != '\\')
        {
            *output_pointer++ = *input_pointer++;
        }
        /* escape sequence */
        else
        {
            unsigned char sequence_length = 2;
            if ((input_end - input_pointer) < 1)
            {
                goto fail;
            }

            switch (input_pointer[1])
            {
                case 'b':
                    *output_pointer++ = '\b';
                    break;
                case 'f':
                    *output_pointer++ = '\f';
                    break;
                case 'n':
                    *output_pointer++ = '\n';
                    break;
                case 'r':
                    *output_pointer++ = '\r';
                    break;
                case 't':
                    *output_pointer++ = '\t';
                    break;
                case '\"':
                case '\\':
                case '/':
                    *output_pointer++ = input_pointer[1];
                    break;

                /* UTF-16 literal */
                case 'u':
                    sequence_length = utf16_literal_to_utf8(input_pointer, input_end, &output_pointer);
                    if (sequence_length == 0)
                    {
                        /* failed to convert UTF16-literal to UTF-8 */
                        goto fail;
                    }
                    break;

                default:
                    goto fail;
            }
            input_pointer += sequence_length;
        }
    }

    /* zero terminate the output */
    *output_pointer = '\0';

    item->type = cJSON_String;
    item->valuestring = (char*)output;

    input_buffer->offset = (size_t) (input_end - input_buffer->content);
    input_buffer->offset++;

    return true;

fail:
    if (output != NULL)
    {
        input_buffer->hooks.deallocate(output);
    }

    if (input_pointer != NULL)
    {
        input_buffer->offset = (size_t)(input_pointer - input_buffer->content);
    }

    return false;
}

/* Render the cstring provided to an escaped version that can be printed. */
static cJSON_bool print_string_ptr(const unsigned char * const input, printbuffer * const output_buffer)
{
    const unsigned char *input_pointer = NULL;
    unsigned char *output = NULL;
    unsigned char *output_pointer = NULL;
    size_t output_length = 0;
    /* numbers of additional characters needed for escaping */
    size_t escape_characters = 0;

    if (output_buffer == NULL)
    {
        return false;
    }

    /* empty string */
    if (input == NULL)
    {
        output = ensure(output_buffer, sizeof("\"\""));
        if (output == NULL)
        {
            return false;
        }
        strcpy((char*)output, "\"\"");

        return true;
    }

    /* set "flag" to 1 if something needs to be escaped */
    for (input_pointer = input; *input_pointer; input_pointer++)
    {
        switch (*input_pointer)
        {
            case '\"':
            case '\\':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
                /* one character escape sequence */
                escape_characters++;
                break;
            default:
                if (*input_pointer < 32)
                {
                    /* UTF-16 escape sequence uXXXX */
                    escape_characters += 5;
                }
                break;
        }
    }
    output_length = (size_t)(input_pointer - input) + escape_characters;

    output = ensure(output_buffer, output_length + sizeof("\"\""));
    if (output == NULL)
    {
        return false;
    }

    /* no characters have to be escaped */
    if (escape_characters == 0)
    {
        output[0] = '\"';
        memcpy(output + 1, input, output_length);
        output[output_length + 1] = '\"';
        output[output_length + 2] = '\0';

        return true;
    }

    output[0] = '\"';
    output_pointer = output + 1;
    /* copy the string */
    for (input_pointer = input; *input_pointer != '\0'; (void)input_pointer++, output_pointer++)
    {
        if ((*input_pointer > 31) && (*input_pointer != '\"') && (*input_pointer != '\\'))
        {
            /* normal character, copy */
            *output_pointer = *input_pointer;
        }
        else
        {
            /* character needs to be escaped */
            *output_pointer++ = '\\';
            switch (*input_pointer)
            {
                case '\\':
                    *output_pointer = '\\';
                    break;
                case '\"':
                    *output_pointer = '\"';
                    break;
                case '\b':
                    *output_pointer = 'b';
                    break;
                case '\f':
                    *output_pointer = 'f';
                    break;
                case '\n':
                    *output_pointer = 'n';
                    break;
                case '\r':
                    *output_pointer = 'r';
                    break;
                case '\t':
                    *output_pointer = 't';
                    break;
                default:
                    /* escape and print as unicode codepoint */
                    sprintf((char*)output_pointer, "u%04x", *input_pointer);
                    output_pointer += 4;
                    break;
            }
        }
    }
    output[output_length + 1] = '\"';
    output[output_length + 2] = '\0';

    return true;
}

/* Invoke print_string_ptr (which is useful) on an item. */
static cJSON_bool print_string(const cJSON * const item, printbuffer * const p)
{
    return print_string_ptr((unsigned char*)item->valuestring, p);
}

/* Predeclare these prototypes. */
static cJSON_bool parse_value(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_value(const cJSON * const item, printbuffer * const output_buffer);
static cJSON_bool parse_array(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_array(const cJSON * const item, printbuffer * const output_buffer);
static cJSON_bool parse_object(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_object(const cJSON * const item, printbuffer * const output_buffer);

/* Utility to jump whitespace and cr/lf */
static parse_buffer *buffer_skip_whitespace(parse_buffer * const buffer)
{
    if ((buffer == NULL) || (buffer->content == NULL))
    {
        return NULL;
    }

    if (cannot_access_at_index(buffer, 0))
    {
        return buffer;
    }

    while (can_access_at_index(buffer, 0) && (buffer_at_offset(buffer)[0] <= 32))
    {
       buffer->offset++;
    }

    if (buffer->offset == buffer->length)
    {
        buffer->offset--;
    }

    return buffer;
}

/* skip the UTF-8 BOM (byte order mark) if it is at the beginning of a buffer */
static parse_buffer *skip_utf8_bom(parse_buffer * const buffer)
{
    if ((buffer == NULL) || (buffer->content == NULL) || (buffer->offset != 0))
    {
        return NULL;
    }

    if (can_access_at_index(buffer, 4) && (strncmp((const char*)buffer_at_offset(buffer), "\xEF\xBB\xBF", 3) == 0))
    {
        buffer->offset += 3;
    }

    return buffer;
}

CJSON_PUBLIC(cJSON *) cJSON_ParseWithOpts(const char *value, const char **return_parse_end, cJSON_bool require_null_terminated)
{
    size_t buffer_length;

    if (NULL == value)
    {
        return NULL;
    }

    /* Adding null character size due to require_null_terminated. */
    buffer_length = strlen(value) + sizeof("");

    return cJSON_ParseWithLengthOpts(value, buffer_length, return_parse_end, require_null_terminated);
}

/* Parse an object - create a new root, and populate. */
CJSON_PUBLIC(cJSON *) cJSON_ParseWithLengthOpts(const char *value, size_t buffer_length, const char **return_parse_end, cJSON_bool require_null_terminated)
{
    parse_buffer buffer = { 0, 0, 0, 0, { 0, 0, 0 } };
    cJSON *item = NULL;

    /* reset error position */
    global_error.json = NULL;
    global_error.position = 0;

    if (value == NULL || 0 == buffer_length)
    {
        goto fail;
    }

    buffer.content = (const unsigned char*)value;
    buffer.length = buffer_length;
    buffer.offset = 0;
    buffer.hooks = global_hooks;

    item = cJSON_New_Item(&global_hooks);
    if (item == NULL) /* memory fail */
    {
        goto fail;
    }

    if (!parse_value(item, buffer_skip_whitespace(skip_utf8_bom(&buffer))))
    {
        /* parse failure. ep is set. */
        goto fail;
    }

    /* if we require null-terminated JSON without appended garbage, skip and then check for a null terminator */
    if (require_null_terminated)
    {
        buffer_skip_whitespace(&buffer);
        if ((buffer.offset >= buffer.length) || buffer_at_offset(&buffer)[0] != '\0')
        {
            goto fail;
        }
    }
    if (return_parse_end)
    {
        *return_parse_end = (const char*)buffer_at_offset(&buffer);
    }

    return item;

fail:
    if (item != NULL)
    {
        cJSON_Delete(item);
    }

    if (value != NULL)
    {
        error local_error;
        local_error.json = (const unsigned char*)value;
        local_error.position = 0;

        if (buffer.offset < buffer.length)
        {
            local_error.position = buffer.offset;
        }
        else if (buffer.length > 0)
        {
            local_error.position = buffer.length - 1;
        }

        if (return_parse_end != NULL)
        {
            *return_parse_end = (const char*)local_error.json + local_error.position;
        }

        global_error = local_error;
    }

    return NULL;
}

/* Default options for cJSON_Parse */
CJSON_PUBLIC(cJSON *) cJSON_Parse(const char *value)
{
    return cJSON_ParseWithOpts(value, 0, 0);
}

CJSON_PUBLIC(cJSON *) cJSON_ParseWithLength(const char *value, size_t buffer_length)
{
    return cJSON_ParseWithLengthOpts(value, buffer_length, 0, 0);
}

#define cjson_min(a, b) (((a) < (b)) ? (a) : (b))

static unsigned char *print(const cJSON * const item, cJSON_bool format, const internal_hooks * const hooks)
{
    static const size_t default_buffer_size = 256;
    printbuffer buffer[1];
    unsigned char *printed = NULL;

    memset(buffer, 0, sizeof(buffer));

    /* create buffer */
    buffer->buffer = (unsigned char*) hooks->allocate(default_buffer_size);
    buffer->length = default_buffer_size;
    buffer->format = format;
    buffer->hooks = *hooks;
    if (buffer->buffer == NULL)
    {
        goto fail;
    }

    /* print the value */
    if (!print_value(item, buffer))
    {
        goto fail;
    }
    update_offset(buffer);

    /* check if reallocate is available */
    if (hooks->reallocate != NULL)
    {
        printed = (unsigned char*) hooks->reallocate(buffer->buffer, buffer->offset + 1);
        if (printed == NULL) {
            goto fail;
        }
        buffer->buffer = NULL;
    }
    else /* otherwise copy the JSON over to a new buffer */
    {
        printed = (unsigned char*) hooks->allocate(buffer->offset + 1);
        if (printed == NULL)
        {
            goto fail;
        }
        memcpy(printed, buffer->buffer, cjson_min(buffer->length, buffer->offset + 1));
        printed[buffer->offset] = '\0'; /* just to be sure */

        /* free the buffer */
        hooks->deallocate(buffer->buffer);
    }

    return printed;

fail:
    if (buffer->buffer != NULL)
    {
        hooks->deallocate(buffer->buffer);
    }

    if (printed != NULL)
    {
        hooks->deallocate(printed);
    }

    return NULL;
}

/* Render a cJSON item/entity/structure to text. */
CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item)
{
    return (char*)print(item, true, &global_hooks);
}

CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item)
{
    return (char*)print(item, false, &global_hooks);
}

CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON *item, int prebuffer, cJSON_bool fmt)
{
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };

    if (prebuffer < 0)
    {
        return NULL;
    }

    p.buffer = (unsigned char*)global_hooks.allocate((size_t)prebuffer);
    if (!p.buffer)
    {
        return NULL;
    }

    p.length = (size_t)prebuffer;
    p.offset = 0;
    p.noalloc = false;
    p.format = fmt;
    p.hooks = global_hooks;

    if (!print_value(item, &p))
    {
        global_hooks.deallocate(p.buffer);
        return NULL;
    }

    return (char*)p.buffer;
}

CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buffer, const int length, const cJSON_bool format)
{
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };

    if ((length < 0) || (buffer == NULL))
    {
        return false;
    }

    p.buffer = (unsigned char*)buffer;
    p.length = (size_t)length;
    p.offset = 0;
    p.noalloc = true;
    p.format = format;
    p.hooks = global_hooks;

    return print_value(item, &p);
}

/* Parser core - when encountering text, process appropriately. */
static cJSON_bool parse_value(cJSON * const item, parse_buffer * const input_buffer)
{
    if ((input_buffer == NULL) || (input_buffer->content == NULL))
    {
        return false; /* no input */
    }

    /* parse the different types of values */
    /* null */
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), "null", 4) == 0))
    {
        item->type = cJSON_NULL;
        input_buffer->offset += 4;
        return true;
    }
    /* false */
    if (can_read(input_buffer, 5) && (strncmp((const char*)buffer_at_offset(input_buffer), "false", 5) == 0))
    {
        item->type = cJSON_False;
        input_buffer->offset += 5;
        return true;
    }
    /* true */
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), "true", 4) == 0))
    {
        item->type = cJSON_True;
        item->valueint = 1;
        input_buffer->offset += 4;
        return true;
    }
    /* string */
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '\"'))
    {
        return parse_string(item, input_buffer);
    }
    /* number */
    if (can_access_at_index(input_buffer, 0) && ((buffer_at_offset(input_buffer)[0] == '-') || ((buffer_at_offset(input_buffer)[0] >= '0') && (buffer_at_offset(input_buffer)[0] <= '9'))))
    {
        return parse_number(item, input_buffer);
    }
    /* array */
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '['))
    {
        return parse_array(item, input_buffer);
    }
    /* object */
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '{'))
    {
        return parse_object(item, input_buffer);
    }

    return false;
}

/* Render a value to text. */
static cJSON_bool print_value(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output = NULL;

    if ((item == NULL) || (output_buffer == NULL))
    {
        return false;
    }

    switch ((item->type) & 0xFF)
    {
        case cJSON_NULL:
            output = ensure(output_buffer, 5);
            if (output == NULL)
            {
                return false;
            }
            strcpy((char*)output, "null");
            return true;

        case cJSON_False:
            output = ensure(output_buffer, 6);
            if (output == NULL)
            {
                return false;
            }
            strcpy((char*)output, "false");
            return true;

        case cJSON_True:
            output = ensure(output_buffer, 5);
            if (output == NULL)
            {
                return false;
            }
            strcpy((char*)output, "true");
            return true;

        case cJSON_Number:
            return print_number(item, output_buffer);

        case cJSON_Raw:
        {
            size_t raw_length = 0;
            if (item->valuestring == NULL)
            {
                return false;
            }

            raw_length = strlen(item->valuestring) + sizeof("");
            output = ensure(output_buffer, raw_length);
            if (output == NULL)
            {
                return false;
            }
            memcpy(output, item->valuestring, raw_length);
            return true;
        }

        case cJSON_String:
            return print_string(item, output_buffer);

        case cJSON_Array:
            return print_array(item, output_buffer);

        case cJSON_Object:
            return print_object(item, output_buffer);

        default:
            return false;
    }
}

/* Build an array from input text. */
static cJSON_bool parse_array(cJSON * const item, parse_buffer * const input_buffer)
{
    cJSON *head = NULL; /* head of the linked list */
    cJSON *current_item = NULL;

    if (input_buffer->depth >= CJSON_NESTING_LIMIT)
    {
        return false; /* to deeply nested */
    }
    input_buffer->depth++;

    if (buffer_at_offset(input_buffer)[0] != '[')
    {
        /* not an array */
        goto fail;
    }

    input_buffer->offset++;
    buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ']'))
    {
        /* empty array */
        goto success;
    }

    /* check if we skipped to the end of the buffer */
    if (cannot_access_at_index(input_buffer, 0))
    {
        input_buffer->offset--;
        goto fail;
    }

    /* step back to character in front of the first element */
    input_buffer->offset--;
    /* loop through the comma separated array elements */
    do
    {
        /* allocate next item */
        cJSON *new_item = cJSON_New_Item(&(input_buffer->hooks));
        if (new_item == NULL)
        {
            goto fail; /* allocation failure */
        }

        /* attach next item to list */
        if (head == NULL)
        {
            /* start the linked list */
            current_item = head = new_item;
        }
        else
        {
            /* add to the end and advance */
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }

        /* parse next value */
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_value(current_item, input_buffer))
        {
            goto fail; /* failed to parse value */
        }
        buffer_skip_whitespace(input_buffer);
    }
    while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));

    if (cannot_access_at_index(input_buffer, 0) || buffer_at_offset(input_buffer)[0] != ']')
    {
        goto fail; /* expected end of array */
    }

success:
    input_buffer->depth--;

    if (head != NULL) {
        head->prev = current_item;
    }

    item->type = cJSON_Array;
    item->child = head;

    input_buffer->offset++;

    return true;

fail:
    if (head != NULL)
    {
        cJSON_Delete(head);
    }

    return false;
}

/* Render an array to text */
static cJSON_bool print_array(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    size_t length = 0;
    cJSON *current_element = item->child;

    if (output_buffer == NULL)
    {
        return false;
    }

    /* Compose the output array. */
    /* opening square bracket */
    output_pointer = ensure(output_buffer, 1);
    if (output_pointer == NULL)
    {
        return false;
    }

    *output_pointer = '[';
    output_buffer->offset++;
    output_buffer->depth++;

    while (current_element != NULL)
    {
        if (!print_value(current_element, output_buffer))
        {
            return false;
        }
        update_offset(output_buffer);
        if (current_element->next)
        {
            length = (size_t) (output_buffer->format ? 2 : 1);
            output_pointer = ensure(output_buffer, length + 1);
            if (output_pointer == NULL)
            {
                return false;
            }
            *output_pointer++ = ',';
            if(output_buffer->format)
            {
                *output_pointer++ = ' ';
            }
            *output_pointer = '\0';
            output_buffer->offset += length;
        }
        current_element = current_element->next;
    }

    output_pointer = ensure(output_buffer, 2);
    if (output_pointer == NULL)
    {
        return false;
    }
    *output_pointer++ = ']';
    *output_pointer = '\0';
    output_buffer->depth--;

    return true;
}

/* Build an object from the text. */
static cJSON_bool parse_object(cJSON * const item, parse_buffer * const input_buffer)
{
    cJSON *head = NULL; /* linked list head */
    cJSON *current_item = NULL;

    if (input_buffer->depth >= CJSON_NESTING_LIMIT)
    {
        return false; /* to deeply nested */
    }
    input_buffer->depth++;

    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '{'))
    {
        goto fail; /* not an object */
    }

    input_buffer->offset++;
    buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '}'))
    {
        goto success; /* empty object */
    }

    /* check if we skipped to the end of the buffer */
    if (cannot_access_at_index(input_buffer, 0))
    {
        input_buffer->offset--;
        goto fail;
    }

    /* step back to character in front of the first element */
    input_buffer->offset--;
    /* loop through the comma separated array elements */
    do
    {
        /* allocate next item */
        cJSON *new_item = cJSON_New_Item(&(input_buffer->hooks));
        if (new_item == NULL)
        {
            goto fail; /* allocation failure */
        }

        /* attach next item to list */
        if (head == NULL)
        {
            /* start the linked list */
            current_item = head = new_item;
        }
        else
        {
            /* add to the end and advance */
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }

        /* parse the name of the child */
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_string(current_item, input_buffer))
        {
            goto fail; /* failed to parse name */
        }
        buffer_skip_whitespace(input_buffer);

        /* swap valuestring and string, because we parsed the name */
        current_item->string = current_item->valuestring;
        current_item->valuestring = NULL;

        if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != ':'))
        {
            goto fail; /* invalid object */
        }

        /* parse the value */
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_value(current_item, input_buffer))
        {
            goto fail; /* failed to parse value */
        }
        buffer_skip_whitespace(input_buffer);
    }
    while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));

    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '}'))
    {
        goto fail; /* expected end of object */
    }

success:
    input_buffer->depth--;

    if (head != NULL) {
        head->prev = current_item;
    }

    item->type = cJSON_Object;
    item->child = head;

    input_buffer->offset++;
    return true;

fail:
    if (head != NULL)
    {
        cJSON_Delete(head);
    }

    return false;
}

/* Render an object to text. */
static cJSON_bool print_object(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    size_t length = 0;
    cJSON *current_item = item->child;

    if (output_buffer == NULL)
    {
        return false;
    }

    /* Compose the output: */
    length = (size_t) (output_buffer->format ? 2 : 1); /* fmt: {\n */
    output_pointer = ensure(output_buffer, length + 1);
    if (output_pointer == NULL)
    {
        return false;
    }

    *output_pointer++ = '{';
    output_buffer->depth++;
    if (output_buffer->format)
    {
        *output_pointer++ = '\n';
    }
    output_buffer->offset += length;

    while (current_item)
    {
        if (output_buffer->format)
        {
            size_t i;
            output_pointer = ensure(output_buffer, output_buffer->depth);
            if (output_pointer == NULL)
            {
                return false;
            }
            for (i = 0; i < output_buffer->depth; i++)
            {
                *output_pointer++ = '\t';
            }
            output_buffer->offset += output_buffer->depth;
        }

        /* print key */
        if (!print_string_ptr((unsigned char*)current_item->string, output_buffer))
        {
            return false;
        }
        update_offset(output_buffer);

        length = (size_t) (output_buffer->format ? 2 : 1);
        output_pointer = ensure(output_buffer, length);
        if (output_pointer == NULL)
        {
            return false;
        }
        *output_pointer++ = ':';
        if (output_buffer->format)
        {
            *output_pointer++ = '\t';
        }
        output_buffer->offset += length;

        /* print value */
        if (!print_value(current_item, output_buffer))
        {
            return false;
        }
        update_offset(output_buffer);

        /* print comma if not last */
        length = ((size_t)(output_buffer->format ? 1 : 0) + (size_t)(current_item->next ? 1 : 0));
        output_pointer = ensure(output_buffer, length + 1);
        if (output_pointer == NULL)
        {
            return false;
        }
        if (current_item->next)
        {
            *output_pointer++ = ',';
        }

        if (output_buffer->format)
        {
            *output_pointer++ = '\n';
        }
        *output_pointer = '\0';
        output_buffer->offset += length;

        current_item = current_item->next;
    }

    output_pointer = ensure(output_buffer, output_buffer->format ? (output_buffer->depth + 1) : 2);
    if (output_pointer == NULL)
    {
        return false;
    }
    if (output_buffer->format)
    {
        size_t i;
        for (i = 0; i < (output_buffer->depth - 1); i++)
        {
            *output_pointer++ = '\t';
        }
    }
    *output_pointer++ = '}';
    *output_pointer = '\0';
    output_buffer->depth--;

    return true;
}

/* Get Array size/item / object item. */
CJSON_PUBLIC(int) cJSON_GetArraySize(const cJSON *array)
{
    cJSON *child = NULL;
    size_t size = 0;

    if (array == NULL)
    {
        return 0;
    }

    child = array->child;

    while(child != NULL)
    {
        size++;
        child = child->next;
    }

    /* FIXME: Can overflow here. Cannot be fixed without breaking the API */

    return (int)size;
}

static cJSON* get_array_item(const cJSON *array, size_t index)
{
    cJSON *current_child = NULL;

    if (array == NULL)
    {
        return NULL;
    }

    current_child = array->child;
    while ((current_child != NULL) && (index > 0))
    {
        index--;
        current_child = current_child->next;
    }

    return current_child;
}

CJSON_PUBLIC(cJSON *) cJSON_GetArrayItem(const cJSON *array, int index)
{
    if (index < 0)
    {
        return NULL;
    }

    return get_array_item(array, (size_t)index);
}

static cJSON *get_object_item(const cJSON * const object, const char * const name, const cJSON_bool case_sensitive)
{
    cJSON *current_element = NULL;

    if ((object == NULL) || (name == NULL))
    {
        return NULL;
    }

    current_element = object->child;
    if (case_sensitive)
    {
        while ((current_element != NULL) && (current_element->string != NULL) && (strcmp(name, current_element->string) != 0))
        {
            current_element = current_element->next;
        }
    }
    else
    {
        while ((current_element != NULL) && (case_insensitive_strcmp((const unsigned char*)name, (const unsigned char*)(current_element->string)) != 0))
        {
            current_element = current_element->next;
        }
    }

    if ((current_element == NULL) || (current_element->string == NULL)) {
        return NULL;
    }

    return current_element;
}

CJSON_PUBLIC(cJSON *) cJSON_GetObjectItem(const cJSON * const object, const char * const string)
{
    return get_object_item(object, string, false);
}

CJSON_PUBLIC(cJSON *) cJSON_GetObjectItemCaseSensitive(const cJSON * const object, const char * const string)
{
    return get_object_item(object, string, true);
}

CJSON_PUBLIC(cJSON_bool) cJSON_HasObjectItem(const cJSON *object, const char *string)
{
    return cJSON_GetObjectItem(object, string) ? 1 : 0;
}

/* Utility for array list handling. */
static void suffix_object(cJSON *prev, cJSON *item)
{
    prev->next = item;
    item->prev = prev;
}

/* Utility for handling references. */
static cJSON *create_reference(const cJSON *item, const internal_hooks * const hooks)
{
    cJSON *reference = NULL;
    if (item == NULL)
    {
        return NULL;
    }

    reference = cJSON_New_Item(hooks);
    if (reference == NULL)
    {
        return NULL;
    }

    memcpy(reference, item, sizeof(cJSON));
    reference->string = NULL;
    reference->type |= cJSON_IsReference;
    reference->next = reference->prev = NULL;
    return reference;
}

static cJSON_bool add_item_to_array(cJSON *array, cJSON *item)
{
    cJSON *child = NULL;

    if ((item == NULL) || (array == NULL) || (array == item))
    {
        return false;
    }

    child = array->child;
    /*
     * To find the last item in array quickly, we use prev in array
     */
    if (child == NULL)
    {
        /* list is empty, start new one */
        array->child = item;
        item->prev = item;
        item->next = NULL;
    }
    else
    {
        /* append to the end */
        if (child->prev)
        {
            suffix_object(child->prev, item);
            array->child->prev = item;
        }
    }

    return true;
}

/* Add item to array/object. */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToArray(cJSON *array, cJSON *item)
{
    return add_item_to_array(array, item);
}

#if defined(__clang__) || (defined(__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
    #pragma GCC diagnostic push
#endif
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
/* helper function to cast away const */
static void* cast_away_const(const void* string)
{
    return (void*)string;
}
#if defined(__clang__) || (defined(__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
    #pragma GCC diagnostic pop
#endif


static cJSON_bool add_item_to_object(cJSON * const object, const char * const string, cJSON * const item, const internal_hooks * const hooks, const cJSON_bool constant_key)
{
    char *new_key = NULL;
    int new_type = cJSON_Invalid;

    if ((object == NULL) || (string == NULL) || (item == NULL) || (object == item))
    {
        return false;
    }

    if (constant_key)
    {
        new_key = (char*)cast_away_const(string);
        new_type = item->type | cJSON_StringIsConst;
    }
    else
    {
        new_key = (char*)cJSON_strdup((const unsigned char*)string, hooks);
        if (new_key == NULL)
        {
            return false;
        }

        new_type = item->type & ~cJSON_StringIsConst;
    }

    if (!(item->type & cJSON_StringIsConst) && (item->string != NULL))
    {
        hooks->deallocate(item->string);
    }

    item->string = new_key;
    item->type = new_type;

    return add_item_to_array(object, item);
}

CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item)
{
    return add_item_to_object(object, string, item, &global_hooks, false);
}

/* Add an item to an object with constant string as key */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item)
{
    return add_item_to_object(object, string, item, &global_hooks, true);
}

CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item)
{
    if (array == NULL)
    {
        return false;
    }

    return add_item_to_array(array, create_reference(item, &global_hooks));
}

CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item)
{
    if ((object == NULL) || (string == NULL))
    {
        return false;
    }

    return add_item_to_object(object, string, create_reference(item, &global_hooks), &global_hooks, false);
}

CJSON_PUBLIC(cJSON*) cJSON_AddNullToObject(cJSON * const object, const char * const name)
{
    cJSON *null = cJSON_CreateNull();
    if (add_item_to_object(object, name, null, &global_hooks, false))
    {
        return null;
    }

    cJSON_Delete(null);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddTrueToObject(cJSON * const object, const char * const name)
{
    cJSON *true_item = cJSON_CreateTrue();
    if (add_item_to_object(object, name, true_item, &global_hooks, false))
    {
        return true_item;
    }

    cJSON_Delete(true_item);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddFalseToObject(cJSON * const object, const char * const name)
{
    cJSON *false_item = cJSON_CreateFalse();
    if (add_item_to_object(object, name, false_item, &global_hooks, false))
    {
        return false_item;
    }

    cJSON_Delete(false_item);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddBoolToObject(cJSON * const object, const char * const name, const cJSON_bool boolean)
{
    cJSON *bool_item = cJSON_CreateBool(boolean);
    if (add_item_to_object(object, name, bool_item, &global_hooks, false))
    {
        return bool_item;
    }

    cJSON_Delete(bool_item);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddNumberToObject(cJSON * const object, const char * const name, const double number)
{
    cJSON *number_item = cJSON_CreateNumber(number);
    if (add_item_to_object(object, name, number_item, &global_hooks, false))
    {
        return number_item;
    }

    cJSON_Delete(number_item);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string)
{
    cJSON *string_item = cJSON_CreateString(string);
    if (add_item_to_object(object, name, string_item, &global_hooks, false))
    {
        return string_item;
    }

    cJSON_Delete(string_item);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw)
{
    cJSON *raw_item = cJSON_CreateRaw(raw);
    if (add_item_to_object(object, name, raw_item, &global_hooks, false))
    {
        return raw_item;
    }

    cJSON_Delete(raw_item);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddObjectToObject(cJSON * const object, const char * const name)
{
    cJSON *object_item = cJSON_CreateObject();
    if (add_item_to_object(object, name, object_item, &global_hooks, false))
    {
        return object_item;
    }

    cJSON_Delete(object_item);
    return NULL;
}

CJSON_PUBLIC(cJSON*) cJSON_AddArrayToObject(cJSON * const object, const char * const name)
{
    cJSON *array = cJSON_CreateArray();
    if (add_item_to_object(object, name, array, &global_hooks, false))
    {
        return array;
    }

    cJSON_Delete(array);
    return NULL;
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemViaPointer(cJSON *parent, cJSON * const item)
{
    if ((parent == NULL) || (item == NULL))
    {
        return NULL;
    }

    if (item != parent->child)
    {
        /* not the first element */
        item->prev->next = item->next;
    }
    if (item->next != NULL)
    {
        /* not the last element */
        item->next->prev = item->prev;
    }

    if (item == parent->child)
    {
        /* first element */
        parent->child = item->next;
    }
    else if (item->next == NULL)
    {
        /* last element */
        parent->child->prev = item->prev;
    }

    /* make sure the detached item doesn't point anywhere anymore */
    item->prev = NULL;
    item->next = NULL;

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromArray(cJSON *array, int which)
{
    if (which < 0)
    {
        return NULL;
    }

    return cJSON_DetachItemViaPointer(array, get_array_item(array, (size_t)which));
}

CJSON_PUBLIC(void) cJSON_DeleteItemFromArray(cJSON *array, int which)
{
    cJSON_Delete(cJSON_DetachItemFromArray(array, which));
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObject(cJSON *object, const char *string)
{
    cJSON *to_detach = cJSON_GetObjectItem(object, string);

    return cJSON_DetachItemViaPointer(object, to_detach);
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObjectCaseSensitive(cJSON *object, const char *string)
{
    cJSON *to_detach = cJSON_GetObjectItemCaseSensitive(object, string);

    return cJSON_DetachItemViaPointer(object, to_detach);
}

CJSON_PUBLIC(void) cJSON_DeleteItemFromObject(cJSON *object, const char *string)
{
    cJSON_Delete(cJSON_DetachItemFromObject(object, string));
}

CJSON_PUBLIC(void) cJSON_DeleteItemFromObjectCaseSensitive(cJSON *object, const char *string)
{
    cJSON_Delete(cJSON_DetachItemFromObjectCaseSensitive(object, string));
}

/* Replace array/object items with new ones. */
CJSON_PUBLIC(cJSON_bool) cJSON_InsertItemInArray(cJSON *array, int which, cJSON *newitem)
{
    cJSON *after_inserted = NULL;

    if (which < 0 || newitem == NULL)
    {
        return false;
    }

    after_inserted = get_array_item(array, (size_t)which);
    if (after_inserted == NULL)
    {
        return add_item_to_array(array, newitem);
    }

    if (after_inserted != array->child && after_inserted->prev == NULL) {
        /* return false if after_inserted is a corrupted array item */
        return false;
    }

    newitem->next = after_inserted;
    newitem->prev = after_inserted->prev;
    after_inserted->prev = newitem;
    if (after_inserted == array->child)
    {
        array->child = newitem;
    }
    else
    {
        newitem->prev->next = newitem;
    }
    return true;
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemViaPointer(cJSON * const parent, cJSON * const item, cJSON * replacement)
{
    if ((parent == NULL) || (parent->child == NULL) || (replacement == NULL) || (item == NULL))
    {
        return false;
    }

    if (replacement == item)
    {
        return true;
    }

    replacement->next = item->next;
    replacement->prev = item->prev;

    if (replacement->next != NULL)
    {
        replacement->next->prev = replacement;
    }
    if (parent->child == item)
    {
        if (parent->child->prev == parent->child)
        {
            replacement->prev = replacement;
        }
        parent->child = replacement;
    }
    else
    {   /*
         * To find the last item in array quickly, we use prev in array.
         * We can't modify the last item's next pointer where this item was the parent's child
         */
        if (replacement->prev != NULL)
        {
            replacement->prev->next = replacement;
        }
        if (replacement->next == NULL)
        {
            parent->child->prev = replacement;
        }
    }

    item->next = NULL;
    item->prev = NULL;
    cJSON_Delete(item);

    return true;
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem)
{
    if (which < 0)
    {
        return false;
    }

    return cJSON_ReplaceItemViaPointer(array, get_array_item(array, (size_t)which), newitem);
}

static cJSON_bool replace_item_in_object(cJSON *object, const char *string, cJSON *replacement, cJSON_bool case_sensitive)
{
    if ((replacement == NULL) || (string == NULL))
    {
        return false;
    }

    /* replace the name in the replacement */
    if (!(replacement->type & cJSON_StringIsConst) && (replacement->string != NULL))
    {
        cJSON_free(replacement->string);
    }
    replacement->string = (char*)cJSON_strdup((const unsigned char*)string, &global_hooks);
    if (replacement->string == NULL)
    {
        return false;
    }

    replacement->type &= ~cJSON_StringIsConst;

    return cJSON_ReplaceItemViaPointer(object, get_object_item(object, string, case_sensitive), replacement);
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem)
{
    return replace_item_in_object(object, string, newitem, false);
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObjectCaseSensitive(cJSON *object, const char *string, cJSON *newitem)
{
    return replace_item_in_object(object, string, newitem, true);
}

/* Create basic types: */
CJSON_PUBLIC(cJSON *) cJSON_CreateNull(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_NULL;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateTrue(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_True;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateFalse(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_False;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateBool(cJSON_bool boolean)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = boolean ? cJSON_True : cJSON_False;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateNumber(double num)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_Number;
        item->valuedouble = num;

        /* use saturation in case of overflow */
        if (num >= INT_MAX)
        {
            item->valueint = INT_MAX;
        }
        else if (num <= (double)INT_MIN)
        {
            item->valueint = INT_MIN;
        }
        else
        {
            item->valueint = (int)num;
        }
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateString(const char *string)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_String;
        item->valuestring = (char*)cJSON_strdup((const unsigned char*)string, &global_hooks);
        if(!item->valuestring)
        {
            cJSON_Delete(item);
            return NULL;
        }
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateStringReference(const char *string)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item != NULL)
    {
        item->type = cJSON_String | cJSON_IsReference;
        item->valuestring = (char*)cast_away_const(string);
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateObjectReference(const cJSON *child)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item != NULL) {
        item->type = cJSON_Object | cJSON_IsReference;
        item->child = (cJSON*)cast_away_const(child);
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateArrayReference(const cJSON *child) {
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item != NULL) {
        item->type = cJSON_Array | cJSON_IsReference;
        item->child = (cJSON*)cast_away_const(child);
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateRaw(const char *raw)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_Raw;
        item->valuestring = (char*)cJSON_strdup((const unsigned char*)raw, &global_hooks);
        if(!item->valuestring)
        {
            cJSON_Delete(item);
            return NULL;
        }
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateArray(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type=cJSON_Array;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateObject(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item)
    {
        item->type = cJSON_Object;
    }

    return item;
}

/* Create Arrays: */
CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber(numbers[i]);
        if (!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateFloatArray(const float *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber((double)numbers[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateDoubleArray(const double *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber(numbers[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateStringArray(const char *const *strings, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (strings == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for (i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateString(strings[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p,n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

/* Duplication */
CJSON_PUBLIC(cJSON *) cJSON_Duplicate(const cJSON *item, cJSON_bool recurse)
{
    cJSON *newitem = NULL;
    cJSON *child = NULL;
    cJSON *next = NULL;
    cJSON *newchild = NULL;

    /* Bail on bad ptr */
    if (!item)
    {
        goto fail;
    }
    /* Create new item */
    newitem = cJSON_New_Item(&global_hooks);
    if (!newitem)
    {
        goto fail;
    }
    /* Copy over all vars */
    newitem->type = item->type & (~cJSON_IsReference);
    newitem->valueint = item->valueint;
    newitem->valuedouble = item->valuedouble;
    if (item->valuestring)
    {
        newitem->valuestring = (char*)cJSON_strdup((unsigned char*)item->valuestring, &global_hooks);
        if (!newitem->valuestring)
        {
            goto fail;
        }
    }
    if (item->string)
    {
        newitem->string = (item->type&cJSON_StringIsConst) ? item->string : (char*)cJSON_strdup((unsigned char*)item->string, &global_hooks);
        if (!newitem->string)
        {
            goto fail;
        }
    }
    /* If non-recursive, then we're done! */
    if (!recurse)
    {
        return newitem;
    }
    /* Walk the ->next chain for the child. */
    child = item->child;
    while (child != NULL)
    {
        newchild = cJSON_Duplicate(child, true); /* Duplicate (with recurse) each item in the ->next chain */
        if (!newchild)
        {
            goto fail;
        }
        if (next != NULL)
        {
            /* If newitem->child already set, then crosswire ->prev and ->next and move on */
            next->next = newchild;
            newchild->prev = next;
            next = newchild;
        }
        else
        {
            /* Set newitem->child and move to it */
            newitem->child = newchild;
            next = newchild;
        }
        child = child->next;
    }
    if (newitem && newitem->child)
    {
        newitem->child->prev = newchild;
    }

    return newitem;

fail:
    if (newitem != NULL)
    {
        cJSON_Delete(newitem);
    }

    return NULL;
}

static void skip_oneline_comment(char **input)
{
    *input += static_strlen("//");

    for (; (*input)[0] != '\0'; ++(*input))
    {
        if ((*input)[0] == '\n') {
            *input += static_strlen("\n");
            return;
        }
    }
}

static void skip_multiline_comment(char **input)
{
    *input += static_strlen("/*");

    for (; (*input)[0] != '\0'; ++(*input))
    {
        if (((*input)[0] == '*') && ((*input)[1] == '/'))
        {
            *input += static_strlen("*/");
            return;
        }
    }
}

static void minify_string(char **input, char **output) {
    (*output)[0] = (*input)[0];
    *input += static_strlen("\"");
    *output += static_strlen("\"");


    for (; (*input)[0] != '\0'; (void)++(*input), ++(*output)) {
        (*output)[0] = (*input)[0];

        if ((*input)[0] == '\"') {
            (*output)[0] = '\"';
            *input += static_strlen("\"");
            *output += static_strlen("\"");
            return;
        } else if (((*input)[0] == '\\') && ((*input)[1] == '\"')) {
            (*output)[1] = (*input)[1];
            *input += static_strlen("\"");
            *output += static_strlen("\"");
        }
    }
}

CJSON_PUBLIC(void) cJSON_Minify(char *json)
{
    char *into = json;

    if (json == NULL)
    {
        return;
    }

    while (json[0] != '\0')
    {
        switch (json[0])
        {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                json++;
                break;

            case '/':
                if (json[1] == '/')
                {
                    skip_oneline_comment(&json);
                }
                else if (json[1] == '*')
                {
                    skip_multiline_comment(&json);
                } else {
                    json++;
                }
                break;

            case '\"':
                minify_string(&json, (char**)&into);
                break;

            default:
                into[0] = json[0];
                json++;
                into++;
        }
    }

    /* and null-terminate. */
    *into = '\0';
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsInvalid(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Invalid;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsFalse(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_False;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsTrue(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xff) == cJSON_True;
}


CJSON_PUBLIC(cJSON_bool) cJSON_IsBool(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & (cJSON_True | cJSON_False)) != 0;
}
CJSON_PUBLIC(cJSON_bool) cJSON_IsNull(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_NULL;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsNumber(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Number;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsString(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_String;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsArray(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Array;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsObject(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Object;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsRaw(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Raw;
}

CJSON_PUBLIC(cJSON_bool) cJSON_Compare(const cJSON * const a, const cJSON * const b, const cJSON_bool case_sensitive)
{
    if ((a == NULL) || (b == NULL) || ((a->type & 0xFF) != (b->type & 0xFF)))
    {
        return false;
    }

    /* check if type is valid */
    switch (a->type & 0xFF)
    {
        case cJSON_False:
        case cJSON_True:
        case cJSON_NULL:
        case cJSON_Number:
        case cJSON_String:
        case cJSON_Raw:
        case cJSON_Array:
        case cJSON_Object:
            break;

        default:
            return false;
    }

    /* identical objects are equal */
    if (a == b)
    {
        return true;
    }

    switch (a->type & 0xFF)
    {
        /* in these cases and equal type is enough */
        case cJSON_False:
        case cJSON_True:
        case cJSON_NULL:
            return true;

        case cJSON_Number:
            if (compare_double(a->valuedouble, b->valuedouble))
            {
                return true;
            }
            return false;

        case cJSON_String:
        case cJSON_Raw:
            if ((a->valuestring == NULL) || (b->valuestring == NULL))
            {
                return false;
            }
            if (strcmp(a->valuestring, b->valuestring) == 0)
            {
                return true;
            }

            return false;

        case cJSON_Array:
        {
            cJSON *a_element = a->child;
            cJSON *b_element = b->child;

            for (; (a_element != NULL) && (b_element != NULL);)
            {
                if (!cJSON_Compare(a_element, b_element, case_sensitive))
                {
                    return false;
                }

                a_element = a_element->next;
                b_element = b_element->next;
            }

            /* one of the arrays is longer than the other */
            if (a_element != b_element) {
                return false;
            }

            return true;
        }

        case cJSON_Object:
        {
            cJSON *a_element = NULL;
            cJSON *b_element = NULL;
            cJSON_ArrayForEach(a_element, a)
            {
                /* TODO This has O(n^2) runtime, which is horrible! */
                b_element = get_object_item(b, a_element->string, case_sensitive);
                if (b_element == NULL)
                {
                    return false;
                }

                if (!cJSON_Compare(a_element, b_element, case_sensitive))
                {
                    return false;
                }
            }

            /* doing this twice, once on a and b to prevent true comparison if a subset of b
             * TODO: Do this the proper way, this is just a fix for now */
            cJSON_ArrayForEach(b_element, b)
            {
                a_element = get_object_item(a, b_element->string, case_sensitive);
                if (a_element == NULL)
                {
                    return false;
                }

                if (!cJSON_Compare(b_element, a_element, case_sensitive))
                {
                    return false;
                }
            }

            return true;
        }

        default:
            return false;
    }
}

CJSON_PUBLIC(void *) cJSON_malloc(size_t size)
{
    return global_hooks.allocate(size);
}

CJSON_PUBLIC(void) cJSON_free(void *object)
{
    global_hooks.deallocate(object);
}
#define cJSON__h
#endif






char *private_cxpathjson_read_file(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);


    char *buffer = (char *)malloc(file_size + 1);


    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}

int  private_cxpathjson_write_file(const char *filename, const char *value) {

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return CXPATHJSON_IO_PROBLEM_CODE;
    }
    fputs(value, file);

    fclose(file);
    return CXPATHJSON_OK_CODE;
}







int private_cxpathjson_get_json_type(cJSON *element){
    if(!element){
        return CXPATHJSON_NOT_FOUND;
    }

    if(cJSON_IsNull(element)){
        return CXPATHJSON_NULL;
    }

    if(cJSON_IsBool(element)){
        return CXPATHJSON_BOOL;
    }

    if(cJSON_IsNumber(element)){
        return CXPATHJSON_NUMBER;
    }

    if(cJSON_IsString(element)){
        return CXPATHJSON_STRING;
    }

    if(cJSON_IsObject(element)){
        return CXPATHJSON_OBJECT;
    }

    if(cJSON_IsArray(element)){
        return CXPATHJSON_ARRAY;
    }
    return CXPATHJSON_NOT_FOUND;
}


const char * private_cxpathjson_convert_json_type_to_str(cJSON *element){


    if(!element){
        return CXPATHJSON_NOT_FOUND_TEXT;
    }

    if(cJSON_IsNull(element)){
        return CXPATHJSON_NULL_TEXT;
    }

    if(cJSON_IsBool(element)){
        return CXPATHJSON_BOOL_TEXT;
    }

    if(cJSON_IsNumber(element)){
        return CXPATHJSON_NUMBER_TEXT;
    }

    if(cJSON_IsString(element)){
        return CXPATHJSON_STRING_TEXT;
    }

    if(cJSON_IsObject(element)){
        return CXPATHJSON_OBJECT_TEXT;
    }

    if(cJSON_IsArray(element)){
        return CXPATHJSON_ARRAY_TEXT;
    }
    return CXPATHJSON_NOT_FOUND_TEXT;
}





int CxpathJson_type(CxpathJson * self, const char *format, ...){

    if(CxpathJson_get_error_code(self)){
        return CXPATHJSON_NOT_FOUND;
    }

    va_list args;
    va_start(args, format);
    bool old_raise_conf = self->raise_runtime_errors;
    self->raise_runtime_errors = false;
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs( self,format, args);
    self->raise_runtime_errors = old_raise_conf;

    va_end(args);
    return private_cxpathjson_get_json_type(result);
}

bool CxpathJson_exist(CxpathJson * self, const char *format, ...){

    if(CxpathJson_get_error_code(self)){
        return CXPATHJSON_NOT_FOUND;
    }

    va_list args;
    va_start(args, format);
    bool old_raise_conf = self->raise_runtime_errors;

    self->raise_runtime_errors = false;
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs( self,format, args);
    self->raise_runtime_errors = old_raise_conf;

    va_end(args);
    return (bool)private_cxpathjson_get_json_type(result);
}

bool CxpathJson_its_not_null(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return CXPATHJSON_NOT_FOUND;
    }

    va_list args;
    va_start(args, format);
    bool old_raise_conf = self->raise_runtime_errors;

    self->raise_runtime_errors = false;
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs( self,format, args);
    self->raise_runtime_errors = old_raise_conf;

    va_end(args);
    int generated_type = private_cxpathjson_get_json_type(result);
    if(generated_type == CXPATHJSON_NULL){
        return  false;
    }

    if(generated_type == CXPATHJSON_NOT_FOUND){
        return  false;
    }

    return  true;
}



const char * CxpathJson_type_str(CxpathJson * self, const char *format, ...){

    if(CxpathJson_get_error_code(self)){
        return CXPATHJSON_NOT_FOUND_TEXT;
    }

    va_list args;
    va_start(args, format);
    bool old_raise_conf = self->raise_runtime_errors;
    self->raise_runtime_errors = false;
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs( self,format, args);
    self->raise_runtime_errors = old_raise_conf;
    va_end(args);
    return private_cxpathjson_convert_json_type_to_str(result);
}
bool CxpathJson_its_iterable(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return CXPATHJSON_NOT_FOUND;
    }

    va_list args;
    va_start(args, format);
    bool old_raise_conf = self->raise_runtime_errors;

    self->raise_runtime_errors = false;
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs( self,format, args);
    self->raise_runtime_errors = old_raise_conf;

    va_end(args);
    int generated_type = private_cxpathjson_get_json_type(result);
    if(generated_type == CXPATHJSON_ARRAY){
        return  true;
    }

    if(generated_type == CXPATHJSON_OBJECT){
        return  true;
    }

    return  false;
}



CxpathJson * private_newCxpathJson(){
    CxpathJson  *self = (CxpathJson*) malloc(sizeof (CxpathJson));
    *self = (CxpathJson){0};
    self->childs = (struct CxpathJson **) (CxpathJson **) malloc(0);
    self->raise_runtime_errors = true;
    return self;
}

CxpathJson * private_CxpathJson_get_root(CxpathJson *self){
    if(!self->private_root){
        return self;
    }
    return (CxpathJson *) self->private_root;
}

CxpathJson * private_CxpathJson_construct_child(CxpathJson  *self,cJSON *element){
    CxpathJson  *created = newCxpathJson_from_cJSON_getting_ownership(element);
    created->private_root = (struct CxpathJson *) private_CxpathJson_get_root(self);
    self->childs = (struct CxpathJson **) realloc(
            self->childs,
            (self->size +1) * sizeof(CxpathJson)
             );
    self->childs[self->size] = (struct CxpathJson *) created;
    self->size+=1;
    return created;
}


CxpathJson * newCxpathJson_from_cJSON_getting_ownership(cJSON *element){
    CxpathJson  *self = private_newCxpathJson();
    self->element = element;
    return  self;
}

CxpathJson * newCxpathJsonObject(){
    cJSON *created = cJSON_CreateObject();
    return newCxpathJson_from_cJSON_getting_ownership(created);
}

CxpathJson * newCxpathJsonArray(){
    cJSON *created = cJSON_CreateArray();
    return newCxpathJson_from_cJSON_getting_ownership(created);
}

CxpathJson * newCxpathJson_from_string( const char *data){
    CxpathJson  *self = private_newCxpathJson();
    self->element =  cJSON_Parse(data);
    if(!self->element){
        CxpathJson_raise_errror(
                self,
                CXPATHJSON_STRING_NOT_PARSABLE_CODE,
                NULL,
                PRIVATE_CXPATHJSON_STRING_NOT_PARSABLE_MESSAGE,
                data
                );
    }
    return  self;
}

CxpathJson * newCxpathJson_from_file(const char *path){

    char *content = private_cxpathjson_read_file(path);
    if(!content){
        CxpathJson  *self = private_newCxpathJson();
        CxpathJson_raise_errror(
                self,
                CXPATHJSON_FILE_NOT_FOUND_CODE,
                NULL,
                PRIVATE_CXPATHJSON_FILE_NOT_FOUND_MESSAGE,
                path
                );
        return self;
    }

    CxpathJson *result = newCxpathJson_from_string(content);
    free(content);
    return result;
}

void CxpathJson_free(CxpathJson * self){
    //means its root element
    if(!self->private_root){
        CxpathJson_clear_errors(self);
        if(self->element){
            cJSON_Delete(self->element);
        }
    }

    for(int i = 0; i < self->size; i++){
        struct CxpathJson  *current_child = self->childs[i];
        CxpathJson_free((CxpathJson *) current_child);
    }
    free(self->childs);

    free(self);
}



void CxpathJson_raise_errror(CxpathJson * self, int error_code, cJSON *path, const char *format, ...){
    if(self->error_code){
        return;
    }
    self->error_code = error_code;
    if(path){
        self->error_path_str = cJSON_PrintUnformatted(path);
        self->path_list = cJSON_Duplicate(path, true);
    }

    va_list args;
    va_start(args, format);
    self->error_message = (char*)malloc(2000 * sizeof (char));
    vsnprintf(self->error_message, 2000 * sizeof (char), format, args);
    va_end(args);

}

int CxpathJson_get_error_code(CxpathJson * self){

    if(!self){
        return CXPATHJSON_NOT_FOUND;
    }
    CxpathJson  *root = private_CxpathJson_get_root(self);
    return root->error_code;
}


bool CxpathJson_has_errors(CxpathJson * self){
    return (bool) CxpathJson_get_error_code(self);
}

bool CxpathJson_is_ok(CxpathJson * self){
    if(CxpathJson_has_errors(self)){
        return false;
    }
    return true;
}
char *  CxpathJson_get_error_message(CxpathJson * self){
    if(!self->error_code){
        return NULL;
    }
    return self->error_message;



}


void CxpathJson_clear_errors(CxpathJson * self){

    if(self->error_message){
        free(self->error_message);
    }

    if(self->path_list){
        cJSON_Delete(self->path_list);
    }

    if(self->error_path_str){
        free(self->error_path_str);
    }



    self->error_code = CXPATHJSON_OK_CODE;
}
char * CxpathJson_get_error_path(CxpathJson * self){
    return  self->error_path_str;
}


cJSON * private_CxpathJson_cJSON_by_cjson_path_list(CxpathJson * self, cJSON *path_list){
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }

    cJSON *current_element = self->element;
    int path_size = cJSON_GetArraySize(path_list);
    for(int i = 0;i <path_size;i++){

        if(!current_element){
            if(self->raise_runtime_errors){
                CxpathJson  *root = private_CxpathJson_get_root(self);
                CxpathJson_raise_errror(
                        root,
                        CXPATHJSON_ELEMENT_PATH_NOT_EXIST_CODE,
                        path_list,
                        PRIVATE_CXPATHJSON_ELEMENT_PATH_NOT_EXIST_MESSAGE
                );
            }

            return  NULL;
        }

        bool current_its_object = cJSON_IsObject(current_element);
        bool current_its_terable = cJSON_IsArray(current_element) || current_its_object;

        if(!current_its_terable){

            if(self->raise_runtime_errors){
                CxpathJson  *root = private_CxpathJson_get_root(self);

                CxpathJson_raise_errror(
                        root,
                        CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ITERABLE_CODE,
                        path_list,
                        PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ITERABLE_MESSAGE
                );
            }

            return  NULL;
        }

        cJSON *current_path = cJSON_GetArrayItem(path_list,i);

        if(cJSON_IsString(current_path) && !current_its_object){
            if(self->raise_runtime_errors){
                CxpathJson  *root = private_CxpathJson_get_root(self);
                CxpathJson_raise_errror(
                        root,
                        CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_OBJECT_CODE,
                        path_list,
                        PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_OBJECT_MESSAGE
                );
            }
            return  NULL;
        }


        if(cJSON_IsString(current_path)){
            current_element = cJSON_GetObjectItem(current_element,current_path->valuestring);
        }


        if(cJSON_IsNumber(current_path)){
            int index = private_cxpathjson_convert_index(
                    current_path->valueint,
                    cJSON_GetArraySize(current_element));
            current_element = cJSON_GetArrayItem(current_element,index);
        }

    }

    if(!current_element){
        if(self->raise_runtime_errors){
            CxpathJson  *root = private_CxpathJson_get_root(self);
            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_PATH_NOT_EXIST_CODE,
                    path_list,
                    PRIVATE_CXPATHJSON_ELEMENT_PATH_NOT_EXIST_MESSAGE
            );
        }

        return  NULL;
    }

    return current_element;
}

cJSON * private_CxpathJson_get_cJSON_by_vargs(CxpathJson * self, const char *format, va_list args){
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }
    char buffer[2000] = {0};
    vsnprintf(buffer, sizeof(buffer), format, args);
    private_cxpathjson_replace_comas(buffer);
    cJSON *parsed_path  = cJSON_Parse(buffer);

    if(private_cxpathjson_validate_path_read_only(parsed_path)){
        //we raise here beacause bad formatting its consider a comptime error
        CxpathJson  *root = private_CxpathJson_get_root(self);
        CxpathJson_raise_errror(root,
                                CXPATHJSON_ARG_PATH_NOT_VALID_CODE,
                                NULL,
                                PRIVATE_CXPATHJSON_ARG_PATH_NOT_VALID_MESSAGE,
                                buffer
        );

        cJSON_Delete(parsed_path);
        return  NULL;
    }
    cJSON *result = private_CxpathJson_cJSON_by_cjson_path_list(self, parsed_path);
    cJSON_Delete(parsed_path);
    return result;

}





cJSON *CxpathJson_get_cJSON(CxpathJson  *self, const char *format, ...) {

    if(CxpathJson_get_error_code(self)){
        return NULL;
    }
    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs( self, format, args);
    va_end(args);
    return  result;
}


CxpathJson  * CxpathJson_get_object(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }

    if(!cJSON_IsObject(result)){
        if(self->raise_runtime_errors){
            char buffer[2000] = {0};
            vsnprintf(buffer, sizeof(buffer), format, args);
            private_cxpathjson_replace_comas(buffer);
            cJSON *parsed_path  = cJSON_Parse(buffer);
            CxpathJson  *root = private_CxpathJson_get_root(self);

            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE,
                    parsed_path,
                    PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE,
                    private_cxpathjson_convert_json_type_to_str(result),
                    CXPATHJSON_OBJECT_TEXT
            );
            cJSON_Delete(parsed_path);
        }


        return  NULL;
    }

    return private_CxpathJson_construct_child(self,result);
}

CxpathJson  * CxpathJson_get_array(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }
    if(!cJSON_IsArray(result)){
        if(self->raise_runtime_errors){
            char buffer[2000] = {0};
            vsnprintf(buffer, sizeof(buffer), format, args);
            private_cxpathjson_replace_comas(buffer);
            cJSON *parsed_path  = cJSON_Parse(buffer);
            CxpathJson  *root = private_CxpathJson_get_root(self);

            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE,
                    parsed_path,
                    PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE,
                    private_cxpathjson_convert_json_type_to_str(result),
                    CXPATHJSON_ARRAY_TEXT
            );
            cJSON_Delete(parsed_path);
        }


        return  NULL;
    }

    return private_CxpathJson_construct_child(self,result);
}


 char * CxpathJson_get_str(CxpathJson *self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }

    if(!cJSON_IsString(result)){
        if(self->raise_runtime_errors){
            char buffer[2000] = {0};
            vsnprintf(buffer, sizeof(buffer), format, args);
            private_cxpathjson_replace_comas(buffer);
            cJSON *parsed_path  = cJSON_Parse(buffer);
            CxpathJson  *root = private_CxpathJson_get_root(self);

            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE,
                    parsed_path,
                    PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE,
                    private_cxpathjson_convert_json_type_to_str(result),
                    CXPATHJSON_STRING_TEXT
            );
            cJSON_Delete(parsed_path);

        }

        return  NULL;
    }
    return  result->valuestring;
}
char * CxpathJson_get_key(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);
    if(CxpathJson_get_error_code(self)){
        return NULL;
    }
    char *key =  result->string;
    if(!key && self->raise_runtime_errors){
        char buffer[2000] = {0};
        vsnprintf(buffer, sizeof(buffer), format, args);
        private_cxpathjson_replace_comas(buffer);
        cJSON *parsed_path  = cJSON_Parse(buffer);
        CxpathJson  *root = private_CxpathJson_get_root(self);

        CxpathJson_raise_errror(
                root,
                XPATH_JSON_ELEMENT_NOT_HAVE_KEY_CODE,
                parsed_path,
                XPATH_JSON_ELEMENT_NOT_HAVE_KEY_MESSAGE
        );
        cJSON_Delete(parsed_path);

    }
    return key;
}
double CxpathJson_get_double(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);

    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    if(!cJSON_IsNumber(result)){
        if(self->raise_runtime_errors){
            char buffer[2000] = {0};
            vsnprintf(buffer, sizeof(buffer), format, args);
            private_cxpathjson_replace_comas(buffer);
            cJSON *parsed_path  = cJSON_Parse(buffer);
            CxpathJson  *root = private_CxpathJson_get_root(self);

            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE,
                    parsed_path,
                    PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE,
                    private_cxpathjson_convert_json_type_to_str(result),
                    CXPATHJSON_NUMBER_TEXT
            );
            cJSON_Delete(parsed_path);
        }
        return CXPATH_ERROR_NUM_RETURN;
    }
    return  result->valuedouble;
}

int CxpathJson_get_int(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);

    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    if(!cJSON_IsNumber(result)){

        if(self->raise_runtime_errors){
            char buffer[2000] = {0};
            vsnprintf(buffer, sizeof(buffer), format, args);
            private_cxpathjson_replace_comas(buffer);

            cJSON *parsed_path  = cJSON_Parse(buffer);
            CxpathJson  *root = private_CxpathJson_get_root(self);
            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE,
                    parsed_path,
                    PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE,
                    private_cxpathjson_convert_json_type_to_str(result),
                    CXPATHJSON_NUMBER_TEXT
            );
            cJSON_Delete(parsed_path);

        }

        return CXPATH_ERROR_NUM_RETURN;
    }
    return  result->valueint;
}

bool CxpathJson_get_bool(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);

    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    if(!cJSON_IsBool(result)){

        if(self->raise_runtime_errors){
            char buffer[2000] = {0};
            vsnprintf(buffer, sizeof(buffer), format, args);
            private_cxpathjson_replace_comas(buffer);

            cJSON *parsed_path  = cJSON_Parse(buffer);
            CxpathJson  *root = private_CxpathJson_get_root(self);
            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE,
                    parsed_path,
                    PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE,
                    private_cxpathjson_convert_json_type_to_str(result),
                    CXPATHJSON_BOOL_TEXT
            );
            cJSON_Delete(parsed_path);

        }

        return CXPATH_ERROR_NUM_RETURN;
    }
    return  (bool)result->valueint;
}

int CxpathJson_get_size(CxpathJson * self, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    va_list args;
    va_start(args, format);
    cJSON *result = private_CxpathJson_get_cJSON_by_vargs(self, format, args);
    va_end(args);

    if(CxpathJson_get_error_code(self)){
        return CXPATH_ERROR_NUM_RETURN;
    }

    if(!cJSON_IsArray(result) && !cJSON_IsObject(result)){
        if(self->raise_runtime_errors){
            char buffer[2000] = {0};
            vsnprintf(buffer, sizeof(buffer), format, args);
            private_cxpathjson_replace_comas(buffer);

            cJSON *parsed_path  = cJSON_Parse(buffer);
            CxpathJson  *root = private_CxpathJson_get_root(self);
            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_CODE,
                    parsed_path,
                    PRIVATE_CXPATHJSON_ELEMENT_HAS_WRONG_TYPE_MESSAGE,
                    private_cxpathjson_convert_json_type_to_str(result),
                    CXPATHJSON_ARRAY_TEXT

            );
            cJSON_Delete(parsed_path);

        }

        return CXPATH_ERROR_NUM_RETURN;
    }
    return cJSON_GetArraySize(result);
}


int private_CxpathJson_verifiy_if_insertion_is_possible(CxpathJson *self, cJSON *path_list){

    int path_size = cJSON_GetArraySize(path_list);

    if(path_size == 0){

        if(self->raise_runtime_errors){
            char *dumped = cJSON_Print(path_list);
            CxpathJson  *root = private_CxpathJson_get_root(self);
            CxpathJson_raise_errror(
                    root,
                    CXPATHJSON_ARG_PATH_NOT_VALID_CODE,
                    path_list,
                    PRIVATE_CXPATHJSON_ARG_PATH_NOT_VALID_MESSAGE,
                    dumped
            );
            free(dumped);

        }
        return CXPATHJSON_GENERIC_ERROR;
    }

    cJSON *current_element = self->element;
    for(int i = 0;i <path_size;i++){


        if(!current_element){
            return  CXPATHJSON_OK_CODE;
        }

        cJSON *current_path = cJSON_GetArrayItem(path_list,i);
        bool is_append = private_cxpathjson_path_is_append(current_path);

        bool current_its_object = cJSON_IsObject(current_element);
        bool current_is_an_array = cJSON_IsArray(current_element);
        bool current_its_iterable = cJSON_IsArray(current_element) || current_its_object;

        bool path_must_be_an_object = cJSON_IsString(current_path) && !is_append;
        bool path_must_be_an_array = cJSON_IsNumber(current_path) || is_append;


        if(current_its_iterable == false) {
            if(self->raise_runtime_errors){
                CxpathJson  *root = private_CxpathJson_get_root(self);

                CxpathJson_raise_errror(
                        root,
                        CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ITERABLE_CODE,
                        path_list,
                        PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ITERABLE_MESSAGE
                );
            }
            return  CXPATHJSON_GENERIC_ERROR;
        }

        if(path_must_be_an_object && current_its_object == false){

            if(self->raise_runtime_errors){
                CxpathJson  *root = private_CxpathJson_get_root(self);

                CxpathJson_raise_errror(
                        root,
                        CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_OBJECT_CODE,
                        path_list,
                        PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_OBJECT_MESSAGE
                );
            }

            return CXPATHJSON_GENERIC_ERROR;

        }

        if(path_must_be_an_array && current_is_an_array == false){

            if(self->raise_runtime_errors){
                CxpathJson  *root = private_CxpathJson_get_root(self);

                CxpathJson_raise_errror(
                        root,
                        CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ARRAY_CODE,
                        path_list,
                        PRIVATE_CXPATHJSON_MIDDLE_ELEMENT_ITS_NOT_ARRAY_MESSAGE
                );
            }

            return  CXPATHJSON_GENERIC_ERROR;
        }

        if(is_append){
            return  CXPATHJSON_OK_CODE;
        }

        if(current_its_object){
            const char *key = current_path->valuestring;
            current_element = cJSON_GetObjectItem(current_element,key);
        }


        if(current_is_an_array){
            int index = private_cxpathjson_convert_index(
                    current_path->valueint,
                    cJSON_GetArraySize(current_element)
            );
            current_element = cJSON_GetArrayItem(current_element,index);
            if(!current_element){
                //for array explict possitions its required

                if(self->raise_runtime_errors){
                    CxpathJson  *root = private_CxpathJson_get_root(self);

                    CxpathJson_raise_errror(
                            root,
                            CXPATHJSON_ELEMENT_PATH_NOT_EXIST_CODE,
                    path_list,
                            PRIVATE_CXPATHJSON_ELEMENT_PATH_NOT_EXIST_MESSAGE
                    );
                }

                return  CXPATHJSON_GENERIC_ERROR;
            }
        }
    }
    return CXPATHJSON_OK_CODE;


}



void CxpathJson_set_cjson_getting_ownership(CxpathJson *self, cJSON *value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        cJSON_Delete(value);
        return;
    }

    va_list args;
    va_start(args, format);
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value, format, args);
    va_end(args);

    if(CxpathJson_get_error_code(self)){
        cJSON_Delete(value);
        return;
    }

}

void CxpathJson_set_xpathJson_getting_onwership(CxpathJson *self, CxpathJson *value, const char *format, ...){

    if(CxpathJson_get_error_code(self)){
        CxpathJson_free(value);
        return;
    }
    va_list args;
    va_start(args, format);
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value->element, format, args);
    va_end(args);
    value->element = NULL;
     CxpathJson_free(value);
}

void CxpathJson_set_xpath_by_reference(CxpathJson *self, CxpathJson *value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }
    va_list args;
    va_start(args, format);
    cJSON *copy = cJSON_Duplicate(value->element,true);
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, copy, format, args);
    va_end(args);
}


void CxpathJson_set_str_getting_ownership(CxpathJson *self,  char *value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_New_Item(&global_hooks);
    value_cjson->type = cJSON_String;
    value_cjson->valuestring = value;
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value_cjson, format, args);
    va_end(args);


}

void CxpathJson_set_str(CxpathJson *self, const char *value, const char *format, ...){

    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateString(value);
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value_cjson, format, args);
    va_end(args);

}

void CxpathJson_set_double(CxpathJson *self, double value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateNumber(value);
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value_cjson, format, args);
    va_end(args);


}

void CxpathJson_set_int(CxpathJson *self, int value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateNumber((double )value);
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value_cjson, format, args);
    va_end(args);


}

void CxpathJson_set_bool(CxpathJson *self, bool value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateBool(value);
    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value_cjson, format, args);
    va_end(args);


}



void private_cxpathjson_set_cjson_by_path_list(CxpathJson *self, cJSON *value, cJSON *path_list) {

    if(CxpathJson_get_error_code(self)){
        return;
    }

    if(private_CxpathJson_verifiy_if_insertion_is_possible(self, path_list)){
        return;
    }


    cJSON *last_element = self->element;
    int path_size = cJSON_GetArraySize(path_list);

    for(int i = 0;i <path_size-1;i++){


        cJSON *current_path = cJSON_GetArrayItem(path_list,i);


        bool is_append = private_cxpathjson_path_is_append(current_path);
        bool path_must_be_an_object = cJSON_IsString(current_path);
        bool path_must_be_an_array = cJSON_IsNumber(current_path);
        cJSON *possible_current_element = NULL;

        if(path_must_be_an_object && !is_append){
            possible_current_element = cJSON_GetObjectItem(last_element, current_path->valuestring);
        }


        if(path_must_be_an_array && !is_append){
            int index = private_cxpathjson_convert_index(
                    current_path->valueint,
                    cJSON_GetArraySize(last_element)
            );
            possible_current_element = cJSON_GetArrayItem(last_element, index);
        }


        if(possible_current_element){
            last_element = possible_current_element;
            continue;
        }

        cJSON *created_element  = NULL;

        cJSON *next_path = cJSON_GetArrayItem(path_list,i+1);

        //Creating the element
        if(private_cxpathjson_path_is_append(next_path)){
            created_element = cJSON_CreateArray();
        }

        else if(cJSON_IsNumber(next_path) ){
            created_element = cJSON_CreateArray();
        }

        else if(cJSON_IsString(next_path)) {
            created_element = cJSON_CreateObject();
        }




        if(cJSON_IsObject(last_element)){
            cJSON_AddItemToObject(last_element, current_path->valuestring, created_element);
        }
            //inserting the created element in current
        else{
            cJSON_AddItemToArray(last_element, created_element);
        }
        last_element = created_element;

    }

    cJSON *last_path = cJSON_GetArrayItem(path_list,path_size-1);

    if(private_cxpathjson_path_is_append(last_path)){

        cJSON_AddItemToArray(last_element, value);
    }

    else if(cJSON_IsString(last_path)){
        cJSON *already_exist  = cJSON_GetObjectItem(last_element, last_path->valuestring);
        if(already_exist){
            cJSON_DeleteItemFromObject(last_element, last_path->valuestring);
        }
        cJSON_AddItemToObject(last_element, last_path->valuestring, value);
    }

    else {
        int index = private_cxpathjson_convert_index(
                last_path->valueint,
                cJSON_GetArraySize(last_element)
        );

        cJSON_ReplaceItemInArray(last_element, index, value);

    }

}

void private_CxpathJson_set_cjson_by_va_arg_getting_ownership(CxpathJson *self, cJSON *value, const char *format, va_list args){
    if(CxpathJson_get_error_code(self)){
        cJSON_Delete(value);
        return;
    }
    char buffer[2000] = {0};
    vsnprintf(buffer, sizeof(buffer), format, args);
    private_cxpathjson_replace_comas(buffer);
    cJSON *parsed_path  = cJSON_Parse(buffer);

    if(private_cxpathjson_validate_path_all(parsed_path)){
        //we raise here beacause bad formatting its consider a comptime error
        CxpathJson  *root = private_CxpathJson_get_root(self);

        CxpathJson_raise_errror(root,
                                CXPATHJSON_ARG_PATH_NOT_VALID_CODE,
                                NULL,
                                PRIVATE_CXPATHJSON_ARG_PATH_NOT_VALID_MESSAGE,
                                buffer
        );

        cJSON_Delete(value);
        cJSON_Delete(parsed_path);
        return ;

    }
    private_cxpathjson_set_cjson_by_path_list(self, value, parsed_path);
    cJSON_Delete(parsed_path);

}




void CxpathJson_set_default_cjson_getting_ownership(CxpathJson *self, cJSON *value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        cJSON_Delete(value);
        return;
    }
    va_list args;
    va_start(args, format);

    bool old_config = self->raise_runtime_errors;
    self->raise_runtime_errors = false;
    cJSON *old = private_CxpathJson_get_cJSON_by_vargs(self,format,args);
    self->raise_runtime_errors = old_config;

    if(CxpathJson_get_error_code(self) || old){
        cJSON_Delete(value);
        return;
    }

    private_CxpathJson_set_cjson_by_va_arg_getting_ownership(self, value, format, args);
    va_end(args);
}
void CxpathJson_set_default_str_getting_onwership(CxpathJson *self,  char *value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_New_Item(&global_hooks);
    value_cjson->type = cJSON_String;
    value_cjson->valuestring = value;
    CxpathJson_set_default_cjson_getting_ownership(self, value_cjson, format, args);
    va_end(args);


}
void CxpathJson_set_default_str(CxpathJson *self, const char *value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateString(value);
    CxpathJson_set_default_cjson_getting_ownership(self, value_cjson, format, args);
    va_end(args);

}


void CxpathJson_set_default_double(CxpathJson *self, double value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateNumber(value);
    CxpathJson_set_default_cjson_getting_ownership(self, value_cjson, format, args);
    va_end(args);

}

void CxpathJson_set_default_int(CxpathJson *self, int value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateNumber((double )value);
    CxpathJson_set_default_cjson_getting_ownership(self, value_cjson, format, args);
    va_end(args);

}

void CxpathJson_set_default_bool(CxpathJson *self, bool value, const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return;
    }

    va_list args;
    va_start(args, format);
    cJSON *value_cjson = cJSON_CreateBool(value);
    CxpathJson_set_default_cjson_getting_ownership(self, value_cjson, format, args);
    va_end(args);
}



char *CxpathJson_dump_to_string(CxpathJson  *self, bool ident){
    if(CxpathJson_has_errors(self)){
        return NULL;
    }

    if(ident){
        return cJSON_Print(self->element);
    }
    return cJSON_PrintUnformatted(self->element);
}


void CxpathJson_dump_to_file(CxpathJson  *self, const char *filename, bool ident){

    if(CxpathJson_has_errors(self)){
        return ;
    }

    char *result = CxpathJson_dump_to_string(self, ident);
    int write_result = private_cxpathjson_write_file(filename, result);
    if(write_result == CXPATHJSON_IO_PROBLEM_CODE){
        CxpathJson_raise_errror(
                self,
                CXPATHJSON_IO_PROBLEM_CODE,
                NULL,
                CXPATHJSON_IO_PROBLEM_MESSAGE,
                filename
                );
    }
    free(result);
}






void CxpathJson_destroy(CxpathJson *self,const char *format, ...){
    if(CxpathJson_get_error_code(self)){
        return ;
    }

    va_list args;
    va_start(args, format);
    char buffer[2000] = {0};
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    private_cxpathjson_replace_comas(buffer);

    cJSON *parsed_path  = cJSON_Parse(buffer);
    if(private_cxpathjson_validate_path_read_only(parsed_path)){
        //we raise here beacause bad formatting its consider a comptime error
        CxpathJson_raise_errror(self,
                                CXPATHJSON_ARG_PATH_NOT_VALID_CODE,
                                NULL,
                                PRIVATE_CXPATHJSON_ARG_PATH_NOT_VALID_MESSAGE,
                                buffer
        );


        cJSON_Delete(parsed_path);
        return ;

    }
    bool old_raise = self->raise_runtime_errors;
    self->raise_runtime_errors = false;
    cJSON *possible_element = private_CxpathJson_cJSON_by_cjson_path_list(self,parsed_path);
    self->raise_runtime_errors = old_raise;
    if(!possible_element){
        cJSON_Delete(parsed_path);
        return;
    }

    //here we know the element exist
    int path_size = cJSON_GetArraySize(parsed_path);
    cJSON *rest = cJSON_CreateArray();

    for(int i = 0; i < path_size -1; i++){
        cJSON *element = cJSON_GetArrayItem(parsed_path,i);
        cJSON_AddItemReferenceToArray(rest,element);
    }
    cJSON *father = private_CxpathJson_cJSON_by_cjson_path_list(self,rest);
    cJSON_Delete(rest);

    cJSON *last = cJSON_GetArrayItem(parsed_path,path_size -1);

    if(cJSON_IsString(last)){
        cJSON_DeleteItemFromObject(father,last->valuestring);
    }
    if(cJSON_IsNumber(last)){
        cJSON_DeleteItemFromArray(father,last->valueint);
    }

    cJSON_Delete(parsed_path);

}



int private_cxpathjson_convert_index(int index, int size){
    if(index >=0){
        return index;
    }
    return (size +index);


}

bool private_cxpathjson_path_is_append(cJSON *current_path){
    bool is_append = false;
    if(cJSON_IsString(current_path)){
        is_append = strcmp(current_path->valuestring, CXPATHJSON_APPEND_KEY) == 0;
    }
    return is_append;

}

int private_cxpathjson_validate_path_all(cJSON *path){

    const int NOT_PATH = 1 ;
    const int NOT_ARRAY = 1;
    const int WRONG_TYPE = 1;
    if(!path){
        return NOT_PATH;
    }

    if(!cJSON_IsArray(path)){
        return NOT_ARRAY;
    }

    int path_size = cJSON_GetArraySize(path);
    for(int i = 0;i <path_size;i++){
        cJSON *current = cJSON_GetArrayItem(path,i);
        if(cJSON_IsString(current)){
            continue;
        }

        if(cJSON_IsNumber(current)){
            continue;
        }
        return  WRONG_TYPE;
    }
    return  CXPATHJSON_OK_CODE;



}

int private_cxpathjson_validate_path_read_only(cJSON *path){
    int first_test =  private_cxpathjson_validate_path_all(path);
    if(first_test){
        return first_test;
    }
    const int APPEND_NOT_VALID_IN_READ_ONY = 1;

    int path_size = cJSON_GetArraySize(path);
    for(int i = 0;i <path_size;i++){
        cJSON *current = cJSON_GetArrayItem(path,i);
        if(!cJSON_IsString(current)){
            continue;
        }
        if(strcmp(current->valuestring,CXPATHJSON_APPEND_KEY) ==0){
            return APPEND_NOT_VALID_IN_READ_ONY;
        }
    }
    return CXPATHJSON_OK_CODE;
}


void private_cxpathjson_replace_comas(char *result){
    long size = strlen(result);
    for(int i =1; i < size; i++){
        char current_char = result[i];
        char last_char = result[i-1];

        if(current_char == '\'' && last_char !='\\'){
            result[i] = '"';
        }
    }
}





CxpathJsonErrorNamespace newCxpathJsonErrorNamespace(){
    CxpathJsonErrorNamespace self = {0};
    self.get_error_code = CxpathJson_get_error_code;
    self.get_error_message = CxpathJson_get_error_message;
    self.get_error_path =CxpathJson_get_error_path;
    self.clear_errors = CxpathJson_clear_errors;
    self.has_errors = CxpathJson_has_errors;
    self.is_ok = CxpathJson_is_ok;
    self.raise_errror = CxpathJson_raise_errror;
    return  self;
}



CxpathJsonTypeNamespace newCxpathJsonTypeNamespace(void){
    CxpathJsonTypeNamespace self;
    self.NOT_FOUND = CXPATHJSON_NOT_FOUND;
    self.NULL_TYPE = CXPATHJSON_NULL;
    self.BOOL_TYPE = CXPATHJSON_BOOL;
    self.NUMBER_TYPE = CXPATHJSON_NUMBER;
    self.STRING_TYPE = CXPATHJSON_STRING;
    self.ARRAY_TYPE =CXPATHJSON_ARRAY;
    self.OBJECT_TYPE = CXPATHJSON_OBJECT;

    self.exist = CxpathJson_exist;
    self.type = CxpathJson_type;
    self.its_not_null = CxpathJson_its_not_null;
    self.type_str = CxpathJson_type_str;
    return self;
}



CxpathJsonNamespace newCxpathJsonNamespace(){
    CxpathJsonNamespace self;

    self.dump_to_file =CxpathJson_dump_to_file;
    self.dump_to_string = CxpathJson_dump_to_string;
    self.new_from_cJSON = newCxpathJson_from_cJSON_getting_ownership;
    self.newJsonObject = newCxpathJsonObject;
    self.newJsonArray = newCxpathJsonArray;
    self.new_from_string = newCxpathJson_from_string;
    self.new_from_file = newCxpathJson_from_file;

    self.get_bool = CxpathJson_get_bool;
    self.get_cJSON = CxpathJson_get_cJSON;
    self.get_double = CxpathJson_get_double;
    self.get_int = CxpathJson_get_int;
    self.get_str  = CxpathJson_get_str;
    self.get_object = CxpathJson_get_object;
    self.get_array = CxpathJson_get_array;
    self.get_key = CxpathJson_get_key;
    self.size = CxpathJson_get_size;


    self.set_bool = CxpathJson_set_bool;
    self.set_int = CxpathJson_set_int;
    self.set_double = CxpathJson_set_double;
    self.set_str  = CxpathJson_set_str;
    self.set_str_getting_ownership = CxpathJson_set_str_getting_ownership;
    self.set_cjson_getting_ownership = CxpathJson_set_cjson_getting_ownership;

    self.set_xpathJson_getting_onwership =CxpathJson_set_xpathJson_getting_onwership;
    self.set_xpath_by_reference = CxpathJson_set_xpath_by_reference;

    self.set_default_bool = CxpathJson_set_default_bool;
    self.set_default_int = CxpathJson_set_default_int;
    self.set_default_double = CxpathJson_set_default_double;
    self.set_default_str = CxpathJson_set_default_str;
    self.set_default_str_getting_onwership = CxpathJson_set_default_str_getting_onwership;
    self.set_default_cjson_getting_ownership = CxpathJson_set_default_cjson_getting_ownership;

    self.destroy = CxpathJson_destroy;


    self.free = CxpathJson_free;
    self.errors = newCxpathJsonErrorNamespace();
    self.types = newCxpathJsonTypeNamespace();
    return self;
}



    
#endif