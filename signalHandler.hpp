#ifndef __SIGNAL_HANDLER_HPP__
#define __SIGNAL_HANDLER_HPP__

#include <csignal>
#include <map>

typedef void signalfunc_t();

class signalHandler {
public:
    static signalHandler &getInstance();
    void addSignal(int sig, signalfunc_t *function);
    void callFunction(int sig);

private:
    signalHandler() {}
    ~signalHandler() {}
    signalHandler(const signalHandler &) {}
    signalHandler &operator=(const signalHandler &) { return *this; }
    static void _sigCatcher(int sig);

    std::map<int, signalfunc_t *> _signals;
};

#endif
