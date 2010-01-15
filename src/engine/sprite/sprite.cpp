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
	
	this->SetZoomFactor ( 1.f );
	
	initialized = false;
// 	std::cout << this->GetDimension().GetWidth() << std::endl;
// 	this->SetCenter ( 71, 0 );
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
	static sf::Rect< float > dim;
	static sf::Shape circle, bg;
	
	
	dim = this->GetDimension();
// 	dim.Left += this->GetCenter().x;
// 	dim.Top += this->GetCenter().y;
	
// 	circle = sf::Shape::Circle ( dim.Left + dim.GetWidth() * 0.5f, dim.Top + dim.GetHeight() * 0.5f, dim.GetWidth() * 0.5f + 5, sf::Color ( 30, 30, 50, 150 ), 1, sf::Color ( 20, 20, 60,200 ) );
// 	
// 	// 		drawMarkedSpecialColor += app->GetFrameTime()*10;
// 	// 		if ( drawMarkedSpecialColor+1 > circle.GetNbPoints() )
// 	// 			drawMarkedSpecialColor = 1;
// 	// 		
// 	// 		circle.SetPointColor ( (int)drawMarkedSpecialColor+1, sf::Color ( 40, 30, 50, 100 ) );
// 	// 		circle.SetPointColor ( (int)drawMarkedSpecialColor-1, sf::Color ( 40, 30, 50, 100 ) );
// 	// 		circle.SetPointColor ( (int)drawMarkedSpecialColor, sf::Color ( 40, 30, 50, 100 ) );
// 	
// 	app->Draw ( circle );

	float Angle;
	sf::Vector2f Center ( 20, 20 );
	for ( int i = 0; i < 80; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		bg.AddPoint ( Center + Offset * 20.f, sf::Color ( 100,40,200) );
	}
}



void CSprite::Update()
{
	if ( !background_ )
		return;
	
	if ( !initialized ) {
		this->SetCenter ( background_->GetSize().x*0.5f, background_->GetSize().y*0.5f );
		initialized = true;
	}
	
	
	/* Animation berechnen */
	background_->Update();
	
	/* Scale image to zoom level */
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	this->Scale ( (1/zoom + ( zoom * GetZoomLevel() )) * this->GetZoomFactor() );

// 	std::cout << this->GetDimension().GetWidth() - oldWidth << std::endl;
// 	this->Scale ( 1/zoom*GetZoomLevel() );
	
	
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
	return sf::Rect<float> ( GetPosition().x, GetPosition().y,
				 GetPosition().x + background_->GetSize().x * this->GetScale().x, GetPosition().y + background_->GetSize().y * this->GetScale().y );
}


sf::Image* CSprite::GetMiniImage()
{
	return miniImage_;
}



void CSprite::Scale ( double scale )
{
	float verh = (float)settings::GetHeight() / (float)settings::GetWidth();
	this->SetScale ( scale * verh, scale );
	
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
