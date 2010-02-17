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
#include <luabind/iterator_policy.hpp>

#include "script.hpp"

#include "../game.hpp"

#include "../settings/settings.hpp"
#include "../engine/sprite/sprite.hpp"
#include "../sprite/sun.hpp"
#include "../sprite/planet.hpp"
#include "../sprite/ship.hpp"


namespace script
{

	
/* Create a new lua state */
lua_State* luaState = lua_open();
int progress = 0;


int GetInitProgress()
{
	return progress;
}


void SetInitProgress ( int p )
{
	progress = p;
	
	if ( p == 100 ) {
		GetGameClass()->SetGameType( START );
	}
}

	
void Initialize ( void* UserData )
{
	/* Connect LuaBind to this lua state */
	luabind::open ( luaState );
	luaL_openlibs( luaState );
	
	/* Export functions */
	luabind::module ( luaState ) [
		luabind::def ( "setLoadProgress", SetInitProgress),
		luabind::def ( "log", GetLog ),
		luabind::def ( "path", settings::GetPath ),
		luabind::def ( "mapManager", GetMapManager )
	];
	
	/* Export our class with LuaBind */
	luabind::module ( luaState ) [
		luabind::class_ <sf::Drawable> ( "Drawable" ),
			
		luabind::class_ <CSystem, sf::Drawable> ( "System" )
			.def ( luabind::constructor<>() )
			.def ( "addSprite", &CSystem::AddSprite )
			.property ( "x", &CSystem::GetPositionX, &CSystem::SetPositionX )
			.property ( "y", &CSystem::GetPositionY, &CSystem::SetPositionY ),
			
		luabind::class_ <sprite::CSprite, sf::Drawable> ( "Sprite" )
			.def ( luabind::constructor<>() )
			.property ( "x", &sprite::CSprite::GetPositionX, &sprite::CSprite::SetPositionX )
			.property ( "y", &sprite::CSprite::GetPositionY, &sprite::CSprite::SetPositionY )
			.property ( "player", &sprite::CSprite::GetPlayer, &sprite::CSprite::SetPlayer ),
			
		luabind::class_ <sprite::CPlanet, luabind::bases<sprite::CSprite, sf::Drawable> > ( "Planet" )
			.def ( luabind::constructor<>() ),
			
		luabind::class_ <sprite::CSun, luabind::bases<sprite::CSprite, sf::Drawable> > ( "Sun" )
			.def ( luabind::constructor<>() ),
			
			luabind::class_ <sprite::CShip, luabind::bases<sprite::CSprite, sf::Drawable> > ( "Ship" )
			.def ( luabind::constructor<>() ),
			
		luabind::class_ <CMapManager> ("MapManager" )
			.def ( "createSystem", &CMapManager::CreateSystem )
			.def ( "addSprite", &CMapManager::AddSprite )
			.def_readonly ( "zoomLevel", &CMapManager::GetZoomLevel )
			.def_readonly ( "selectedSprites", &CMapManager::GetSelectedSprites, luabind::return_stl_iterator )
	];
	
	
	std::string filename = settings::GetPath() + "scripts/system.main";
	if ( luaL_dofile( luaState, filename.c_str() ) ) {
		GetGameClass()->Error ( lua_tostring(luaState, -1), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		return;
	}

}


void Quit()
{
	lua_close ( luaState );
}


void Reload()
{
	Quit();

	luaState = lua_open();
	Initialize();
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


CMapManager* GetMapManager()
{
	return GetGameClass()->GetMapManager();
}

	
	
} /* namespace script */

