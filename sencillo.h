#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "fio.h"
#include "http.h"

#if defined(__linux__)
    #include <linux/limits.h>
#else
    #include <limits.h>
#endif

#if !defined(__APPLE__) && !defined(__MACH__)
    #include <sys/random.h>
#endif

#include <mysql/mysql.h>

#define VA_MACRO(M, ...)                                                                                                    \
    __OVR(M, __VA_NUM_ARGS(__VA_ARGS__)) (__VA_ARGS__)

#define __OVR(macroName, numberArgs)                                                                                        \
    __OVR_EXPAND(macroName, numberArgs)

#define __OVR_EXPAND(macroName, numberArgs)                                                                                 \
    __##macroName##numberArgs

#define __ARG_PATTERN_MATCH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N
#define __ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15

#define __HAS_COMMA(...)                                                                                                    \
    __ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)

#define __NO_COMMA(...)                                                                                                     \
    __ARG16(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)

#define __TRIGGER_PARENTHESIS__(...) ,

#define __ZERO_OR_ONE_ARGS(...)                                                                                             \
    __HAS_ZERO_OR_ONE_ARGS(                                                                                                 \
        __HAS_COMMA(__VA_ARGS__),                                                                                           \
        __HAS_COMMA(__TRIGGER_PARENTHESIS__ __VA_ARGS__),                                                                   \
        __HAS_COMMA(__VA_ARGS__(~)),                                                                                        \
        __HAS_COMMA(__TRIGGER_PARENTHESIS__ __VA_ARGS__(~))                                                                 \
    )

#define __PASTE5(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4

#define __HAS_ZERO_OR_ONE_ARGS(_0, _1, _2, _3)                                                                              \
    __NO_COMMA(__PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))

#define _IS_EMPTY_CASE_0001 ,

#define __VA0(...) __ZERO_OR_ONE_ARGS(__VA_ARGS__)
#define __VA1(...) __ZERO_OR_ONE_ARGS(__VA_ARGS__)
#define __VA2(...)   2
#define __VA3(...)   3
#define __VA4(...)   4
#define __VA5(...)   5
#define __VA6(...)   6
#define __VA7(...)   7
#define __VA8(...)   8
#define __VA9(...)   9
#define __VA10(...) 10
#define __VA11(...) 11
#define __VA12(...) 12
#define __VA13(...) 13
#define __VA14(...) 14
#define __VA15(...) 15
#define __VA16(...) 16

#define __VA_NUM_ARGS(...)                                                                                                  \
    __VA_NUM_ARGS_IMPL(__VA_ARGS__, __PP_RSEQ_N(__VA_ARGS__))

#define __VA_NUM_ARGS_IMPL(...)                                                                                             \
    __VA_NUM_ARGS_N(__VA_ARGS__)

#define __VA_NUM_ARGS_N(                                                                                                    \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,                                                                                \
    _11, _12, _13, _14, _15, _16, N, ...) N

#define __PP_RSEQ_N(...)                                                                                                    \
    __VA16(__VA_ARGS__), __VA15(__VA_ARGS__), __VA14(__VA_ARGS__), __VA13(__VA_ARGS__),                                     \
    __VA12(__VA_ARGS__), __VA11(__VA_ARGS__), __VA10(__VA_ARGS__), __VA9 (__VA_ARGS__),                                     \
    __VA8 (__VA_ARGS__), __VA7 (__VA_ARGS__), __VA6 (__VA_ARGS__), __VA5 (__VA_ARGS__),                                     \
    __VA4 (__VA_ARGS__), __VA3 (__VA_ARGS__), __VA2 (__VA_ARGS__), __VA1 (__VA_ARGS__),                                     \
    __VA0 (__VA_ARGS__)

#define __RLY_UNPAREN(...) __VA_ARGS__
#define __INVOKE(__expr) __expr
#define unparen(__args) __INVOKE(__RLY_UNPAREN __args)

/* pseudotypes, manipulators, and aliases */

#define OBJECT_INVALID FIOBJ_INVALID
typedef FIOBJ Object;

typedef struct {
    Object key;
    Object value;
} Pair;

static struct HashMap {
    Object  (*create)       (Pair *pairs);
    Object  (*keys)         (Object map);
    Object  (*values)       (Object map);
    Object  (*get)          (Object map, char *key);
    Object  (*replace)      (Object map, char *key, Object value);
    int     (*set)          (Object map, char *key, Object value);
    int     (*remove)       (Object map, char *key);
    bool    (*contains)     (Object map, char *key);
    bool    (*isEmpty)      (Object map);
    void    (*clear)        (Object map);
    size_t  (*count)        (Object map);
} HashMap = {
             .create         = &__HashMap_create,
             .keys           = &__HashMap_keys,
             .values         = &__HashMap_values,
             .get            = &__HashMap_get,
             .replace        = &__HashMap_replace,
             .set            = &__HashMap_set,
             .remove         = &__HashMap_remove,
             .contains       = &__HashMap_contains,
             .isEmpty        = &__HashMap_isEmpty,
             .clear          = &fiobj_hash_clear,
             .count          = &fiobj_hash_count
};

static struct {
    Object  (*create)       (Object *items);
    Object* (*ptr)          (Object list);
    Object  (*first)        (Object list);
    Object  (*last)         (Object list);
    Object  (*pop)          (Object list);
    Object  (*get)          (Object list, int64_t pos);
    Object  (*replace)      (Object list, Object obj, int64_t pos);
    int     (*remove)       (Object list, int64_t pos);
    bool    (*isEmpty)      (Object list);
    bool    (*contains)     (Object list, Object obj);
    void    (*append)       (Object list, Object obj);
    size_t  (*size)         (Object list);
    int64_t (*indexOf)      (Object list, Object obj);
} List = {
             .create         = &__List_create,
             .ptr            = &fiobj_ary2ptr,
             .first          = &__List_first,
             .last           = &__List_last,
             .pop            = &fiobj_ary_pop,
             .get            = &fiobj_ary_index,
             .replace        = &fiobj_ary_replace,
             .remove         = &fiobj_ary_remove,
             .isEmpty        = &__List_isEmpty,
             .contains       = &__List_contains,
             .append         = &fiobj_ary_push,
             .size           = &fiobj_ary_count,
             .indexOf        = &fiobj_ary_find
};

static struct String {
    Object  (*create)       (char *str);
    Object  (*copy)         (Object src);
    void    (*immutable)    (Object obj);
    void    (*clear)        (Object obj);
    size_t  (*length)       (Object obj);
    size_t  (*appendInt)    (Object dest, int64_t num);
    size_t  (*appendFmt)    (Object dest, const char *format, ...);
    size_t  (*append)       (Object dest, const char *str);
    size_t  (*fromFile)     (Object dest, const char *filename, intptr_t start_at, intptr_t limit);
} String = {
             .create         = &__String_create,
             .copy           = &fiobj_str_copy,
             .immutable      = &fiobj_str_freeze,
             .clear          = &fiobj_str_clear,
             .length         = &__String_length,
             .appendInt      = &fiobj_str_write_i,
             .appendFmt      = &fiobj_str_printf,
             .append         = &__String_append,
             .fromFile       = &fiobj_str_readfile
};

#define null NULL

#define to ,

#define mapOf(...)                                                                                                          \
    VA_MACRO(mapOf, __VA_ARGS__)

#define __mapOf0()                                                                                                          \
    HashMap.create(null)

#define __mapOf1(_pairs)                                                                                                    \
    HashMap.create((Pair[]) { unparen(_pairs), {null} })                                                                    \

#define listOf(...)                                                                                                         \
    VA_MACRO(listOf, __VA_ARGS__)

#define __listOf0()                                                                                                         \
    List.create(null)

#define __listOf1(_items)                                                                                                   \
    List.create((Object[]) { unparen(_items), null })

#define String(_str)                                                                                                        \
    String.create(_str)

#define Int(_num)                                                                                                           \
    fiobj_num_new(_num)

#define Float(_num)                                                                                                         \
    fiobj_float_new(_num)

#define toString(_obj)                                                                                                      \
    fiobj_obj2cstr(_obj).data

#define toInt(_obj)                                                                                                         \
    fiobj_obj2num(_obj)

#define toFloat(_obj)                                                                                                       \
    fiobj_obj2float(_obj)

#define toJSON(_obj, _pretty)                                                                                               \
    fiobj_obj2json(_obj, _pretty)

#define fromJSON(_obj, _str)                                                                                                \
    fiobj_json2obj(_obj, _str, strlen(_str))

#define Free(_obj)                                                                                                          \
    fiobj_free(_obj)

#define Equals(_obj1, _obj2)                                                                                                \
    fiobj_iseq(_obj1, _obj2)

#define getClass(_obj)                                                                                                      \
    fiobj_type(_obj)

#define instanceOf(_obj, _type)                                                                                             \
    fiobj_type_is(_obj, _type)

#define forEach(_obj, ...) {                                                                                                \
    size_t __count = 0;                                                                                                     \
    if (instanceOf(_obj, FIOBJ_T_HASH)) {                                                                                   \
        if ((__count = HashMap.count(_obj)) > 0) {                                                                          \
            Object __keys = HashMap.keys(_obj);                                                                             \
            for (size_t __i = 0; __i < __count; __i++) {                                                                    \
                Object it = List.get(__keys, __i);                                                                          \
                __VA_ARGS__                                                                                                 \
                Free(it);                                                                                                   \
            }                                                                                                               \
            Free(__keys);                                                                                                   \
    } else if (instanceOf(_obj, FIOBJ_T_ARRAY)) {                                                                           \
        if ((__count = List.size(_obj)) > 0) {                                                                              \
            for (size_t __i = 0; __i < __count; __i++) {                                                                    \
                Object it = List.get(_obj, __i);                                                                            \
                __VA_ARGS__                                                                                                 \
                Free(it);                                                                                                   \
            }                                                                                                               \
        }                                                                                                                   \
    } else {                                                                                                                \
        FIO_LOG_ERROR("forEach() can only be called on HashMap and List objects.");                                         \
    }                                                                                                                       \
}

Object __HashMap_create(Pair *pairs) {
    if (!pairs) {
        return OBJECT_INVALID;
    }
    
    Object map = fiobj_hash_new();

    for (size_t i = 0; pairs[i].key != null; i++) {
        fiobj_hash_set(map, pairs->key, pairs->value);
        Free(pairs->key);
        Free(pairs->value);
    }

    return map;
}

Object __HashMap_keys(Object map) {
    if (!map) {
        return OBJECT_INVALID;
    }

    Object keys = fiobj_ary_new();

    fiobj_each1(map, 0, __HashMap_keys_callback, &keys);

    return keys;
}

int __HashMap_keys_callback(Object obj, void *keys) {
    fiobj_ary_push(*((Object *) keys), fiobj_hash_key_in_loop());
}

Object __HashMap_values(Object map) {
    if (!map) {
        return OBJECT_INVALID;
    }

    Object values = fiobj_ary_new();

    fiobj_each1(map, 0, __HashMap_values_callback, fiobj_ary2ptr(values));

    return values;
}

int __HashMap_values_callback(Object obj, void *values) {
    fiobj_ary_push(*((Object *) values), fiobj_hash_get(obj, fiobj_hash_key_in_loop()));
}

Object __HashMap_get(Object map, char *key) {
    if (!map || !key || !instanceOf(map, FIOBJ_T_HASH)) {
        return OBJECT_INVALID;
    }

    Object target = map;

    char *__key   = strdup(key);
    char *nextKey = strtok(__key, ".");

    while (nextKey) {
        Object objNextKey = String(nextKey);

        target = fiobj_hash_get(target, objNextKey);

        Free(objNextKey);
    }

    free(__key);

    return target;
}

Object __HashMap_replace(Object map, char *key, Object value) {
    Object keyObj = String(key);
    Object oldVal = fiobj_hash_replace(map, keyObj, value);

    Free(keyObj);

    return oldVal;
}

int __HashMap_set(Object map, char *key, Object value) {
    if (!map || !key || !value || !instanceOf(map, FIOBJ_T_HASH)) {
        return 0;
    }

    Object target = map;

    char *__key   = strdup(key);
    char *nextKey = strtok(__key, ".");
    char *lastKey;
    
    while (nextKey) {
        Object objNextKey = String(nextKey);

        if (!HashMap.contains(target, nextKey)) {
            fiobj_hash_set(target, objNextKey, mapOf());
        }

        Free(objNextKey);

        target  = HashMap.get(target, nextKey);
        lastKey = strdup(nextKey);
    }

    Object oldVal = HashMap.replace(target, lastKey, value);
    Free(oldVal);
    
    free(__key);
    free(lastKey);
}

int __HashMap_remove(Object map, char *key) {
    Object keyObj = String(key);

    int error = fiobj_hash_delete(map, keyObj);

    Free(keyObj);

    return error;
}

bool __HashMap_contains(Object map, char *key) {
    Object keyObj = String(key);

    bool truth = fiobj_hash_haskey(map, keyObj) == 1;

    Free(keyObj);

    return truth;
}

bool __HashMap_isEmpty(Object map) {
    return fiobj_hash_count(map) == 0;
}

Object __List_create(Object *items) {
    if (!items) {
        return OBJECT_INVALID;
    }

    Object list = fiobj_ary_new();

    for (size_t i = 0; items[i] != null; i++) {
        fiobj_ary_push(list, items[i]);
        Free(items[i]);
    }

    return list;
}

Object __List_first(Object list) {
    return fiobj_dup(fiobj_ary_index(list, 0));
}

Object __List_last(Object list){
    return fiobj_dup(fiobj_ary_index(list, -1));
}

bool __List_isEmpty(Object list) {
    return fiobj_ary_count(list) == 0;
}

bool __List_contains(Object list, Object obj){
    return fiobj_ary_find(list, obj) != -1;
}

Object __String_create(char *str) {
    return fiobj_str_new(str, strlen(str));
}

size_t __String_length(Object obj) {
    return fiobj_obj2cstr(obj).len;
}

size_t __String_append(Object obj, const char *str) {
    return fiobj_str_write(obj, str, strlen(str));
}

/* config file */

static struct Config {
    Object  values;
    Object  (*get)          (char *key);
    void    (*set)          (char *key, Object value);
    void    (*load)         (void);
} Config = {
             .values         = null,
             .get            = &__Config_get,
             .set            = &__Config_set,
             .load           = &__Config_load
};

Object __Config_get(char *key) {
    if (!key) {
        return OBJECT_INVALID;
    }

    if (!instanceOf(Config.values, FIOBJ_T_HASH)) {
        FIO_LOG_ERROR("Tried to get config value but config has not yet been loaded!");
        return OBJECT_INVALID;
    }

    return HashMap.get(Config.values, key);
}

void __Config_set(char *key, Object value) {
    if (!key || !value) {
        return;
    }

    if (!instanceOf(Config.values, FIOBJ_T_HASH)) {
        FIO_LOG_ERROR("Tried to set config value but config has not yet been loaded!");
        return;
    }

    HashMap.set(Config.values, key, value);
}

void __Config_load() {
    Object fileContents;

    size_t bytes = String.fromFile(fileContents, "./resources/config.json", 0, 0);

    if (!bytes || !fileContents) {
        FIO_LOG_FATAL("Unable to load configuration file at ./resources/config.json!");
        exit(1);
    }

    char *jsonString = toString(fileContents);

    size_t parsed = fromJSON(&Config.values, jsonString);

    Free(fileContents);

    if (!parsed || !Config.values) {
        FIO_LOG_FATAL("Configuration file is not valid JSON!");
        exit(2);
    }
}

/* server */

static struct Server {
    void    (*start)        (void);
    void    (*stop)         (void);
    int16_t (*isRunning)    (void);
} Server = {
             .start          = &__Server_start,
             .stop           = &fio_stop,
             .isRunning      = &fio_is_running
};

void __Server_start() {
    Config.load();

    Object facilHost = Config.get("facil.host");

    if (!facilHost) {
        facilHost = String("0.0.0.0");
    }

    Object facilPort = Config.get("facil.port");

    if (!facilPort) {
        facilPort = String("3000");
    }

    Object facilThreads = Config.get("facil.threads");

    if (!facilThreads) {
        facilThreads = Int(0);
    }

    Object facilWorkers = Config.get("facil.workers");

    if (!facilWorkers) {
        facilWorkers = Int(0);
    }

    Object facilPublicFolder = Config.get("facil.public-folder");

    if (!facilPublicFolder) {
        FIO_LOG_FATAL("Configuration file is missing definition for 'facil.public-folder'!");
        exit(4);
    }

    Object facilMaxBodySize = Config.get("facil.max-body-size");

    if (!facilMaxBodySize) {
        facilMaxBodySize = Int(10);
    }

    Object sencilloViewsFolder = Config.get("sencillo.views-folder");

    if (!sencilloViewsFolder) {
        sencilloViewsFolder = String("./resources/views");
        Config.set("sencillo.views-folder", sencilloViewsFolder);
        Free(sencilloViewsFolder);
    }

    Object sencilloAntiCsrf = Config.get("sencillo.csrf-protection");

    if (!sencilloAntiCsrf) {
        sencilloAntiCsrf = Int(0);
        Config.set("sencillo.csrf-protection", sencilloAntiCsrf);
        Free(sencilloAntiCsrf);
    }

    Object databaseEngine = Config.get("database.engine");

    if (!databaseEngine) {
        // TODO
    }

    int error = http_listen(
        toString(facilPort),
        toString(facilHost),
        .on_request     = onRequest,
        .public_folder  = toString(facilPublicFolder),
        .max_body_size  = toInt(facilMaxBodySize) * 1024 * 1024,
        .log            = 1
    );

    if (error < 0) {
        FIO_LOG_FATAL("Unable to start Facil.io webserver on %s:%s!", facilHost, facilPort);
        exit(6);
    }

    fio_reap_children();

    fio_start(
        .threads = facilThreads, 
        .workers = facilWorkers
    );
}

/* http */

#define Headers()                                                                                                           \
    request->headers

#define Header(...)                                                                                                         \
    VA_MACRO(Header, __VA_ARGS__)

#define __Header1(_name)                                                                                                    \
    HashMap.get(request->headers, _name);

#define __Header2(_name, _value) {                                                                                          \
    Object name  = String(_name);                                                                                           \
    http_set_header(request, name, _value);                                                                                 \
    Free(name);                                                                                                             \
}

#define Cookies()                                                                                                           \
    request->cookies

#define Cookie(...)                                                                                                         \
    VA_MACRO(Cookie, __VA_ARGS__)

#define __Cookie1(_name)                                                                                                    \
    toString(HashMap.get(request->cookies, _name))

#define __Cookie2(_name, _value)                                                                                            \
    http_set_cookie(request,                                                                                                \
        .name       = _name,                                                                                                \
        .value      = _value,                                                                                               \
        .name_len   = strlen(_name),                                                                                        \
        .value_len  = strlen(_value),                                                                                       \
        .http_only  = 1                                                                                                     \
    )

#define __Cookie7(_name, _value, _domain, _path, _max_age, _httpOnly, _secure)                                              \
    http_set_cookie(request,                                                                                                \
        .name       = _name,                                                                                                \
        .value      = _value,                                                                                               \
        .domain     = _domain,                                                                                              \
        .path       = _path,                                                                                                \
        .name_len   = strlen(_name),                                                                                        \
        .value_len  = strlen(_value),                                                                                       \
        .domain_len = strlen(_domain),                                                                                      \
        .path_len   = strlen(_path),                                                                                        \
        .max_age    = _max_age,                                                                                             \
        .secure     = _secure,                                                                                              \
        .http_only  = _httpOnly                                                                                             \
    )

#define Method(...)                                                                                                         \
    VA_MACRO(Method, __VA_ARGS__)

#define __Method0()                                                                                                         \
    request->method

#define __Method1(_method) {                                                                                                \
    Object method = String(request->method);                                                                                \
    bool truth = (strcmp(method, _method) == 0);                                                                            \
    Free(method);                                                                                                           \
    truth;                                                                                                                  \
}

#define Path(...)                                                                                                           \
    VA_MACRO(Path, __VA_ARGS__)

#define __Path0()                                                                                                           \
    request->path

#define __Path1(_path) {                                                                                                    \
    Object path = String(request->path);                                                                                    \
    bool truth  = (strcmp(path, _path) == 0);                                                                               \
    Free(path);                                                                                                             \
    truth;                                                                                                                  \
}

#define Params()                                                                                                            \
    request->params

#define Param(_param)                                                                                                       \
    HashMap.get(request->params, _param)

#define Error(_code)                                                                                                        \
    http_send_error(request, _code)

#define ModelAndView(_template, _map) {                                                                                     \
    char path[PATH_MAX];                                                                                                    \
    snprintf(path, sizeof(path), "%s/%s.mustache", Config.get("sencillo.views-folder"), path);                              \
                                                                                                                            \
    if (toInt(Config.get("sencillo.csrf-protection"))) {                                                                    \
        Object token = __Generate_Csrf_token(request, SessionId);                                                           \
        HashMap.set(_map, "csrf-token", token);                                                                             \
        Free(token);                                                                                                        \
    }                                                                                                                       \
                                                                                                                            \
    mustache_s *mustache = fiobj_mustache_load((fio_str_info_s) { .data = path });                                          \
    char *rendered = toString(fiobj_mustache_build(mustache, _map));                                                        \
                                                                                                                            \
    http_send_body(request, rendered, strlen(rendered))                                                                     \
}

#define Redirect(_url)                                                                                                      \
    Object url = String(_url);                                                                                              \
    Header("Location", url);                                                                                                \
    Free(url);                                                                                                              \
    Error(302);

/* sha256 for session key generation */

#define SHR(x, n) (x >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))

#define S0(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S1(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))
#define s0(x) (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3))
#define s1(x) (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10))

#define Ch(x,y,z)   (z ^ (x & (y ^ z)))
#define Maj(x,y,z) ((x & y) | (z & (x | y)))

#define SHA256_STEP(a,b,c,d,e,f,g,h,x,K) {                                                                                  \
    tmp1 = h + S1(e) + Ch(e,f,g) + K + x;                                                                                   \
    tmp2 = S0(a) + Maj(a,b,c);                                                                                              \
    h    = tmp1 + tmp2;                                                                                                     \
    d   += tmp1;                                                                                                            \
}

#define BE_LOAD32(n,b,i) {                                                                                                  \
    (n) = ((uint32_t) (b)[(i)    ] << 24)                                                                                   \
        | ((uint32_t) (b)[(i) + 1] << 16)                                                                                   \
        | ((uint32_t) (b)[(i) + 2] <<  8)                                                                                   \
        | ((uint32_t) (b)[(i) + 3]      );                                                                                  \
}

void __SHA256_hex(const char *in, char *out) {
    uint32_t w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12, w13, w14, w15;
    uint32_t tmp1, tmp2, a, b, c, d, e, f, g, h;
    size_t i, len = 0;

    unsigned char buf[60] = { 0 };
    uint32_t final[8] = { 0 };

    w0 = w1 = w2 = w3 = w4 = w5 = w6 = w7 = w8 = w9 = w10 = w11 = w12 = w13 = w14 = w15 = 0;
    tmp1 = tmp2 = a = b = c = d = e = f = g = h = 0;

    while (*in) {
        buf[len++] = *in++;
    }

    w15 = len << 3;
    buf[len++] = 0x80;

    BE_LOAD32( w0, buf,  0);
    BE_LOAD32( w1, buf,  4);
    BE_LOAD32( w2, buf,  8);
    BE_LOAD32( w3, buf, 12);
    BE_LOAD32( w4, buf, 16);
    BE_LOAD32( w5, buf, 20);
    BE_LOAD32( w6, buf, 24);
    BE_LOAD32( w7, buf, 28);
    BE_LOAD32( w8, buf, 32);
    BE_LOAD32( w9, buf, 36);
    BE_LOAD32(w10, buf, 40);
    BE_LOAD32(w11, buf, 44);
    BE_LOAD32(w12, buf, 48);
    BE_LOAD32(w13, buf, 52);
    BE_LOAD32(w14, buf, 56);

    a = 0x6a09e667;
    b = 0xbb67ae85;
    c = 0x3c6ef372;
    d = 0xa54ff53a;
    e = 0x510e527f;
    f = 0x9b05688c;
    g = 0x1f83d9ab;
    h = 0x5be0cd19;

    SHA256_STEP(a, b, c, d, e, f, g, h,  w0, 0x428a2f98);
    SHA256_STEP(h, a, b, c, d, e, f, g,  w1, 0x71374491);
    SHA256_STEP(g, h, a, b, c, d, e, f,  w2, 0xb5c0fbcf);
    SHA256_STEP(f, g, h, a, b, c, d, e,  w3, 0xe9b5dba5);
    SHA256_STEP(e, f, g, h, a, b, c, d,  w4, 0x3956c25b);
    SHA256_STEP(d, e, f, g, h, a, b, c,  w5, 0x59f111f1);
    SHA256_STEP(c, d, e, f, g, h, a, b,  w6, 0x923f82a4);
    SHA256_STEP(b, c, d, e, f, g, h, a,  w7, 0xab1c5ed5);
    SHA256_STEP(a, b, c, d, e, f, g, h,  w8, 0xd807aa98);
    SHA256_STEP(h, a, b, c, d, e, f, g,  w9, 0x12835b01);
    SHA256_STEP(g, h, a, b, c, d, e, f, w10, 0x243185be);
    SHA256_STEP(f, g, h, a, b, c, d, e, w11, 0x550c7dc3);
    SHA256_STEP(e, f, g, h, a, b, c, d, w12, 0x72be5d74);
    SHA256_STEP(d, e, f, g, h, a, b, c, w13, 0x80deb1fe);
    SHA256_STEP(c, d, e, f, g, h, a, b, w14, 0x9bdc06a7);
    SHA256_STEP(b, c, d, e, f, g, h, a, w15, 0xc19bf174);

    w0 = s1(w14) + w9 + s0(w1) + w0;
    SHA256_STEP(a, b, c, d, e, f, g, h,  w0, 0xe49b69c1);
    w1 = s1(w15) + w10 + s0(w2) + w1;
    SHA256_STEP(h, a, b, c, d, e, f, g,  w1, 0xefbe4786);
    w2 = s1(w0) + w11 + s0(w3) + w2;
    SHA256_STEP(g, h, a, b, c, d, e, f,  w2, 0x0fc19dc6);
    w3 = s1(w1) + w12 + s0(w4) + w3;
    SHA256_STEP(f, g, h, a, b, c, d, e,  w3, 0x240ca1cc);
    w4 = s1(w2) + w13 + s0(w5) + w4;
    SHA256_STEP(e, f, g, h, a, b, c, d,  w4, 0x2de92c6f);
    w5 = s1(w3) + w14 + s0(w6) + w5;
    SHA256_STEP(d, e, f, g, h, a, b, c,  w5, 0x4a7484aa);
    w6 = s1(w4) + w15 + s0(w7) + w6;
    SHA256_STEP(c, d, e, f, g, h, a, b,  w6, 0x5cb0a9dc);
    w7 = s1(w5) + w0 + s0(w8) + w7;
    SHA256_STEP(b, c, d, e, f, g, h, a,  w7, 0x76f988da);
    w8 = s1(w6) + w1 + s0(w9) + w8;
    SHA256_STEP(a, b, c, d, e, f, g, h,  w8, 0x983e5152);
    w9 = s1(w7) + w2 + s0(w10) + w9;
    SHA256_STEP(h, a, b, c, d, e, f, g,  w9, 0xa831c66d);
    w10 = s1(w8) + w3 + s0(w11) + w10;
    SHA256_STEP(g, h, a, b, c, d, e, f, w10, 0xb00327c8);
    w11 = s1(w9) + w4 + s0(w12) + w11;
    SHA256_STEP(f, g, h, a, b, c, d, e, w11, 0xbf597fc7);
    w12 = s1(w10) + w5 + s0(w13) + w12;
    SHA256_STEP(e, f, g, h, a, b, c, d, w12, 0xc6e00bf3);
    w13 = s1(w11) + w6 + s0(w14) + w13;
    SHA256_STEP(d, e, f, g, h, a, b, c, w13, 0xd5a79147);
    w14 = s1(w12) + w7 + s0(w15) + w14;
    SHA256_STEP(c, d, e, f, g, h, a, b, w14, 0x06ca6351);
    w15 = s1(w13) + w8 + s0(w0) + w15;
    SHA256_STEP(b, c, d, e, f, g, h, a, w15, 0x14292967);

    w0 = s1(w14) + w9 + s0(w1) + w0;
    SHA256_STEP(a, b, c, d, e, f, g, h,  w0, 0x27b70a85);
    w1 = s1(w15) + w10 + s0(w2) + w1;
    SHA256_STEP(h, a, b, c, d, e, f, g,  w1, 0x2e1b2138);
    w2 = s1(w0) + w11 + s0(w3) + w2;
    SHA256_STEP(g, h, a, b, c, d, e, f,  w2, 0x4d2c6dfc);
    w3 = s1(w1) + w12 + s0(w4) + w3;
    SHA256_STEP(f, g, h, a, b, c, d, e,  w3, 0x53380d13);
    w4 = s1(w2) + w13 + s0(w5) + w4;
    SHA256_STEP(e, f, g, h, a, b, c, d,  w4, 0x650a7354);
    w5 = s1(w3) + w14 + s0(w6) + w5;
    SHA256_STEP(d, e, f, g, h, a, b, c,  w5, 0x766a0abb);
    w6 = s1(w4) + w15 + s0(w7) + w6;
    SHA256_STEP(c, d, e, f, g, h, a, b,  w6, 0x81c2c92e);
    w7 = s1(w5) + w0 + s0(w8) + w7;
    SHA256_STEP(b, c, d, e, f, g, h, a,  w7, 0x92722c85);
    w8 = s1(w6) + w1 + s0(w9) + w8;
    SHA256_STEP(a, b, c, d, e, f, g, h,  w8, 0xa2bfe8a1);
    w9 = s1(w7) + w2 + s0(w10) + w9;
    SHA256_STEP(h, a, b, c, d, e, f, g,  w9, 0xa81a664b);
    w10 = s1(w8) + w3 + s0(w11) + w10;
    SHA256_STEP(g, h, a, b, c, d, e, f, w10, 0xc24b8b70);
    w11 = s1(w9) + w4 + s0(w12) + w11;
    SHA256_STEP(f, g, h, a, b, c, d, e, w11, 0xc76c51a3);
    w12 = s1(w10) + w5 + s0(w13) + w12;
    SHA256_STEP(e, f, g, h, a, b, c, d, w12, 0xd192e819);
    w13 = s1(w11) + w6 + s0(w14) + w13;
    SHA256_STEP(d, e, f, g, h, a, b, c, w13, 0xd6990624);
    w14 = s1(w12) + w7 + s0(w15) + w14;
    SHA256_STEP(c, d, e, f, g, h, a, b, w14, 0xf40e3585);
    w15 = s1(w13) + w8 + s0(w0) + w15;
    SHA256_STEP(b, c, d, e, f, g, h, a, w15, 0x106aa070);

    w0 = s1(w14) + w9 + s0(w1) + w0;
    SHA256_STEP(a, b, c, d, e, f, g, h,  w0, 0x19a4c116);
    w1 = s1(w15) + w10 + s0(w2) + w1;
    SHA256_STEP(h, a, b, c, d, e, f, g,  w1, 0x1e376c08);
    w2 = s1(w0) + w11 + s0(w3) + w2;
    SHA256_STEP(g, h, a, b, c, d, e, f,  w2, 0x2748774c);
    w3 = s1(w1) + w12 + s0(w4) + w3;
    SHA256_STEP(f, g, h, a, b, c, d, e,  w3, 0x34b0bcb5);
    w4 = s1(w2) + w13 + s0(w5) + w4;
    SHA256_STEP(e, f, g, h, a, b, c, d,  w4, 0x391c0cb3);
    w5 = s1(w3) + w14 + s0(w6) + w5;
    SHA256_STEP(d, e, f, g, h, a, b, c,  w5, 0x4ed8aa4a);
    w6 = s1(w4) + w15 + s0(w7) + w6;
    SHA256_STEP(c, d, e, f, g, h, a, b,  w6, 0x5b9cca4f);
    w7 = s1(w5) + w0 + s0(w8) + w7;
    SHA256_STEP(b, c, d, e, f, g, h, a,  w7, 0x682e6ff3);
    w8 = s1(w6) + w1 + s0(w9) + w8;
    SHA256_STEP(a, b, c, d, e, f, g, h,  w8, 0x748f82ee);
    w9 = s1(w7) + w2 + s0(w10) + w9;
    SHA256_STEP(h, a, b, c, d, e, f, g,  w9, 0x78a5636f);
    w10 = s1(w8) + w3 + s0(w11) + w10;
    SHA256_STEP(g, h, a, b, c, d, e, f, w10, 0x84c87814);
    w11 = s1(w9) + w4 + s0(w12) + w11;
    SHA256_STEP(f, g, h, a, b, c, d, e, w11, 0x8cc70208);
    w12 = s1(w10) + w5 + s0(w13) + w12;
    SHA256_STEP(e, f, g, h, a, b, c, d, w12, 0x90befffa);
    w13 = s1(w11) + w6 + s0(w14) + w13;
    SHA256_STEP(d, e, f, g, h, a, b, c, w13, 0xa4506ceb);
    w14 = s1(w12) + w7 + s0(w15) + w14;
    SHA256_STEP(c, d, e, f, g, h, a, b, w14, 0xbef9a3f7);
    w15 = s1(w13) + w8 + s0(w0) + w15;
    SHA256_STEP(b, c, d, e, f, g, h, a, w15, 0xc67178f2);

    final[0] = a + 0x6a09e667;
    final[1] = b + 0xbb67ae85;
    final[2] = c + 0x3c6ef372;
    final[3] = d + 0xa54ff53a;
    final[4] = e + 0x510e527f;
    final[5] = f + 0x9b05688c;
    final[6] = g + 0x1f83d9ab;
    final[7] = h + 0x5be0cd19;

    for (i = 0; i < 8; i++) {
        sprintf(out + (i * 8), "%08x", final[i]);
    }
}

/* session handler */

static struct Session {
    Object  table;
    Object  (*get)          (char *SessionId, char *key);
    int     (*destroy)      (char *SessionId);
    bool    (*set)          (char *SessionId, char *key, Object value);
    char   *(*getId)        (http_s *request);
    void    (*create)       (http_s *request);
} Session = {
             .table          = mapOf(),
             .get            = &__Session_get,
             .destroy        = &__Session_destroy,
             .set            = &__Session_set,
             .create         = &__Session_create
};

Object __Session_get(char *SessionId, char *key) {
    if (!SessionId || !key || !HashMap.contains(Session.table, SessionId)) {
        return OBJECT_INVALID;
    }

    Object keyPath = String("");
    String.appendFmt(keyPath, "%s.%s", SessionId, key);

    Object value = HashMap.get(Session.table, toString(keyPath));

    Free(keyPath);

    return value;
}

bool __Session_set(char *SessionId, char *key, Object value) {
    if (
           !SessionId
        || !key
        || !value
        || !HashMap.contains(Session.table, SessionId)
    ) {
        return false;
    }

    Object session = HashMap.get(Session.table, SessionId);

    if (!instanceOf(session, FIOBJ_T_HASH)) {
        return false;
    }

    HashMap.set(session, key, value);

    Object oldVal = HashMap.replace(Session.table, SessionId, session);

    Free(oldVal);
    Free(session);

    return true;
}

int __Session_destroy(char *SessionId) {
    return HashMap.remove(Session.table, SessionId);
}

Object __Generate_token() {
    #define SHA256_DIGEST_SZ    64 + 1
    #define RANDOM_SZ           16

    char token[SHA256_DIGEST_SZ] = {0};
    char random[RANDOM_SZ] = {0};

#ifdef __APPLE__
    arc4random_buf(random, sizeof random);
#else
    if (getrandom(random, sizeof random, 0) < (ssize_t) sizeof random) {
        FIO_LOG_ERROR("Failed to read sufficient random bytes!");
    }
#endif

    __SHA256_hex(random, token);

    for (int i = 0; i < SHA256_DIGEST_SZ; i++) {
        token[i] ^= random[i % RANDOM_SZ];
    }

    return String(token);
}

void __Session_create(http_s *request) {
    Object token    = __Generate_token();
    char *SessionId = toString(token);

    HashMap.set(Session.table, SessionId, mapOf());

    Free(token);

    Object cookieName = Config.get("sencillo.cookie-name");

    Cookie(toString(cookieName), SessionId);

    Free(cookieName);
}

#define Session(...)                                                                                                        \
    VA_MACRO(Session, __VA_ARGS__)

#define __Session1(_key)                                                                                                    \
    Session.get(SessionID, _key)

#define __Session2(_key, _val)                                                                                              \
    Session.set(SessionId, _key, _val)

/* csrf protection */

void __Generate_Csrf_token(http_s *request, char *SessionId) {
    Object token = __Generate_token();

    Session("csrf-token", token);
    Header("csrf-token", token);
    
    Free(token);
}

/* route handler */

bool __Path_Matches(http_s *request, char *__route) {
    char *route = strdup(route);
    
    if (!route) {
        FIO_LOG_ERROR("Failed to allocate memory!");
        return false;
    }
    
    char *path = toString(request->path);
    
    if (!path) {
        FIO_LOG_ERROR("No request path?");
        free(route);
        return false;
    }

    path = strtok(path, "?");

    if (strchr(route, ':') == null) {
        if (strcmp(route, path) == 0) {
            free(route);
            return true;
        } else {
            free(route);
            return false;
        }
    }

    int routeWordCnt = 0;
    int pathWordCnt  = 0;

    for (int i = 0; route[i]; route[i] == '/' ? routeWordCnt++, i++ : i++);
    for (int i = 0; path[i];  path[i]  == '/' ? pathWordCnt++,  i++ : i++);

    if (routeWordCnt != pathWordCnt) {
        free(route);
        return false;
    }

    char *routeWords[routeWordCnt];
    char *pathWords[pathWordCnt];

    char *routeRemaining;
    char *pathRemaining;

    char *routeWordNext = strtok_r(__route, "/", &routeRemaining);
    char *pathWordNext  = strtok_r(__path,  "/", &pathRemaining);

    int matches = 0;

    while (routeWordNext && pathWordNext) {
        if (routeWordNext[0] == ':') {
            routeWords[matches]  = routeWordNext + 1;
            pathWords[matches++] = pathWordNext;
        } else {
            if (strcmp(routeWordNext, pathWordNext)) {
                free(route);
                return false;
            }
        }

        routeWordNext = strtok_r(null, "/", &routeRemaining);
        pathWordNext  = strtok_r(null, "/", &pathRemaining);
    }

    free(route);

    for (int i = 0; i < matches; i++) {
        http_add2hash(request->params,
            routeWords[i], strlen(routeWords[i]),
            pathWords[i],  strlen(pathWords[i]), 1
        );
    }

    return true;
}

void onRequest(http_s *request);

#define Routes                                                                                                              \
    void onRequest(http_s *request) {                                                                                       \
        http_parse_query(request);                                                                                          \
        http_parse_body(request);                                                                                           \
        http_parse_cookies(request, 1);                                                                                     \
                                                                                                                            \
        char *SessionId = Session.getId();                                                                                  \
                                                                                                                            \
        if (!SessionId || !HashMap.contains(Session.table, SessionID)) {                                                    \
            Session.create(request);                                                                                        \
        }                                                                                                                   \
                                                                                                                            \
        Object csrfProtection = Config.get("sencillo.csrf_protection");                                                     \
                                                                                                                            \
        if (Method("POST") && toInt(csrfProtection)) {                                                                      \
            Object sessCsrfToken = Session("csrf-token");                                                                   \
            Object userCsrfToken = Param("csrf-token");                                                                     \
                                                                                                                            \
            if (!userCsrfToken) {                                                                                           \
                userCsrfToken = Header("csrf-token");                                                                       \
            }                                                                                                               \
                                                                                                                            \
            if (!Equals(sessCsrfToken, userCsrfToken)) {                                                                    \
                Error(403);                                                                                                 \
            }                                                                                                               \
        }                                                                                                                   \
                                                                                                                            \
        __Routes(request, SessionId);                                                                                       \
    }                                                                                                                       \
                                                                                                                            \
    void __Routes(http_s *request, char *SessionId)

#define Route(_method, _route, ...)                                                                                         \
    if (Method(_method) && __Path_Matches(request, _route)) {                                                               \
        FIO_LOG_DEBUG("Matched route %s %s", _method, _route);                                                              \
        __VA_ARGS__                                                                                                         \
        return;                                                                                                             \
    }

#define Get(_route, ...)                                                                                                    \
    Route("GET", _route, __VA_ARGS__);

#define Post(_route, ...)                                                                                                   \
    Route("POST", _route, __VA_ARGS__);

#define Put(_route, ...)                                                                                                    \
    Route("PUT", _route, __VA_ARGS__);

#define Delete(_route, ...)                                                                                                 \
    Route("DELETE", _route, __VA_ARGS__);

#define Default(...)                                                                                                        \
    FIO_LOG_INFO("No route for '%s %s', falling back on default route.",                                                    \
        toString(request->method), toString(request->path));                                                                \
    __VA_ARGS__

/* database */

static struct Database {
    Object  params;
    void    (*connect)      (void);
    void    (*close)        (void);
    void    (*prepare)      (char *sql);
    void    (*bind)         (char *name, char *value);
    int     (*exec)         (void);
    Object  (*fetchrow)     ();
    Object  (*column)       (int column);
} Database = {
             .params         = mapOf()
};

void __Database_connect(char *SessionId) {
    Object engine = Config.get("database.engine");
    Object name   = Config.get("database.dbname");
    Object sock   = Config.get("database.socket");
    Object host   = Config.get("database.host");
    Object port   = Config.get("database.port");
    Object user   = Config.get("database.user");
    Object pass   = Config.get("database.pass");

    if (!engine || !name || !user || !pass || (!sock && (!host || !port))) {
        FIO_LOG_ERROR("Database has not been configured!");
    } else if (strcmp(toString(engine), "mysql") == 0) {
        MYSQL *dbh = mysql_init(null);

        if (!dbh) {
            FIO_LOG_ERROR("Failed to initalize database handler: %s", mysql_error(dbh));
        } else {
            if (!mysql_real_connect(dbh, toString(host), toString(user), toString(pass), toString(name), toInt(port), toString(sock), 0)) {
                FIO_LOG_ERROR("Unable to connect to MySQL/MariaDB: %s", mysql_error(dbh));
                mysql_close(dbh);
            } else {
                my_bool myTrue = (my_bool) 1;
                mysql_options(dbh, MYSQL_OPT_RECONNECT, (void *) myTrue);

                Object handler = fiobj_str_new((const char *) dbh, sizeof(MYSQL));

                Session.set(SessionId, "database.engine", engine);
                Session.set(SessionId, "database.handle", handler);

                Free(handler);
            }
        }
    } else {
        FIO_LOG_ERROR("'%s' is not a supported database engine!", toString(engine));
    }

    Free(engine);
    Free(name);
    Free(sock);
    Free(host);
    Free(port);
    Free(user);
    Free(port);

    return;
}

bool __Database_prepare(char *SessionId, void *sth, const char *sql) {
    Object engine  = Session.get(SessionId, "database.engine");
    Object handler = Session.get(SessionId, "database.handler");

    if (!engine) {
        FIO_LOG_ERROR("Database has not been configured!");
        return false;
    } else if (!handler) {
        FIO_LOG_ERROR("Database handler is not initialized!");
        return false;
    } else if (strcmp(toString(engine), "mysql") == 0) {
        MYSQL *dbh = (MYSQL *) toString(engine);

        if (mysql_ping(dbh)) {
            FIO_LOG_ERROR("Database is not connected: %s", mysql_error(dbh));
            return false;
        } else {
            sth = (void *) mysql_stmt_init(dbh);

            if (!sth) {
                FIO_LOG_ERROR("Unable to allocate prepared statement: %s", mysql_stmt_error((MYSQL_STMT *) sth));
                return false;
            } else {
                if (mysql_stmt_prepare((MYSQL_STMT *) sth, sql, strlen(sql))) {
                    FIO_LOG_ERROR("Unable to prepare statement: %s", mysql_stmt_error((MYSQL_STMT *) sth));
                    return false;
                }
            }
        }
    }
}
