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

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/class.hpp>
#include <luabind/function.hpp>
#include <luabind/object.hpp>

#include "script.hpp"

#include "../game.hpp"
#include "../settings/settings.hpp"
#include "../engine/sprite/sprite.hpp"
#include "../sprite/sun.hpp"


namespace script
{
	
/* Create a new lua state */
lua_State* luaState = lua_open();

	
void Initialize()
{
	/* Connect LuaBind to this lua state */
	luabind::open ( luaState );
	luaL_openlibs( luaState );
	
	/* Export functions */
	luabind::module ( luaState ) [
		luabind::def ( "getLog", GetLog ),
		luabind::def ( "getPath", settings::GetPath )
		
	];
	
	/* Export our class with LuaBind */
	luabind::module ( luaState ) [
		luabind::class_<CMapManager>("MapManager")
			.def_readonly ("ZoomLevel", &CMapManager::GetZoomLevel )
	];
	
	
	std::string filename = settings::GetPath() + "scripts/system.main";
	if ( luaL_dofile( luaState, filename.c_str() ) ) {
		GetGameClass()->Error ( "Could not open script: " + filename, __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		return;
	}


}


lua_State* GetLuaState()
{
	return luaState;
}


std::string GetLog ( int i )
{
	std::vector< std::string >* log = GetGameClass()->GetLog();
	
	std::vector< std::string >::iterator endIter = log->end();
	std::vector< std::string >::iterator iter = log->begin();
	
	std::string ret;
	for ( ; iter != endIter && i; ++iter, --i ) {
		ret += ( *iter );
		ret += "\n";
	}
			
			
	return ret;
}
	
	
} /* namespace script */

