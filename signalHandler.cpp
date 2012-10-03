#include "signalHandler.hpp"

signalHandler &signalHandler::getInstance()
{
    static signalHandler instance; 
    return instance;
}

void signalHandler::addSignal(int sig, signalfunc_t *function)
{
    _signals.insert(std::pair<int, signalfunc_t *>(sig, function));
    signal((int) sig, _sigCatcher);
}

void signalHandler::callFunction(int sig)
{
    ((*(_signals.find(sig))).second)();
}

void signalHandler::_sigCatcher(int sig)
{
    signalHandler::getInstance().callFunction(sig);
}
