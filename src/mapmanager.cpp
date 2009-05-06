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


#include "sprite/sun.hpp"
#include "mapmanager.hpp"
#include "game.hpp"



void CMapManager::Initialize ( void )
{
	sprite::CSprite* newSprite = AddSprite ( new sprite::CSun );
	newSprite->Scale ( .25f );
	newSprite->SetPosition ( 100, 100 );
	

	if ( !newSprite )
	{
		std::cout << "FAILED" << std::endl;	
	}

}


void CMapManager::Render ( void )
{
	sf::RenderWindow* app = GetGameClass()->GetApp();

	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	
	for ( ; iter != iterEnd ; ++iter )
	{
		app->Draw( **iter );
	}

}


void CMapManager::Update ( void )
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();

	for ( ; iter != iterEnd ; ++iter )
	{
		( *iter )->Update();
	}
}


sprite::CSprite* CMapManager::AddSprite ( sprite::CSprite* sprite )
{
	if ( !sprite )
	{
		return NULL;
	}
	else
	{
		spriteList_.push_back( sprite );	
	}
	
	return sprite;
}
