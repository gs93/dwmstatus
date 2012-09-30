#include <string>
#include <vector>

using namespace std;

typedef string statusfunc_t();

class cache {
public:
    cache();
    void add(statusfunc_t *f, unsigned int calls);
    string execute();
    virtual ~cache();
private:
    struct _cacheFunc {
        statusfunc_t *func;
        unsigned int calls;
        unsigned int lastCall;
        string lastResult;
    };
    vector<_cacheFunc> functions;
};
