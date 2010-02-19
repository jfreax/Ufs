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


#include "../game.hpp"
#include "sun.hpp"


namespace sprite
{


CSun::CSun ()
{
	spriteType_ = SUN;
	
	/* Imageresourcen manager */
	CImageResource* imageResource = GetGameClass()->GetImgResource();

	/* load picture of a sun */
	background_ = new CAnimation ( imageResource->Get( "images/sun/001.png" ), 0, 0.05f );
	
	this->SetCenter ( background_->GetSize().x * 0.5, background_->GetSize().y * 0.5 );
	backgroundStatic_.SetCenter ( GetCenter() );
	
	/* Set blendmode */
	background_->SetBlendMode ( sf::Blend::Multiply );

	/* Bild wird nur auf diesem Bereich gezeichnet */
	mask_ = sf::Shape::Circle ( 250, 250, 250, sf::Color ( 255, 255, 255 ) );
	
	/* picture of sun glow */
	glow_.SetImage( *imageResource->Get ( "images/sun/sun_glow.png" ) );
	glow_.SetPosition( -83, -83 );
	
	/* storm picture */
	brightness_.SetImage( *imageResource->Get ( "images/sun/sun_storm.png" ) );
	brightness_.SetPosition( 10, 10 );	

	brightness_.SetColor( sf::Color ( 255, 255, 255, 100 ) );
	brightness_.SetBlendMode( sf::Blend::Alpha );
	
	/* Set center of sprites */
	glow_.SetCenter ( GetCenter() );
	brightness_.SetCenter ( GetCenter() );
	
	/* Set properties */
	this->SetZoomFactor( 0.4 );
	this->SetZoomLevel ( 0.02 );
	
	showGlow_ = 0;
	galaxyGlow_ = NULL;
	
	markerWidth_ = this->GetDimension().GetWidth() * 0.7;
	this->CalcGFX();
	
}




void CSun::Render ( sf::RenderTarget& Target ) const
{
	/* run sprite renderer */
	CSprite::Render( Target );
	
	/* draw sun glow */
	Target.Draw ( glow_ );
	
	if ( showGlow_ )
		Target.Draw ( *galaxyGlow_ );
}


void CSun::Update ( void )
{
	/* Run updater from sprite-class */
	CSprite::Update();
	
	if ( showGlow_ > 0.f ) {
		showGlow_ -= GetGameClass()->GetApp()->GetFrameTime() * 1000;
		galaxyGlow_->SetColor( sf::Color ( 255, 255, 255, showGlow_ ) );
	}
	if ( showGlow_ < 0.f )
		showGlow_ = 0.f;
	
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	if ( oldZoom_ != zoom && zoom < 0.2f ) {
		sf::Color oldColor = gfxMarker_->GetColor();
		oldColor.a = 0;
		gfxMarker_->SetColor( oldColor );
		
		this->Scale ( zoom + 1.8f );
	} else {
		sf::Color oldColor = gfxMarker_->GetColor();
		oldColor.a = 255;
		gfxMarker_->SetColor( oldColor );
	}
	
}


void CSun::CalcGFX()
{
	sprite::CSprite::CalcGFX();
	
	sf::Rect< float > dim;
	sf::Vector2f offset;
	double angle, gap;
	float width = markerWidth_ * 2.f;
	
	gap = 500.f;
	
	sf::Color color = sf::Color ( 130, 130, 50, 255 ); 
	sf::Vector2f center ( 0, 0 );
	
	/* Calc marker */
	delete galaxyGlow_;
	galaxyGlow_ = new sf::Shape();
	galaxyGlow_->EnableFill ( false );
	galaxyGlow_->EnableOutline ( true );
	galaxyGlow_->SetOutlineWidth ( gap * 0.1f );
	
	for ( int j = 5; j; --j ) {
		width -= gap * 0.1f;
		color.a = j*40;
		
		for ( int i = 0; i < 180; ++i ) {
			angle = i * 2 * 3.141592654f / 180.f;
			offset = sf::Vector2f ( cos ( angle ), sin ( angle ) );
			
			galaxyGlow_->AddPoint ( center + offset * width, color, color );
		}
	}
}


sf::Color CSun::GetColor()
{
	return background_->GetColor();
}


void CSun::SetColor ( sf::Color color )
{
	background_->SetColor ( color );
	glow_.SetColor ( color );
}


void CSun::ShowGlow()
{
	if ( showGlow_ < 255 )
		showGlow_ += GetGameClass()->GetApp()->GetFrameTime() * 2000.f;
	if ( showGlow_ > 255 )
		showGlow_ = 255;
}



} /* namespace sprite */

