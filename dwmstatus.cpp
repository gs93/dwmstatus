#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <unistd.h>
#include <X11/Xlib.h>

#include "cache.hpp"

using namespace std;

vector<string> &_split(const string &s, char delim, vector<string> &elems);
vector<string> _split(const string &s, char delim);
string _getFileContent(string path);

void setStatus(const string &status);
string getTime();
string getLoad();
string getNowPlaying();

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

string getTime() // {{{
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[10];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 10, "%H:%M:%S",timeinfo);
    return buffer;
} // }}}

string getLoad() // {{{
{
    vector<string> loads = _split(_getFileContent("/proc/loadavg"));
    return loads[0] + " " + loads[1] + " " + loads[2];
} // }}}

string getNowPlaying() // {{{
{
    // ncmpcpp --now-playing "{{%15a - }{%32t}}|{%50f}"
} // }}}

int main(int argc, const char *argv[])
{
    if (!(dpy = XOpenDisplay(NULL))) {
        cerr << "dwmstatus: cannot open display." << endl;
        return 1;
    }

    cache c;
    c.add(getLoad, 30);
    // getNowPlaying, getUpdates, getBattery, getMem, getCpuTemp, getCpu
    c.add(getTime, 1);

    while (true) {
        setStatus(c.get(getLoad) + " " + c.get(getTime));
        sleep(1);
    }
    
    XCloseDisplay(dpy);
    return 0;
}
