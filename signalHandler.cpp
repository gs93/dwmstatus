#include "signalHandler.hpp"

map<int, bool> signalHandler::_signals;

signalHandler &signalHandler::getInstance()
{
    static signalHandler instance; 
    return instance;
}

void signalHandler::addSignal(int sig)
{
    _signals.insert(pair<int, bool>(sig, false));
    signal((int) sig, _sigCatcher);
}

bool signalHandler::gotSignal(int sig)
{
    auto it = _signals.find(sig);
    if (it != _signals.end()) {
        if ((*it).second) {
            (*it).second = false;
            return true;
        }
        return false;
    }
    return false; // XXX: throw exception?
}

void signalHandler::_sigCatcher(int sig)
{
    auto it = _signals.find(sig);
    (*it).second = true;
}
