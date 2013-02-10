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

#include "cache.hpp"

cache::cache() : _functions (new std::map<statusfunc_t *, _cacheFunction_t>)
{
}

cache::~cache()
{
    delete _functions;
}

void cache::add(const statusfunc_t *function, unsigned int calls)
{
    _cacheFunction_t tmp(calls);
    _functions->insert(std::pair<const statusfunc_t *, _cacheFunction_t>(function, tmp));
}

std::string cache::get(const statusfunc_t *function, bool forceCall)
{
    auto it = _functions->find(function);
    _cacheFunction_t *tmp = &((*it).second);
    if (tmp->getLastCall() % tmp->getCalls() == 0 || forceCall) {
        tmp->setReturnValue(((*it).first)());
        tmp->setLastCall(0);
    }
    tmp->incrementLastCall();
    return tmp->getReturnValue();
}

void cache::refresh(const statusfunc_t *function)
{
    auto it = _functions->find(function);
    (*it).second.setLastCall(0);
}

bool cache::remove(const statusfunc_t *function)
{
    auto it = _functions->find(function);
    if (it != _functions->end()) {
        _functions->erase(it);
        return true;
    }
    return false;
}
