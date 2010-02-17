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
	gfxMarker_ = NULL;
	
	initialized = false;
	
	this->SetZoomFactor ( 1.f );
	this->CalcGFX();
	
}


CSprite::CSprite ( const sprite::CSprite& instance ) : Drawable ( instance )
{ /* TODO */
	id_ = ++globalId;
	
	background_ = NULL;
	
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
	Target.Draw ( mask_ );

	/* Calc the static background if an image exists */
	if ( !background_ )
		Target.Draw ( backgroundStatic_ );		
	else
		Target.Draw ( *background_ );
}


void CSprite::DrawMarker()
{
	/* Save static vars */
	static sf::RenderWindow* app = GetGameClass()->GetApp();
	
	app->Draw ( *gfxMarker_ );
}



void CSprite::Update()
{
	/* Scale image to zoom level */
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	this->Scale ( (1/zoom + ( zoom * GetZoomLevel() )) * this->GetZoomFactor() );
	
	if ( true ) { /* TODO Update this only when necessary */
		/* Update marker graphic */
		this->UpdateMarker();
	}
	
	if ( !initialized ) {
		if ( background_ ) {	
			this->SetCenter ( background_->GetSize().x * 0.5f,
					  background_->GetSize().y * 0.5f );
			background_->SetCenter( this->GetCenter() );
			mask_.SetCenter( this->GetCenter() );
		} else {
			this->SetCenter ( backgroundStatic_.GetSize().x * 0.5f,
					  backgroundStatic_.GetSize().y * 0.5f );
			 backgroundStatic_.SetCenter( this->GetCenter() );
		}
		initialized = true;
	}
	
	/* Animation berechnen */
	if ( background_ )
		background_->Update();
}


void CSprite::UpdateMarker()
{
	static sf::Rect< float > dim;
	dim = this->GetDimension();
	
	gfxMarker_->SetPosition ( dim.Left + dim.GetWidth() * 0.5f, dim.Top + dim.GetHeight() * 0.5f );
	gfxMarker_->SetScale ( this->GetScale().x, this->GetScale().y  );
}


void CSprite::CalcGFX()
{
	sf::Rect< float > dim;
	sf::Vector2f offset;
	double angle;
	float width, gap;
	
	dim = this->GetDimension();
	width = this->GetDimension().GetWidth() * 1.1;
	gap = width - this->GetDimension().GetWidth();
	
	
	/* Calc marker */
	gfxMarker_ = new sf::Shape();
	

}


float CSprite::GetPositionX() const
{
	return this->GetPosition().x;
}


void CSprite::SetPositionX ( float x )
{
	this->SetPosition ( x, GetPositionY() );
}


float CSprite::GetPositionY() const
{
	return this->GetPosition().y;
}


void CSprite::SetPositionY ( float y )
{
	this->SetPosition ( GetPositionX(), y );
}


unsigned int CSprite::GetId() const
{
	return id_;
}


unsigned int CSprite::GetPlayer() const
{
	return player_;
}


void CSprite::SetPlayer ( unsigned int player )
{
	player_ = player;
}



sf::Rect<float> CSprite::GetDimension() const
{
	sf::Vector2f offset;
	
	if ( background_ )
		offset = background_->GetSize();
	else
		offset = sf::Vector2f ( backgroundStatic_.GetSize() );
	
	return sf::Rect<float> ( GetPosition().x - GetCenter().x * this->GetScale().x * 2.f, GetPosition().y - GetCenter().y * this->GetScale().y * 2.f,
				 GetPosition().x - GetCenter().x * this->GetScale().x + offset.x * this->GetScale().x * 0.5f, GetPosition().y - GetCenter().y * this->GetScale().y + offset.y * this->GetScale().y * 0.5f );
}


sf::Image* CSprite::GetMiniImage() const
{
	return miniImage_;
}



void CSprite::Scale ( double scale )
{
	this->SetScale ( scale, scale );
	
}


float CSprite::GetZoomLevel() const
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


float CSprite::GetZoomFactor() const
{
	return zoomFactor_;
}



} /* namespace sprite */
