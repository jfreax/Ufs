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
	alpha_ = 255;
	
	markerRotation_ = 0;
	
	motherSystem_ = NULL;
	background_ = NULL;
	miniImage_  = NULL;
	
	initialized_ = false;
	
	this->SetZoomFactor ( 1.f );

	
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
	
	this->UpdateMarker();
	
	app->Draw ( GetGfxMarker() );
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
	
	if ( !initialized_ ) {
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
		initialized_ = true;
	}
	
	/* Animation berechnen */
	if ( background_ )
		background_->Update();
	
	/* Fade out */
	if ( oldZoom_ != zoom && zoom < 0.6f ) {
		static int alpha;
		
		static sf::Color oldColor;
	
	}
}


void CSprite::UpdateLogic()
{
	/* TODO */
}


void CSprite::UpdateMarker()
{
	static sf::Rect< float > dim;
	dim = this->GetDimensionInGalaxy();
	
	/* Set color */
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	if ( zoom < 0.6 ) {
		static sf::Color color = GetGfxMarker().GetColor();
		
		alpha_ = ( (zoom-0.4f)*1275.f );
		alpha_ = alpha_ < 0 ? 0 : alpha_;
		color.a = alpha_;
		
		GetGfxMarker().SetColor ( color );
	}
	
	/* Set position and size */
	GetGfxMarker().SetPosition ( dim.Left + dim.GetWidth() * 0.5f, dim.Top + dim.GetHeight() * 0.5f );
	GetGfxMarker().SetScale ( this->GetScale().x * this->GetDimension().GetWidth(), this->GetScale().y * this->GetDimension().GetWidth() );
	
 	/* Rotate */
	markerRotation_ += GetGameClass()->GetApp()->GetFrameTime() * 50;
	GetGfxMarker().SetRotation ( markerRotation_ );
}


void CSprite::SetMotherSystem ( CSystem* sys )
{
	motherSystem_ = sys;
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


SPRITETYPE CSprite::GetType() const
{
	return spriteType_;
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
		offset = backgroundStatic_.GetSize();
	
	return sf::Rect<float> ( GetPosition().x - GetCenter().x * this->GetScale().x * 2.f, GetPosition().y - GetCenter().y * this->GetScale().y * 2.f,
				 GetPosition().x - GetCenter().x * this->GetScale().x + offset.x * this->GetScale().x * 0.5f, GetPosition().y - GetCenter().y * this->GetScale().y + offset.y * this->GetScale().y * 0.5f );
}


sf::Rect< float > CSprite::GetDimensionInGalaxy() const
{
	if ( !motherSystem_ )
		return GetDimension();
	
	return sf::Rect<float> ( GetDimension().Left + motherSystem_->GetPositionX(), GetDimension().Top + motherSystem_->GetPositionY(),
				 GetDimension().Right + motherSystem_->GetPositionX(), GetDimension().Bottom + motherSystem_->GetPositionY() );
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


/* ----------- */
sf::Shape gfxMarker_;

void Initialize()
{
	sf::Vector2f offset;
	double angle, gap;
	float width = 15.f;
	gap = 1.5f;
	
	sf::Color color = sf::Color ( 30, 30, 50, 255 ); /* TODO Player color */
	sf::Vector2f center ( 0, 0 );
	
	/* Calc marker */
	gfxMarker_.EnableFill ( false );
	gfxMarker_.EnableOutline ( true );
	gfxMarker_.SetOutlineWidth ( gap * 0.1f );
	
	int alpha;
	for ( int j = 20; j; --j ) {
		width -= gap * 0.1f;
		
		color.a = j*10;
		alpha = color.a;
		
		for ( int i = 0; i < 180; ++i ) {
			if ( i > 0 && i < 5 ||
				i > 45 && i < 50 ||
				i > 90 && i < 95 ||
				i > 135 && i < 140 )
				color.a = 0;
			else color.a = alpha;
			
			angle = i * 2 * 3.141592654f / 180;
			offset = sf::Vector2f ( cos ( angle ), sin ( angle ) );
			
			gfxMarker_.AddPoint ( center + offset * width, color, color );
		}
	}
}


sf::Shape& GetGfxMarker()
{
	return gfxMarker_;
}



} /* namespace sprite */
