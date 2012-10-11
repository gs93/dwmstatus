#ifndef __CACHE_HPP__
#define __CACHE_HPP__

#include <string>
#include <map>

typedef std::string statusfunc_t();

class cache {
public:
    cache();
    cache(const cache&);
    cache &operator=(const cache&);
    virtual ~cache();
    void add(statusfunc_t *function, unsigned int calls);
    std::string get(statusfunc_t *function, bool forceCall = false);
    void refresh(statusfunc_t *function);
    bool remove(statusfunc_t *function);

private:
    class _cacheFunction_t {
    public:
        _cacheFunction_t(unsigned int c) : calls(c), lastCall(0), returnValue("") {}
        unsigned int getCalls() { return calls; }
        unsigned int getLastCall() { return lastCall; }
        void setLastCall(unsigned int c) { lastCall = c;}
        void incrementLastCall() { setLastCall(lastCall + 1); }
        std::string getReturnValue() { return returnValue; }
        void setReturnValue(std::string ret) { returnValue = ret; }

    private:
        unsigned int calls;
        unsigned int lastCall;
        std::string returnValue;
    };

    std::map<statusfunc_t *, _cacheFunction_t> *_functions;
};
#endif
