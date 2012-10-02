#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <unistd.h>
#include <cstdio>
#include <X11/Xlib.h>

#include "cache.hpp"

using namespace std;

#define INTERVAL                1
#define TIME_FORMAT             "%H:%M:%S"
#define TIME_BUFFER             10
#define MEM                     "/proc/meminfo"
#define LOADAVG                 "/proc/loadavg"
#define BATTERY_PRESENT         "/sys/class/power_supply/BAT0/present"
#define BATTERY_STATUS          "/sys/class/power_supply/BAT0/status"
#define BATTERY_NOW             "/sys/class/power_supply/BAT0/charge_now"
#define BATTERY_FULL            "/sys/class/power_supply/BAT0/charge_full"
#define NOWPLAYING_COMMAND      "mpc -f %title% current"
#define NOWPLAYING_MAX_LENGTH   20
#define UPDATE_COMMAND          "pacman -Qqu | wc -l"
//#define UPDATE_NO_SAH         // disable sah integration

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

string _getCommandOutput(string cmd, bool removeLastChar = false) // {{{2
{
    FILE *in;
    char buff[512];
    string ret;

    if(!(in = popen(cmd.c_str(), "r"))) {
        cerr << "_getCommandOutput: could'nt get output from " << cmd << endl;
        return "error";
    }

    while(fgets(buff, sizeof(buff), in)!=NULL){
        ret += buff;
    }
    pclose(in);
    if (removeLastChar) ret = ret.substr(0, ret.length() - 1);
    return ret;
} // 2}}}
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
    char buffer[TIME_BUFFER];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, TIME_BUFFER, TIME_FORMAT, timeinfo);
    return buffer;
} // }}}

string getLoad() // {{{
{
    vector<string> loads = _split(_getFileContent(LOADAVG));
    return loads[0] + " " + loads[1] + " " + loads[2];
} // }}}

string getNowPlaying() // {{{
{
    string np = _getCommandOutput(NOWPLAYING_COMMAND, true);
    if (np.length() > NOWPLAYING_MAX_LENGTH + 2)
        np = np.substr(0, NOWPLAYING_MAX_LENGTH).append("..");
    return np;
} // }}}

string getUpdates() // {{{
{
    #ifndef UPDATE_NO_SAH
        string aur = _getCommandOutput("<$XDG_CACHE_HOME/sah wc -l", true);
        return (_getCommandOutput(UPDATE_COMMAND, true) + (aur != "0" ? "+" + aur : ""));
    #else
        return (_getCommandOutput(UPDATE_COMMAND, true);
    #endif
} // }}}

string getBattery() // {{{
{
    if (_getFileContent(BATTERY_PRESENT).substr(0, 1) == "1") { // is present
        // get the status (Charging, Discharging, ...)
        string status = _getFileContent(BATTERY_STATUS);
        status = status.substr(0, status.length() - 1);
        string prefix = " ";
        if(status == "Discharging")
            prefix = "-";
        else if (status == "Charging")
            prefix = "+";
        else if (status == "Full")
            prefix = "";
        
        unsigned int percent = (stof(_getFileContent(BATTERY_NOW)) / stof(_getFileContent(BATTERY_FULL)) * 100);
        return prefix + to_string(percent) + "%";
    }
    return "";
} // }}}

string getMem() // {{{
{
    float total, free, buff, cache;
    FILE *infile = fopen(MEM, "r");
    fscanf(infile,"MemTotal: %f kB\nMemFree: %f kB\nBuffers: %f kB\nCached: %f kB\n", &total, &free, &buff, &cache);
    fclose(infile);
    return to_string(lround((total - (free + buff + cache)) / 1024)) + "M";
}
// }}}

// getCpuTemp {{{
unsigned int _getCoreTemp(unsigned int core)
{
    return stoul(_getFileContent("/sys/devices/platform/coretemp.0/temp" + to_string(core) + "_input")) / 1000;
}

string getCpuTemp()
{
    return to_string((_getCoreTemp(2) + _getCoreTemp(4)) / 2) + "°";
}
// }}}

int main(int argc, const char *argv[])
{
    if (!(dpy = XOpenDisplay(NULL))) {
        cerr << "dwmstatus: cannot open display." << endl;
        return 1;
    }

    cache c;
    c.add(getLoad, 31);
    c.add(getNowPlaying, 17);
    c.add(getUpdates, 60*60);
    c.add(getBattery, 97);
    c.add(getMem, 23);
    c.add(getCpuTemp, 5);
    // getCpu(1)
    c.add(getTime, 1);
    
    string bat;
    while (true) {
        bat = c.get(getBattery);
        setStatus(c.get(getLoad) + " [" + c.get(getNowPlaying) + "] " + c.get(getUpdates) + " " + 
                (!bat.empty() ? bat + " " : "") + 
                c.get(getMem) + " " + c.get(getCpuTemp) + " " + c.get(getTime));
        sleep(INTERVAL);
    }
    
    XCloseDisplay(dpy);
    return 0;
}
