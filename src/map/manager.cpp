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
#include "../gui/window/systemtooltip.hpp"
#include "../gui/button/planetbutton.hpp"
#include "../game.hpp"
#include "manager.hpp"


CMapManager::~CMapManager()
{
	std::list < CSystem* >::const_iterator sysIter = systems_.begin();
	std::list < CSystem* >::const_iterator sysIterEnd = systems_.end();
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
	zoomTo_ = NULL;
	
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
	std::list < CSystem* >::const_iterator sysIter = systems_.begin();
	std::list < CSystem* >::const_iterator sysIterEnd = systems_.end();
	for ( ; sysIter != sysIterEnd ; ++sysIter )
		app->Draw( **sysIter );

	/* Draw a circle at selected sprites */
	std::list < sprite::CSprite* >::const_iterator iter = selectedSpriteList_.begin();
	std::list < sprite::CSprite* >::const_iterator iterEnd =selectedSpriteList_.end();
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
	std::list < CSystem* >::const_iterator sysIter = systems_.begin();
	std::list < CSystem* >::const_iterator sysIterEnd = systems_.end();
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
					std::list < sprite::CSprite* >::const_iterator iter = currentSystem_->GetSprites().begin();
					std::list < sprite::CSprite* >::const_iterator iterEnd = currentSystem_->GetSprites().end();
					for ( ; iter != iterEnd ; ++iter ) {
						sf::Rect< float > selectedRectInGameCoord = this->ConvertCoords ( selectedRect_ );
						if ( ( *iter )->GetDimensionInGalaxy().Intersects ( selectedRectInGameCoord ) ) {
							selectedSpriteList_.push_back ( *iter );
						}
					
						if ( this->GetSpecialWidget ( "MINI_OBJECT" ) )
							this->GetSpecialWidget ( "MINI_OBJECT" )->Call();
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
				
				/* Mouse is over tooltip window */
				gui::CWindow* tooltip = currentSystem_->GetTooltip();
				if ( tooltip->GetShow() && tooltip->GetWindowDimension().Contains ( mouseX, mouseY ) ) {
					
					tooltip->onUnHoverMouse();
					
					/* Widgets prüfen */
					std::vector< gui::CWidget* >* widgetList = tooltip->GetWidgetList();
					gui::CWidget* currentWidget = NULL;
					
					for ( std::vector< gui::CWidget* >::size_type i = widgetList->size(); i; --i ) {
						currentWidget = widgetList->at ( i - 1 );
						
						/* Wenn Widget angezeigt werden soll, minimale Klickzeitabstand vorbei ist
						und die Maus an der richtigen Position ist, dann Aktion ausführen */
						if ( currentWidget->GetShow() && currentWidget->GetDimensionInScreen().Contains ( mouseX, mouseY )  && currentWidget->GetType() ) {
							zoomTo_ = (dynamic_cast< gui::CPlanetButton* > ( currentWidget ))->planet;
							break;
						}
					}
				}
				
				this->Zoom ( 1.f );
			}

			break;
			
	}
}


bool CMapManager::MouseHover ( const int mouseX, const int mouseY )
{
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	int x = this->ConvertCoordsX ( mouseX );
	int y = this->ConvertCoordsY ( mouseY );
	
	std::list < CSystem* >::const_iterator sysIter = systems_.begin();
	std::list < CSystem* >::const_iterator sysIterEnd = systems_.end();
	for ( ; sysIter != sysIterEnd ; ++sysIter ) {
		/* When we are on galaxy view... */
		if ( this->GetViewMode() == GALAXY && zoom < 0.006f ) {
			
			/* ... and mouse is over tooltip window */
			gui::CWindow* tooltip = (*sysIter)->GetTooltip();
			if ( tooltip->GetShow() && tooltip->GetWindowDimension().Contains ( mouseX, mouseY ) ) {
				tooltip->onUnHoverMouse();
			} else if ( (*sysIter)->GetDimension().Contains( x, y ) ) { /* ... or the mouse is over a sun... */
				/* ... then show tooltip */
				(*sysIter)->GetTooltip()->Show ( mouseX, mouseY );
				
				/* and show glow of the sun */
				(*sysIter)->GetSun().ShowGlow();
					
				currentSystem_ = (*sysIter);
				zoomTo_ = &currentSystem_->GetSun();
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
			if ( this->GetViewMode() == GALAXY && zoomTo_ ) {
				GetGameClass()->GetViewPoint()->Move ( (currentSystem_->GetPositionX() + zoomTo_->GetPositionX() - GetGameClass()->GetViewPoint()->GetCenter().x ) / 10.f,
								       (currentSystem_->GetPositionY() + zoomTo_->GetPositionY() - GetGameClass()->GetViewPoint()->GetCenter().y ) / 10.f );
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
		std::list < CSystem* >::const_iterator sysIterMin;;
		std::list < CSystem* >::const_iterator sysIter = systems_.begin();
		std::list < CSystem* >::const_iterator sysIterEnd = systems_.end();
		for ( ; sysIter != sysIterEnd ; ++sysIter ) {
			sysIterMin = sysIter;
		}
		CSystem* lastSystem = *(sysIterMin);
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
			std::list < CSystem* >::const_iterator sysIter = systems_.begin();
			std::list < CSystem* >::const_iterator sysIterEnd = systems_.end();
			for ( int i = 0; sysIter != sysIterEnd ; ++sysIter, ++i ) {
				if ( i == systemID )
					(*sysIter)->AddSprite ( sprite );
			}
// 			systems_.at ( systemID )->AddSprite ( sprite );
		} catch ( std::out_of_range &e ) {
			GetGameClass()->Error ( "Could not find 'system' with ID '" + util::lCast<std::string>( systemID ) + "'", __PRETTY_FUNCTION__, __FILE__, __LINE__ );
			return NULL;
		}
	}
	
	/* Update sprite for first time */
	sprite->Update();
	
	return sprite;
}


std::list< sprite::CSprite* >& CMapManager::GetSelectedSprites()
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


/* ---- For scripts ---- */


float CMapManager::CalcDistance ( sf::Vector2f vec1, sf::Vector2f vec2 )
{
	return  sqrt ( pow ( ( vec2.x - vec1.x ), 2 ) + pow ( ( vec2.y - vec1.y ), 2 ) );
	
}


float CMapManager::CalcDistance ( sf::Vector2f vec, sprite::CSprite* sprite )
{
	this->CalcDistance ( vec, sf::Vector2f ( sprite->GetPositionX() + sprite->GetMotherSystem().GetPositionX(), sprite->GetPositionY() + sprite->GetMotherSystem().GetPositionY() ) );
}


float CMapManager::CalcDistance ( sprite::CSprite* sprite1, sprite::CSprite* sprite2 )
{
	this->CalcDistance ( sf::Vector2f ( sprite1->GetPositionInGalaxy().x, sprite1->GetPositionInGalaxy().y ) , 
			     sf::Vector2f ( sprite2->GetPositionInGalaxy().x, sprite2->GetPositionInGalaxy().y ) );
}


sprite::CPlanet& CMapManager::FindNextPlanet ( sprite::CSprite* sprite )
{
	if ( sprite->GetType() == sprite::PLANET )
		return *dynamic_cast < sprite::CPlanet* > (sprite);
	
	static float cur;
	static sprite::CSprite* curPlanet;
	curPlanet = NULL;
	float min = 0.f;
	
	std::list < sprite::CSprite* >::const_iterator iter = sprite->GetMotherSystem().GetSprites().begin();
	std::list < sprite::CSprite* >::const_iterator iterEnd = sprite->GetMotherSystem().GetSprites().end();
	for ( ; iter != iterEnd ; ++iter ) {
		if ( (*iter)->GetType() == sprite::PLANET ) {
			cur = CalcDistance ( (*iter), sprite );
			
			if ( cur < min ) {
				min = cur;
				curPlanet = (*iter);
			}
		}
	}
	
	return *dynamic_cast < sprite::CPlanet* > (curPlanet);
}

