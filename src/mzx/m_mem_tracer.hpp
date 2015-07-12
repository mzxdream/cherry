#ifndef _M_MEM_TRACER_H_
#define _M_MEM_TRACER_H_

#include "m_type_define.hpp" 
#include "m_mutex.hpp"
#include <malloc.h>
#include <map>
#include <string>
#include <iostream>

template<typename T>
struct MMemTracerAllocator
    :std::allocator<T>
{
    typedef typename std::allocator<T>::pointer pointer;
    typedef typename std::allocator<T>::size_type size_type;

    template<typename U>
    struct rebind
    {
        typedef MMemTracerAllocator<U> other;
    };

    MMemTracerAllocator(){}

    template<typename U>
    MMemTracerAllocator(MMemTracerAllocator<U> const &u)
        :std::allocator<T>(u)
    {
    }

    pointer allocate(size_type size, std::allocator<void>::const_pointer = 0)
    {
        void *p = ::malloc(size * sizeof(T));
        if (p == 0)
        {
            throw std::bad_alloc();
        }
        return static_cast<pointer>(p);
    }
    
    void deallocate(pointer p, size_type)
    {
        ::free(p);
    }
};

struct MMemTracerInfo
{
    const char *file;
    MI64 line;
    size_t size;
};

class MMemTracerExit;

class MMemTracer
{
private:
    MMemTracer(const MMemTracer &);
    MMemTracer& operator=(const MMemTracer &);
public:
    static void Add(void *memory, const char *file, MI64 line, size_t size)
    {
        MAutoLock<MMutex> lock(mtx_);
        MMemTracerInfo info;
        info.file = file;
        info.line = line;
        info.size = size;
        memory_map_[memory] = info;
    }
    static void Remove(void *memory)
    {
        MAutoLock<MMutex> lock(mtx_);
        std::map<void*, MMemTracerInfo, std::less<void*>, MMemTracerAllocator<std::pair<const char *, MMemTracerInfo> > >::iterator it 
            = memory_map_.find(memory);
        if (it != memory_map_.end())
        {
            memory_map_.erase(it);
        }
    }
    static void Dump()
    {
        
        std::map<void*, MMemTracerInfo, std::less<void*>, MMemTracerAllocator<std::pair<const char *, MMemTracerInfo> > >::iterator it 
            = memory_map_.begin();
        for (; it != memory_map_.end(); ++it)
        {
            std::cerr << "memory leaks at file:" << it->second.file << " line:" << it->second.line << " size:" << it->second.size << std::endl;
        }
    }
private:
    static std::map<void*, MMemTracerInfo, std::less<void*>, MMemTracerAllocator<std::pair<const char *, MMemTracerInfo> > > memory_map_;
    static MMutex mtx_;
    static MMemTracerExit exit_;
};

class MMemTracerExit
{
public:
    ~MMemTracerExit()
    {
        MMemTracer::Dump();
    }
};

std::map<void*, MMemTracerInfo, std::less<void*>, MMemTracerAllocator<std::pair<const char *, MMemTracerInfo> > > MMemTracer::memory_map_;
MMutex MMemTracer::mtx_;
MMemTracerExit MMemTracer::exit_;

inline void* operator new(size_t size, const char *file, MI64 line)
{
//    std::cout << "new(size_t size, const char *file, MI64 line)" << std::endl;
    void *memory = ::malloc(size);
    if (memory)
    {
        MMemTracer::Add(memory, file, line, size);
    }
    return memory;
}

inline void operator delete(void *memory, const char *file, MI64 line)
{
//    std::cout << "delete(void *memory, const char *file, MI64 line)" << std::endl;
    if (memory)
    {
        MMemTracer::Remove(memory);
        ::free(memory);
    }
}

inline void* operator new(size_t size)
{
//    std::cout << "new(size_t size)" << std::endl;
    void *memory = ::malloc(size);
    if (memory)
    {
        MMemTracer::Add(memory, "?", 0, size);
    }
    return memory;
}

inline void operator delete(void *memory)
{
//    std::cout << "delete(void *memory)" << std::endl;
    if (memory)
    {
        MMemTracer::Remove(memory);
        ::free(memory);
    }
}

inline void* operator new[](size_t size, const char *file, MI64 line)
{
//    std::cout << "new[](size_t size, const char *file, MI64 line)" << std::endl;
    void *memory = ::malloc(size);
    if (memory)
    {
        MMemTracer::Add(memory, file, line, size);
    }
    return memory;
}

inline void operator delete[](void *memory, const char *file, MI64 line)
{
//    std::cout << "delete[](void *memory, const char *file, MI64 line)" << std::endl;
    if (memory)
    {
        MMemTracer::Remove(memory);
        ::free(memory);
    }
}

inline void* operator new[](size_t size)
{
//    std::cout << "new[](size_t size)" << std::endl;
    void *memory = ::malloc(size);
    if (memory)
    {
        MMemTracer::Add(memory, "?", 0, size);
    }
    return memory;
}

inline void operator delete[](void *memory)
{
//    std::cout << "delete[](void *memory)" << std::endl;
    if (memory)
    {
        MMemTracer::Remove(memory);
        ::free(memory);
    }
}

inline void* _malloc(size_t size, const char *file, MI64 line)
{
    void *memory = ::malloc(size);
    if (memory)
    {
        MMemTracer::Add(memory, file, line, size);
    }
    return memory;
}

inline void* _calloc(size_t n, size_t size, const char *file, MI64 line)
{
    void *memory = ::calloc(n, size);
    if (memory)
    {
        MMemTracer::Add(memory, file, line, size);
    }
    return memory;
}

inline void* _realloc(void *memory, size_t size, const char *file, MI64 line)
{
    if (memory)
    {
        MMemTracer::Remove(memory);
    }
    memory = ::realloc(memory, size);
    if (memory)
    {
        MMemTracer::Add(memory, file, line, size);
    }
    return memory;
}

inline void _free(void *memory)
{
    if (memory)
    {
        MMemTracer::Remove(memory);
        ::free(memory);
    }
}

#define new            new(__FILE__, __LINE__)
#define malloc(s)      _malloc(s, __FILE__, __LINE__)
#define calloc(n, s)   _calloc(n, s, __FILE__, __LINE__)
#define realloc(p, s)  _realloc(p, s, __FILE__, __LINE__)
#define free(p)        _free(p)

#endif 
