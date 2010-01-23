/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Publicturbine_1.3ds
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

#include "sprite.hpp"
#include "../../settings/settings.hpp"
#include "../../game.hpp"


namespace sprite
{

static unsigned int globalId = 0;

CSprite::CSprite()
{
	id_ = ++globalId;
	
	player_ = 0; /* "NATURE" */
	
	background_ = NULL;
	miniImage_  = NULL;
	bgMarker_ = NULL;
	
	this->SetZoomFactor ( 1.f );
	
	initialized = false;
}


CSprite::CSprite ( const sprite::CSprite& instance ) : Drawable ( instance )
{ /* TODO */
	id_ = ++globalId;
	
// 	background_ = new CAnimation (  instance.background_->, 1, 0.05f );
	background_ = instance.background_;
	
	player_ = instance.player_;
	mask_ = instance.mask_;
	zoomLevel_ = instance.zoomLevel_;
	
}



CSprite::~CSprite()
{
	delete background_;
}



void CSprite::Render ( sf::RenderTarget& Target ) const
{
	if ( !background_ )
		return;
	
	Target.Draw ( mask_ );
	Target.Draw ( *background_ );
}


void CSprite::DrawMarker()
{
	/* Save static vars */
	static sf::RenderWindow* app = GetGameClass()->GetApp();

	app->Draw ( *bgMarker_ );
}



void CSprite::Update()
{
	if ( !background_ )
		return;
	
	if ( !initialized ) {
		this->SetCenter ( background_->GetSize().x*0.5f, background_->GetSize().y*0.5f );
		initialized = true;
	}
	
	if ( true ) { /* update this only when necessary */
		/* Update marker graphic */
		this->UpdateMarker();
	}
	
	/* Animation berechnen */
	background_->Update();
	
	/* Scale image to zoom level */
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	this->Scale ( (1/zoom + ( zoom * GetZoomLevel() )) * this->GetZoomFactor() );
}


void CSprite::UpdateMarker()
{
	static sf::Rect< float > dim;
	static sf::Vector2f offset;
	static double angle;
	static float width;
	
	dim = this->GetDimension();
	width = this->GetDimension().GetWidth();
	
	/* Delete and create then a new shape */
	delete bgMarker_; /* TODO */
	bgMarker_ = new sf::Shape();
	bgMarker_->EnableFill(false);
	bgMarker_->EnableOutline(true);
	
	
	/* Calc the outline width */
	float zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	if ( zoom < 1.f ) zoom = 1.f;
	bgMarker_->SetOutlineWidth ( (this->GetDimension().GetWidth() * (this->GetZoomLevel() +0.5)) / 4 / zoom );
	
	/* Set variables */
	sf::Vector2f center ( dim.Left + dim.GetWidth() * 0.5f, dim.Top + dim.GetHeight() * 0.5f );
	sf::Color color = sf::Color ( 30, 30, 50, 255 ); /* TODO different colors for friend/enemy/neutral... */
	
	/* Calc a offset for rotation */
	static double iOffset = 0;
	iOffset += GetGameClass()->GetApp()->GetFrameTime() * 10;
	
	/* Add points */
	for ( int i = 0; i < 160; ++i ) {
		angle = (i+iOffset) * 2 * 3.141592654f / 160;
		offset = sf::Vector2f ( cos ( angle ), sin ( angle ) );
		
		if ( i > 40 && i < 80 )
			color.a = 255 - (i*2);
		else if ( i > 80 && i < 120 )
			color.a = 15 + (i*2);
		else if ( i > 120 && i < 160 )
			color.a = 255 - ((i-80)*2);
		
		bgMarker_->AddPoint ( center + offset * (width+10/zoom), color, color );
	}
	
}



unsigned int CSprite::GetId()
{
	return id_;
}


unsigned int CSprite::GetPlayer()
{
	return player_;
}


void CSprite::SetPlayer ( unsigned int player )
{
	player_ = player;
}



sf::Rect<float> CSprite::GetDimension()
{
	return sf::Rect<float> ( GetPosition().x - GetCenter().x * this->GetScale().x, GetPosition().y - GetCenter().y * this->GetScale().y,
				 GetPosition().x - GetCenter().x * this->GetScale().x + background_->GetSize().x * this->GetScale().x, GetPosition().y - GetCenter().y * this->GetScale().y + background_->GetSize().y * this->GetScale().y );
}


sf::Image* CSprite::GetMiniImage()
{
	return miniImage_;
}



void CSprite::Scale ( double scale )
{
	this->SetScale ( scale, scale );
	
}


float CSprite::GetZoomLevel()
{
	return zoomLevel_;
}


void CSprite::SetZoomLevel ( float zLevel )
{
	zoomLevel_ = zLevel;
}


void CSprite::SetZoomFactor ( float factor ) {
	zoomFactor_ = factor;
}


float CSprite::GetZoomFactor() {
	return zoomFactor_;
}


} /* namespace sprite */
