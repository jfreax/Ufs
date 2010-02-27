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
	
	if ( p >= 100 ) {
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
		luabind::def ( "quit", QuitGame ),
		luabind::def ( "width", settings::GetWidth ),
		luabind::def ( "height", settings::GetHeight ),
		luabind::def ( "setLoadProgress", SetInitProgress),
		luabind::def ( "log", GetLog ),
		luabind::def ( "path", settings::GetPath ),
		luabind::def ( "mapManager", GetMapManager )
	];
	
	/* Export our class with LuaBind */
	luabind::module ( luaState ) [
		/* Vector */
		luabind::class_ < sf::Vector2<float> > ( "Vec" )
			.def ( luabind::constructor<>() )
			.def ( luabind::constructor< float, float >() )
			.def_readwrite( "x", &sf::Vector2<float>::x )
			.def_readwrite( "y", &sf::Vector2<float>::y ),
		
		/* Drawable */
		luabind::class_ <sf::Drawable> ( "Drawable" ),
		luabind::class_ <sf::Color> ( "Color" )
			.def ( luabind::constructor<sf::Uint8,sf::Uint8,sf::Uint8,sf::Uint8>() ),
			
		/* Sprite */
		luabind::class_ <sprite::CSprite, sf::Drawable> ( "Sprite" )
			.def ( luabind::constructor<>() )
			.property ( "x", &sprite::CSprite::GetPositionX, &sprite::CSprite::SetPositionX )
			.property ( "y", &sprite::CSprite::GetPositionY, &sprite::CSprite::SetPositionY )
			.property ( "player", &sprite::CSprite::GetPlayer, &sprite::CSprite::SetPlayer ),
			
		/* Sprite - Planet */
		luabind::class_ <sprite::CPlanet, luabind::bases<sprite::CSprite, sf::Drawable> > ( "Planet" )
			.def ( luabind::constructor<>() ),
		
		/* Sprite - Sun */
		luabind::class_ <sprite::CSun, luabind::bases<sprite::CSprite, sf::Drawable> > ( "Sun" )
			.def ( luabind::constructor<>() )
			.property ( "color", &sprite::CSun::GetColor, &sprite::CSun::SetColor ),
			
		/* Sprite - Ship */
		luabind::class_ <sprite::CShip, luabind::bases<sprite::CSprite, sf::Drawable> > ( "Ship" )
			.def ( luabind::constructor<>() ),
			
		/* MapManager */
		luabind::class_ <CMapManager> ("MapManager" )
			.def ( "createSystem", &CMapManager::CreateSystem )
			.def ( "addSprite", &CMapManager::AddSprite )
			.def ( "calcDistance", (float(CMapManager::*)(sf::Vector2f,sf::Vector2f))&CMapManager::CalcDistance )
			.def ( "calcDistance", (float(CMapManager::*)(sf::Vector2f,sprite::CSprite*))&CMapManager::CalcDistance )
			.def ( "calcDistance", (float(CMapManager::*)(sprite::CSprite*,sprite::CSprite*))&CMapManager::CalcDistance )
			.def_readonly ( "zoomLevel", &CMapManager::GetZoomLevel )
			.def_readonly ( "selectedSprites", &CMapManager::GetSelectedSprites, luabind::return_stl_iterator ),
			
		/* System float CMapManager::CalcDistance ( sf::Vector2f vec1, sf::Vector2f vec2 )*/
		luabind::class_ <CSystem, sf::Drawable> ( "System" )
			.def ( luabind::constructor<std::string>() )
			.property ( "x", &CSystem::GetPositionX, &CSystem::SetPositionX )
			.property ( "y", &CSystem::GetPositionY, &CSystem::SetPositionY )
			.def ( "addSprite", &CSystem::AddSprite )
			.def ( "getSun", &CSystem::GetSun )
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


void QuitGame()
{
	GetGameClass()->Stop();
}


	
	
} /* namespace script */

