#include "cache.hpp"

cache::cache()
{
}

cache::~cache()
{
}

void cache::add(statusfunc_t *f, unsigned int calls)
{
    _cacheFunc tmp;
    tmp.func = f;
    tmp.calls = calls;
    tmp.lastCall = 1;
    tmp.lastResult = f();
    functions.push_back(tmp);
}

void cache::execute()
{
    // TODO: write this..^^
}
