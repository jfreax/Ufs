/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SCRIPT_HPP
#define SCRIPT_HPP

// extern "C"
// {
// #include "lua.h"
// #include "lauxlib.h"
// #include "lualib.h"
// 
// }

#include <iostream>
#include <SFML/System.hpp>

#include "LuaState.hpp"
#include "../engine/ui/window.hpp"

namespace script
{
	

Diluculum::LuaState* GetLua();
void Initialize();


// int GetPath ( lua_State* L );


} /* namespace script */



#endif // SCRIPT_HPP
