#include "cache.hpp"

cache::cache() : _functions (new std::map<statusfunc_t *, _cacheFunction_t>)
{
}

cache::cache(const cache&) : _functions (new std::map<statusfunc_t *, _cacheFunction_t>)
{
    // TODO
}

cache &cache::operator=(const cache &)
{
    // TODO
    return *this;
}

cache::~cache()
{
    delete _functions;
}

void cache::add(statusfunc_t *function, unsigned int calls)
{
    _cacheFunction_t tmp(calls);
    _functions->insert(std::pair<statusfunc_t *, _cacheFunction_t>(function, tmp));
}

std::string cache::get(statusfunc_t *function, bool forceCall)
{
    auto it = _functions->find(function);
    _cacheFunction_t *tmp = &((*it).second);
    if (tmp->getLastCall() % tmp->getCalls() == 0 || forceCall) {
        tmp->setReturnValue(((*it).first)());
        tmp->setLastCall(0);
    }
    tmp->incrementLastCall();
    return tmp->getReturnValue();
}

void cache::refresh(statusfunc_t *function)
{
    auto it = _functions->find(function);
    (*it).second.setLastCall(0);
}

bool cache::remove(statusfunc_t *function)
{
    auto it = _functions->find(function);
    if (it != _functions->end()) {
        _functions->erase(it);
        return true;
    }
    return false;
}
