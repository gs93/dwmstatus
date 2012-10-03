#ifndef __SIGNAL_HANDLER_HPP__
#define __SIGNAL_HANDLER_HPP__

#include <csignal>
#include <map>

using namespace std;

class signalHandler {
public:
    static signalHandler &getInstance();
    void addSignal(int sig);
    bool gotSignal(int sig);

private:
    signalHandler() {}
    ~signalHandler() {}
    signalHandler(const signalHandler &) {}
    signalHandler &operator=(const signalHandler &) { return *this; }
    static void _sigCatcher(int sig);

    static map<int, bool> _signals;
};

#endif
