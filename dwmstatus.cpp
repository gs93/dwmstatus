#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <unistd.h>
#include <X11/Xlib.h>

using namespace std;

vector<string> &_split(const string &s, char delim, vector<string> &elems);
vector<string> _split(const string &s, char delim);
string _getFileContent(string path);

void setStatus(const string &status);
string getLoad();

static Display *dpy;

// helper {{{1
vector<string> &_split(const string &s, char delim, vector<string> &elems) // {{{2
{
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim))
        elems.push_back(item);
    return elems;
} // 2}}}

vector<string> _split(const string &s, char delim = ' ')
{
    vector<string> elems;
    return _split(s, delim, elems);
}

string _getFileContent(string path)
{
    ifstream t(path.c_str());
    string str((istreambuf_iterator<char>(t)),
            istreambuf_iterator<char>());
    t.close();
    return str;
}
// 1}}}

void setStatus(const string &status) // {{{
{
    XStoreName(dpy, DefaultRootWindow(dpy), status.c_str());
    XSync(dpy, False);
} // }}}

string getLoad()
{
    vector<string> loads = _split(_getFileContent("/proc/loadavg"));
    return loads[0] + " " + loads[1] + " " + loads[2];
}

int main(int argc, const char *argv[])
{
    if (!(dpy = XOpenDisplay(NULL))) {
        cerr << "dwmstatus: cannot open display." << endl;
        return 1;
    }
    
    while (true) {
        setStatus(getLoad());
        sleep(1);
    }
    
    XCloseDisplay(dpy);
    return 0;
}
