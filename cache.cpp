#include "cache.hpp"

cache::cache()
{
}

cache::~cache()
{
}

void cache::add(statusfunc_t *function, unsigned int calls)
{
    _cacheFunction_t tmp;
    tmp.calls = calls;
    tmp.lastCall = 0;
    _functions.insert(pair<statusfunc_t *, _cacheFunction_t>(function, tmp));
}

string cache::get(statusfunc_t *function, bool forceCall)
{
    auto it = _functions.find(function);
    _cacheFunction_t *tmp = &((*it).second);
    if (tmp->lastCall % tmp->calls == 0 || forceCall) {
        tmp->returnValue = ((*it).first)();
        tmp->lastCall = 0;
    }
    tmp->lastCall++;
    return tmp->returnValue;
}

void cache::refresh(statusfunc_t *function)
{
    auto it = _functions.find(function);
    (*it).second.lastCall = 0;
}

bool cache::remove(statusfunc_t *function)
{
    auto it = _functions.find(function);
    if (it != _functions.end()) {
        _functions.erase(it);
        return true;
    }
    return false;
}
