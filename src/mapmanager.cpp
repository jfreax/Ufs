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
		std::cout << "FAILED" << std::endl;
	
	selectedRect_ = sf::Rect < float > ( 0, 0, 0, 0 );
	
	this->UnSetPos();
}


void CMapManager::Render()
{
	sf::RenderWindow* app = GetGameClass()->GetApp();

	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	
	for ( ; iter != iterEnd ; ++iter )
		app->Draw( **iter );
	
	/* Render rect to select objects */
	if ( selectedRect_.Left != 0 && selectedRect_.Top != 0 ) {
		sf::Shape rect = sf::Shape::Rectangle ( selectedRect_.Left, selectedRect_.Top, selectedRect_.Right, selectedRect_.Bottom, sf::Color ( 0, 0, 50, 60 ), 1, sf::Color ( 0, 0, 200,80 ) );
		
		app->SetView ( app->GetDefaultView() );
		app->Draw( rect );
	}

}


void CMapManager::Update()
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();

	for ( ; iter != iterEnd ; ++iter )
		( *iter )->Update();
}


bool CMapManager::MouseClick ( const int mouseX, const int mouseY, const sf::Mouse::Button button )
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	
	int x = GetGameClass()->GetApp()->ConvertCoords ( mouseX, mouseY, GetGameClass()->GetViewPoint() ).x;
	int y = GetGameClass()->GetApp()->ConvertCoords ( mouseX, mouseY, GetGameClass()->GetViewPoint() ).y;
	
	switch ( button ) {
		case sf::Mouse::Left:
			for ( ; iter != iterEnd ; ++iter ) {
				if ( selectedRect_.Left == 0 && selectedRect_.Top == 0 ) {
					selectedRect_.Left = selectedRect_.Right = mouseX;
					selectedRect_.Top = selectedRect_.Bottom = mouseY;
				} else {
					settings::SetSelect();
					
					selectedRect_.Right = mouseX;
					selectedRect_.Bottom = mouseY;
				}
			}
			break;
			
	}
}


bool CMapManager::MouseClickReleased ( const int mouseX, const int mouseY, const sf::Mouse::Button button )
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	
	int x = GetGameClass()->GetApp()->ConvertCoords ( mouseX, mouseY, GetGameClass()->GetViewPoint() ).x;
	int y = GetGameClass()->GetApp()->ConvertCoords ( mouseX, mouseY, GetGameClass()->GetViewPoint() ).y;
	
	switch ( button ) {
		case sf::Mouse::Left:
			selectedRect_.Left = selectedRect_.Top = 0;
			settings::SetSelect ( false );
			
			for ( ; iter != iterEnd ; ++iter ) {
				if ( ( *iter )->GetDimension().Contains( x, y ) ) {
				}
			}
			break;
			
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
	if ( lastPos_.x != 0 )
		GetGameClass()->GetViewPoint()->Move ( (lastPos_.x - newPos.x) / GetZoomLevel(), (lastPos_.y - newPos.y) / GetZoomLevel() );

	lastPos_ = newPos;
}


sprite::CSprite* CMapManager::AddSprite ( sprite::CSprite* sprite )
{
	if ( !sprite )
		return NULL;
	else
		spriteList_.push_back( sprite );	
	
	return sprite;
}


void CMapManager::UnSetPos()
{
	lastPos_.x = lastPos_.y = 0;
}
