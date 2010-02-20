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


#include "../sprite/sun.hpp"
#include "../sprite/planet.hpp"
#include "../sprite/ship.hpp"

#include "../engine/ui/window.hpp"
#include "../gui/window/tooltip.hpp"
#include "../game.hpp"
#include "manager.hpp"


CMapManager::~CMapManager()
{
	std::vector < CSystem* >::iterator sysIter = systems_.begin();
	std::vector < CSystem* >::iterator sysIterEnd = systems_.end();
	for ( ; sysIter != sysIterEnd ; ++sysIter )
		delete ( *sysIter );
// 	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
// 	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
// 	for ( ; iter != iterEnd ; ++iter ) {
// 		delete (*iter);
// 	}
}



void CMapManager::Initialize()
{
	currentSystem_ = NULL;
	
	viewMode_ = SYSTEM;
	zoomed_ = 0;
	
	/* Only test data */
	sprite::CSprite* newSprite =/* AddSprite (*/ new sprite::CSun /*)*/;
	newSprite->SetPosition ( 100, 100 );
	
	/* BUG Why is this necessary? luabind! */
	sprite::CSprite* newSprite2 = /*AddSprite ( */new sprite::CPlanet /*)*/;
	delete newSprite2;

	sprite::CSprite* newShip2 = /*AddSprite ( */new sprite::CShip/* )*/;
	newShip2->SetPosition ( 500, 400 );
	
	selectedRect_ = sf::Rect < float > ( 0, 0, 0, 0 );
	this->UnSetPos();
	
}


void CMapManager::Render()
{
	sf::RenderWindow* app = GetGameClass()->GetApp();

	/* Draw sprites */
	std::vector < CSystem* >::iterator sysIter = systems_.begin();
	std::vector < CSystem* >::iterator sysIterEnd = systems_.end();
	for ( ; sysIter != sysIterEnd ; ++sysIter )
		app->Draw( **sysIter );

	/* Draw a circle at selected sprites */
	std::vector < sprite::CSprite* >::iterator iter = selectedSpriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd =selectedSpriteList_.end();
	for ( ; iter != iterEnd ; ++iter ) {
		(*iter)->DrawMarker();
	}
	
	/* Render rect to select objects */
	if ( settings::GetSelect() ) {
		sf::Shape rect = sf::Shape::Rectangle ( selectedRect_.Left, selectedRect_.Top, selectedRect_.Right, selectedRect_.Bottom, sf::Color ( 0, 0, 50, 60 ), 1, sf::Color ( 0, 0, 200,80 ) );
		app->SetView ( app->GetDefaultView() );
		app->Draw( rect );
	}

}


void CMapManager::Update()
{
	std::vector < CSystem* >::iterator sysIter = systems_.begin();
	std::vector < CSystem* >::iterator sysIterEnd = systems_.end();
	for ( ; sysIter != sysIterEnd ; ++sysIter )
		( *sysIter )->Update();
	
	/* Fade out the zoom */
	if ( zoomed_ > 0 ) {
		zoomed_ -= zoomed_*0.1+0.05f;
		this->Zoom ( lastZoomDirection_, true );
	}
	
	/* Change to galaxy view */
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	if ( lastZoomDirection_ == -1 && zoom < 0.15f && zoom > 0.01f ) {
		this->Zoom ( -1 );
		viewMode_ = GALAXY;
	}
	if ( lastZoomDirection_ == 1 && zoom < 0.10f ) {
		this->Zoom ( 1, false, false );
		
	}
	if ( lastZoomDirection_ == 1 && zoom > 0.14f ) {
		viewMode_ = SYSTEM;
	}
}


bool CMapManager::MouseClick ( const int mouseX, const int mouseY, const sf::Mouse::Button button )
{
	
	switch ( button ) {
		case sf::Mouse::Left:
			selectedSpriteList_.clear();
			
			std::vector < CSystem* >::iterator sysIter = systems_.begin();
			std::vector < CSystem* >::iterator sysIterEnd = systems_.end();
			for ( ; sysIter != sysIterEnd ; ++sysIter ) {
				
				if ( !settings::GetSelect() ) {
					settings::SetSelect();
					selectedRect_.Left = selectedRect_.Right = mouseX;
					selectedRect_.Top = selectedRect_.Bottom = mouseY;
				} else {
					selectedRect_.Right = mouseX;
					selectedRect_.Bottom = mouseY;
					
					/* Test only the sun-sprites in galaxy view */
					if ( this->GetViewMode() == GALAXY ) {
					
					
					} else { /* Iterate all sprite */
						std::vector < sprite::CSprite* >::iterator iter = (*sysIter)->GetSprites().begin();
						std::vector < sprite::CSprite* >::iterator iterEnd = (*sysIter)->GetSprites().end();
						for ( ; iter != iterEnd ; ++iter ) {
							sf::Rect< float > selectedRectInGameCoord = this->ConvertCoords ( selectedRect_ );
							if ( ( *iter )->GetDimension().Intersects ( selectedRectInGameCoord ) ) {
								selectedSpriteList_.push_back ( *iter );
							}
					
							if ( this->GetSpecialWidget ( "MINI_OBJECT" ) )
								this->GetSpecialWidget ( "MINI_OBJECT" )->Call();
						}
					}
				}
			}
			break;
			
	}
}


bool CMapManager::MouseClickReleased ( const int mouseX, const int mouseY, const sf::Mouse::Button button )
{
	int x = this->ConvertCoordsX ( mouseX );
	int y = this->ConvertCoordsY ( mouseY );
	
	sf::Rect< float > selectedRectInGameCoord = this->ConvertCoords ( selectedRect_ );

	switch ( button ) {
		case sf::Mouse::Left:
			settings::SetSelect ( false );
			selectedRect_.Left = selectedRect_.Top = 0;
			
			if ( this->GetViewMode() == GALAXY ) {
				this->Zoom ( 1 );
			}

			break;
			
	}
}


bool CMapManager::MouseHover ( const int mouseX, const int mouseY )
{
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	int x = this->ConvertCoordsX ( mouseX );
	int y = this->ConvertCoordsY ( mouseY );
	
	std::vector < CSystem* >::iterator sysIter = systems_.begin();
	std::vector < CSystem* >::iterator sysIterEnd = systems_.end();
	for ( ; sysIter != sysIterEnd ; ++sysIter ) {
		/* When we are on galaxy view... */
		if ( this->GetViewMode() == GALAXY && zoom < 0.006f ) {
			/* ... and the mouse is over a sun... */
			if ( (*sysIter)->GetDimension().Contains( x, y ) ) {
				/* ... then show tooltip */
				if ( GetGameClass()->GetSpecialWindow ( "GALAXY_TOOLTIP" ) ) {
					dynamic_cast< gui::CTooltip* > ( GetGameClass()->GetSpecialWindow ( "GALAXY_TOOLTIP" ) )->SetText ( (*sysIter)->GetInfoText() );
							
					GetGameClass()->GetSpecialWindow ( "GALAXY_TOOLTIP" )->SetShow ( true );
					GetGameClass()->GetSpecialWindow ( "GALAXY_TOOLTIP" )->AdjustSize();
					GetGameClass()->GetSpecialWindow ( "GALAXY_TOOLTIP" )->SetPosition( sf::Vector2f (mouseX+20, mouseY) );
					
					/* and show glow of the sun */
					(*sysIter)->GetSun().ShowGlow();
					
					currentSystem_ = (*sysIter);
				}
			}
		}
	}
	
	return true;
}


void CMapManager::Zoom ( int direction, bool fade, bool deltaMove )
{
	static const sf::Input* input = &GetGameClass()->GetApp()->GetInput();
	static double zoomStep = 0.02f;
	static int deltaX, deltaY;
	double zoom = 1/ GetGameClass()->GetMapManager()->GetZoomLevel() * (0.03);
	
	
	if ( !fade ) {
		deltaX = input->GetMouseX() - (GetGameClass()->GetApp()->GetWidth()  * 0.5f);
		deltaY = input->GetMouseY() - (GetGameClass()->GetApp()->GetHeight() * 0.5f);
		zoomed_ += 10;
	} else {
		deltaX *= 0.9;
		deltaY *= 0.9;
	}
	
	lastZoomDirection_ = direction;
	
	/* Zoom out */
	if ( direction == 1 ) {
		if ( this->GetZoomLevel() < 6.f ) { /* maximum zoom level */
			GetGameClass()->GetViewPoint()->Zoom ( 1 + zoomStep* ( zoomed_*0.05f ) );
			
			/* If we are in galaxy view, then zoom to system */
			if ( this->GetViewMode() == GALAXY && currentSystem_ ) {
				GetGameClass()->GetViewPoint()->Move ( (currentSystem_->GetPositionX() + currentSystem_->GetSun().GetPositionX() - GetGameClass()->GetViewPoint()->GetCenter().x ) / 10.f,
								       (currentSystem_->GetPositionY() + currentSystem_->GetSun().GetPositionY() - GetGameClass()->GetViewPoint()->GetCenter().y ) / 10.f );
			} else if ( deltaMove ) {
				GetGameClass()->GetViewPoint()->Move ( deltaX * zoom, deltaY * zoom );
			}
		}
	/* Zoom in */
	} else if ( direction == -1 ) {
		if ( this->GetZoomLevel() > 0.006f ) { /* minimum zoom level */
			static int j = 50;
			GetGameClass()->GetViewPoint()->Zoom ( 1 - zoomStep* ( zoomed_*0.05f ) );

			/* If we are in galaxy view, then center the camera */
			if ( this->GetViewMode() == GALAXY ) {
				j = --j<1 ? 1 : j;
				GetGameClass()->GetViewPoint()->Move ( ( - GetGameClass()->GetViewPoint()->GetCenter().x / j * zoom ),
								       ( - GetGameClass()->GetViewPoint()->GetCenter().y / j * zoom ));
			} else {
				j = 50;
				GetGameClass()->GetViewPoint()->Move ( deltaX * 0.5f * zoom, deltaY * 0.5f * zoom );
			}
		}
	}
}


double CMapManager::GetZoomLevel() const
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


CSystem* CMapManager::CreateSystem ( std::string name )
{
	CSystem* system = new CSystem ( name );
	
	if ( !systems_.empty() ) {
		CSystem* lastSystem = *(systems_.end()-1);
		system->SetPositionX ( lastSystem->GetPositionX() + lastSystem->GetSizeX() );
		system->SetPositionY ( lastSystem->GetPositionY() + lastSystem->GetSizeY() );
	}
	
	systems_.push_back ( system );	
	
	if ( currentSystem_ == NULL )
		currentSystem_ = system;

	return system;
}


CSystem& CMapManager::GetCurrentSystem()
{
	return *currentSystem_;
}


sprite::CSprite* CMapManager::AddSprite ( int systemID, sprite::CSprite* sprite )
{
	if ( !sprite )
		return NULL;
	else {
		try {
			systems_.at ( systemID )->AddSprite ( sprite );
		} catch ( std::out_of_range &e ) {
			GetGameClass()->Error ( "Could not find 'system' with ID '" + util::lCast<std::string>( systemID ) + "'", __PRETTY_FUNCTION__, __FILE__, __LINE__ );
			return NULL;
		}
	}
	
	/* Update sprite for first time */
	sprite->Update();
	
	return sprite;
}


std::vector< sprite::CSprite* >& CMapManager::GetSelectedSprites()
{
	return selectedSpriteList_;
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


VIEWMODE CMapManager::GetViewMode()
{
	return viewMode_;
}



/* PRIVATE */


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


float CMapManager::ConvertCoordsX ( float f )
{
	static sf::RenderWindow* app = GetGameClass()->GetApp();
	return app->ConvertCoords ( f, 0, GetGameClass()->GetViewPoint() ).x;
}


float CMapManager::ConvertCoordsY ( float f )
{
	static sf::RenderWindow* app = GetGameClass()->GetApp();
	return app->ConvertCoords ( 0, f, GetGameClass()->GetViewPoint() ).y;
}


