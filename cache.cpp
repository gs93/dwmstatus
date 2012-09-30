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

string cache::execute()
{
    string ret = "";
    for (auto it = functions.begin(); it < functions.end(); it++) {
        if ((*it).lastCall % (*it).calls == 0) {
            (*it).lastResult = (*it).func();
            (*it).lastCall = 0;
        }
        ret += (*it).lastResult;
        if (it < functions.end() - 1) ret += " ";
        (*it).lastCall++;
    }
    return ret;
}
