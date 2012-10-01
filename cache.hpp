#include <string>
#include <vector>
#include <map>

using namespace std;

typedef string statusfunc_t();

class cache {
public:
    cache();
    void add(statusfunc_t *function, unsigned int calls);
    string get(statusfunc_t *function);
    virtual ~cache();

private:
    struct _cacheFunction_t {
        unsigned int calls;
        unsigned int lastCall;
        string lastResult;
    };
    map<statusfunc_t *, _cacheFunction_t> _functions;
};
