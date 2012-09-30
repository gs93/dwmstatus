#include <string>
#include <iostream>
#include <X11/Xlib.h>

using namespace std;

static Display *dpy;

void setStatus(const string status)
{
    XStoreName(dpy, DefaultRootWindow(dpy), status.c_str());
    XSync(dpy, False);
}


int main(int argc, const char *argv[])
{
    if (!(dpy = XOpenDisplay(NULL))) {
        cerr << "dwmstatus: cannot open display." << endl;
        return 1;
    }
    
    setStatus("test");
    
    XCloseDisplay(dpy);
    return 0;
}
