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
#include "sprite/ship.hpp"
#include "mapmanager.hpp"
#include "game.hpp"



void CMapManager::Initialize()
{
	/* Only test data */
	sprite::CSprite* newSprite = AddSprite ( new sprite::CSun );
	newSprite->Scale ( .25f );
	newSprite->SetPosition ( 100, 100 );
	
	sprite::CSprite* newShip = AddSprite ( new sprite::CShip );
	newShip->Scale ( .25f );
	newShip->SetPosition ( 200, 200 );

	if ( !newSprite || !newShip )
	{
		std::cout << "FAILED" << std::endl;	
	}
	
	this->UnSetPos();
}


void CMapManager::Render()
{
	sf::RenderWindow* app = GetGameClass()->GetApp();

	std::vector < sprite::CSprite* >::iterator iter = spriteList.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList.end();
	
	for ( ; iter != iterEnd ; ++iter )
	{
		app->Draw( **iter );
	}

}


void CMapManager::Update()
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList.end();

	for ( ; iter != iterEnd ; ++iter )
	{
		( *iter )->Update();
	}
}


void CMapManager::Zoom ( float offset, int direction )
{
	static const sf::Input* input = &GetGameClass()->GetApp()->GetInput();
	
	int deltaX = input->GetMouseX() - (GetGameClass()->GetApp()->GetWidth()  * 0.5f);
	int deltaY = input->GetMouseY() - (GetGameClass()->GetApp()->GetHeight() * 0.5f);
	
	if ( direction == 1 ) {
		if ( this->GetZoomLevel() < 3 ) { /* maximum zoom level */
			GetGameClass()->GetViewPoint()->Zoom ( 1 + offset );
			GetGameClass()->GetViewPoint()->Move ( deltaX * 0.05, deltaY * 0.05 );
		}
	} else if ( direction == -1 ) {
		if ( this->GetZoomLevel() > 0.2 ) { /* minimum zoom level */
			GetGameClass()->GetViewPoint()->Zoom ( 1 - offset );
			GetGameClass()->GetViewPoint()->Move ( deltaX * 0.03, deltaY * 0.03 );
		}
	}
}


double CMapManager::GetZoomLevel()
{
	return GetGameClass()->GetApp()->GetWidth() / GetGameClass()->GetViewPoint()->GetRect().GetWidth();
}


void CMapManager::Move ( sf::Vector2f newPos )
{
	if ( lastPos.x != 0 )
		GetGameClass()->GetViewPoint()->Move ( (lastPos.x - newPos.x) / GetZoomLevel(), (lastPos.y - newPos.y) / GetZoomLevel() );

	lastPos = newPos;
}


sprite::CSprite* CMapManager::AddSprite ( sprite::CSprite* sprite )
{
	if ( !sprite )
	{
		return NULL;
	}
	else
	{
		spriteList.push_back( sprite );	
	}
	
	return sprite;
}


void CMapManager::UnSetPos()
{
	lastPos.x = lastPos.y = 0;
}
