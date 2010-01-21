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

#include <dirent.h>
#include "../settings/settings.hpp"

#include "script.hpp"

// #include "script/script_window.hpp"
// #include "script/script_widget.hpp"
//
namespace script
{

lua_State* luaInstance;
lua_State* GetLua()
{
    return luaInstance;
}


void Initialize()
{
    // luaState oeffnen
    luaInstance = lua_open();

    // lua-libraries initialisieren
    luaL_openlibs ( luaInstance );
    luaopen_base ( luaInstance );

    // lua-Scripte laden
    script::Load( luaInstance );
}


bool Load ( lua_State* L )
{
    /* Funktionen registrieren */
    lua_register(L, "getPath", script::GetPath);


    /* Scripte öffnen */
    std::string file = settings::GetPath() + "scripts/system.main";
    int status = luaL_dofile ( L, file.c_str() );
    script::CheckError ( L, status );


    /* Klassen registrieren */
// 	Luna < CScriptWidget >::Register ( L );
// 	Luna < CScriptWindow >::Register ( L );

    return true;
}


int GetPath ( lua_State* L )
{
    return Push ( L, settings::GetPath() );
}


void CheckError ( lua_State* L, int status )
{
    if ( status )
    {
        /* TODO logger */
        std::cout << lua_tostring ( L, -1 ) << std::endl;
    }
}


void SetField ( lua_State* L, const char *index, double value )
{
    lua_pushstring ( L, index );
    lua_pushnumber ( L, value );
    lua_settable ( L, -3 );
}


int GetField ( lua_State* L, const char *key )
{
    int result;
    lua_pushstring ( L, key );
    lua_gettable ( L, -2 );

    /* Ist der Wert wirklich eine Zahl? */
    if ( !lua_isnumber ( L, -1 ) )
    {
        /* TODO logger error */
    }

    /* Ausgabe vom Stack holen */
    result = ( int ) lua_tonumber ( L, -1 );

    /* Zahl wieder vom Stack nehmen */
    lua_pop ( L, 1 );

    return result;
}


int Push ( lua_State* L, bool istrue )
{
    lua_pushboolean ( L, istrue );
    return 1;
}


int Push ( lua_State* L, std::string str )
{
    lua_pushstring ( L, str.c_str() );
    return 1;
}


int Push ( lua_State* L, sf::Vector2f vec )
{
    lua_newtable ( L );
    {
        lua_pushnumber ( L, 1 );
        lua_pushnumber ( L, vec.x );
        lua_settable ( L, -3 );

        lua_pushnumber ( L, 2 );
        lua_pushnumber ( L, vec.y );
        lua_settable ( L, -3 );
    }

    return 1;
}


int Push ( lua_State* L, gui::CWindow* window )
{
    if ( !window )
    {
        lua_pushlightuserdata ( L, (void*)window );
    }

    return 1;
}


int Pull ( lua_State* L, int number )
{
    if ( lua_isnumber ( L, number ) ) {
        return lua_tonumber ( L, number );
    }
    else if ( lua_isboolean ( L, number ) ) {
        return lua_toboolean ( L, number );
    } else {
        return 0;
    }
}


gui::CWindow* Pull ( lua_State* L, TYPE, int number )
{
    if ( lua_isuserdata ( L, number ) ) {
        return ( gui::CWindow* ) lua_touserdata ( L, number );
    } else {
        return NULL;
    }
}



} /* namespace script */

