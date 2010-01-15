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


CMapManager::~CMapManager()
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	
	for ( ; iter != iterEnd ; ++iter ) {
		delete (*iter);
	}
}



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


	
	/* Draw a circle at selected sprites */
	std::vector < sprite::CSprite* >::iterator iter = selectedSpriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = selectedSpriteList_.end();
	for ( ; iter != iterEnd ; ++iter ) {
		(*iter)->DrawMarker();
	}
	
	/* Draw sprites */
	iter = spriteList_.begin();
	iterEnd = spriteList_.end();
	for ( ; iter != iterEnd ; ++iter )
		app->Draw( **iter );

	
	/* Render rect to select objects */
	if ( settings::GetSelect() ) {
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
	
	int x = this->ConvertCoords ( mouseX );
	int y = this->ConvertCoords ( mouseY );
	
	switch ( button ) {
		case sf::Mouse::Left:
			selectedSpriteList_.clear();
			
			for ( ; iter != iterEnd ; ++iter ) {
				if ( !settings::GetSelect() ) {
					settings::SetSelect();
					selectedRect_.Left = selectedRect_.Right = mouseX;
					selectedRect_.Top = selectedRect_.Bottom = mouseY;
				} else {
					
					selectedRect_.Right = mouseX;
					selectedRect_.Bottom = mouseY;
					
					sf::Rect< float > selectedRectInGameCoord = this->ConvertCoords ( selectedRect_ );
					if ( ( *iter )->GetDimension().Intersects ( selectedRectInGameCoord ) ) {
						selectedSpriteList_.push_back ( *iter );
					}
					
					if ( this->GetSpecialWidget ( "MINI_OBJECT" ) )
						this->GetSpecialWidget ( "MINI_OBJECT" )->Call();
				}
			}
			break;
			
	}
}


bool CMapManager::MouseClickReleased ( const int mouseX, const int mouseY, const sf::Mouse::Button button )
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	
	int x = this->ConvertCoords ( mouseX );
	int y = this->ConvertCoords ( mouseY );
	
	
	sf::Rect< float > selectedRectInGameCoord = this->ConvertCoords ( selectedRect_ );

	
	switch ( button ) {
		case sf::Mouse::Left:
			settings::SetSelect ( false );
// 			selectedRect_.Left = selectedRect_.Right = 0;
// 			selectedSpriteList_.clear();
			
			for ( ; iter != iterEnd ; ++iter ) {
				/* Select rect was formed... find out which sprites selected */
// 				if ( settings::GetSelect() ) {
// // 					if ( selectedRect_.Contains ( ( *iter)->GetCenter() ) ) {
// 					if ( ( *iter )->GetDimension().Intersects ( selectedRectInGameCoord ) ) {
// 						selectedSpriteList_.push_back ( *iter );
// 					}
// 					this->GetSpecialWidget( "MINI_OBJECT" )->Call();
// 				}
				if ( ( *iter )->GetDimension().Contains( x, y ) ) {
				}
				

			}
			
			
			selectedRect_.Left = selectedRect_.Top = 0;

			break;
			
	}
}


void CMapManager::Zoom ( float offset, int direction )
{
	static const sf::Input* input = &GetGameClass()->GetApp()->GetInput();
	
	int deltaX = input->GetMouseX() - (GetGameClass()->GetApp()->GetWidth()  * 0.5f);
	int deltaY = input->GetMouseY() - (GetGameClass()->GetApp()->GetHeight() * 0.5f);
	
	if ( direction == 1 ) {
		if ( this->GetZoomLevel() < 6 ) { /* maximum zoom level */
			GetGameClass()->GetViewPoint()->Zoom ( 1 + offset );
			GetGameClass()->GetViewPoint()->Move ( deltaX * 0.05, deltaY * 0.05 );
		}
	} else if ( direction == -1 ) {
		if ( this->GetZoomLevel() > 0.05 ) { /* minimum zoom level */
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
	if ( lastPos_.x != 0 ) {
		GetGameClass()->GetViewPoint()->Move ( (lastPos_.x - newPos.x) / GetZoomLevel(), (lastPos_.y - newPos.y) / GetZoomLevel() );
	}

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


std::vector< sprite::CSprite* >* CMapManager::GetSelectedSprites()
{
	return &selectedSpriteList_;
}


gui::CWidget* CMapManager::GetSpecialWidget ( std::string name )
{
	if ( specialWidget_.find( name ) != specialWidget_.end() ) {
		return specialWidget_.at ( name );
	}

	return NULL;
}


void CMapManager::SetSpecialWidget ( std::string name, gui::CWidget* widget )
{
	specialWidget_ [ name ] = widget;
}


void CMapManager::UnSetPos()
{
	lastPos_.x = lastPos_.y = 0;
}


sf::Rect< float > CMapManager::ConvertCoords ( sf::Rect< float > rect )
{
	static sf::RenderWindow* app = GetGameClass()->GetApp();
	
	if ( rect.Right < rect.Left ) {
		float tmp = rect.Right;
		rect.Right = rect.Left;
		rect.Left = tmp;
	}
	if ( rect.Bottom < rect.Top ) {
		float tmp = rect.Bottom;
		rect.Bottom = rect.Top;
		rect.Top = tmp;
	}
		
	
	sf::Vector2f p1 = app->ConvertCoords ( rect.Left, rect.Top, GetGameClass()->GetViewPoint() );
	sf::Vector2f p2 = app->ConvertCoords ( rect.Right, rect.Bottom, GetGameClass()->GetViewPoint() );
	
	return sf::Rect< float > ( p1.x, p1.y, p2.x, p2.y );
}


sf::Vector2f CMapManager::ConvertCoords ( sf::Vector2f vector )
{
	static sf::RenderWindow* app = GetGameClass()->GetApp();
	return app->ConvertCoords ( vector.x, vector.y, GetGameClass()->GetViewPoint() );
}


float CMapManager::ConvertCoords ( float f )
{
	static sf::RenderWindow* app = GetGameClass()->GetApp();
	return app->ConvertCoords ( f, 0, GetGameClass()->GetViewPoint() ).x;
}


/* Private */

