/*
 * Copyright (C) 2012 Giuliano Schneider <gs93@gmx.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CACHE_HPP__
#define __CACHE_HPP__

#include <string>
#include <map>

typedef std::string statusfunc_t();

class cache {
public:
    cache();
    ~cache();
    void add(const statusfunc_t *function, unsigned int calls);
    std::string get(const statusfunc_t *function, bool forceCall = false);
    void refresh(const statusfunc_t *function);
    bool remove(const statusfunc_t *function);

private:
    class _cacheFunction_t {
    public:
        _cacheFunction_t(unsigned int c) : calls(c), lastCall(0), returnValue("") {}
        unsigned int getCalls() const { return calls; }
        unsigned int getLastCall() const { return lastCall; }
        void setLastCall(unsigned int c) { lastCall = c; }
        void incrementLastCall() { setLastCall(lastCall + 1); }
        std::string getReturnValue() const { return returnValue; }
        void setReturnValue(const std::string &ret) { returnValue = ret; }

    private:
        unsigned int calls;
        unsigned int lastCall;
        std::string returnValue;
    };

    std::map<const statusfunc_t *, _cacheFunction_t> *_functions;

    cache(const cache &);
    cache &operator=(const cache &);
};
#endif
