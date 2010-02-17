/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../game.hpp"
#include "ship.hpp"

namespace sprite
{

CShip::CShip()
{


	
	/* Imageresourcen manager */
	CImageResource* imageResource = GetGameClass()->GetImgResource();

	/* Load picture of the ship model TODO example! */
	miniImage_ = imageResource->Get( "images/ship/human/hunter-001.png" );
	backgroundStatic_.SetImage ( *imageResource->Get( "images/ship/human/hunter-001.png" ) );
// 	background_ = new CAnimation ( imageResource->Get( "images/ship/human/hunter-001.png" ), 1, 0.05f );
// 	background_->SetCenter ( GetCenter() );
	
	/* Set properties */
	this->SetZoomFactor( 0.04 );
	this->SetZoomLevel ( 1 );
	
	/* Testwerte! */
	nbShield_ = 4;
	
	gfxShields_ = new sf::Shape[nbShield_];
	shieldPos_ = new int[nbShield_];
	shieldPos_[0] = 45;
	shieldPos_[1] = 90;
	shieldPos_[2] = 135;
	shieldPos_[3] = 180;
	shieldRotation_ = 45; /* rotation */
	
	this->CalcGFX();
	
}


void CShip::Render ( sf::RenderTarget& Target ) const
{
	/* run sprite renderer */
	CSprite::Render ( Target );

	if ( true ) { /* Shield active! */
		for ( int i = 0; i < nbShield_; ++i ) {
			Target.Draw ( gfxShields_[i] );
		}
	}

}


void CShip::Update()
{
	/* run updater from sprite-class */
	CSprite::Update();
}


void CShip::CalcGFX()
{
	/* Call the method from CSprite() */
	sprite::CSprite::CalcGFX();
	
	/* Declare some vars */
	sf::Rect< float > dim;
	sf::Vector2f offset;
	double angle;
	float width, gap;
	
	dim = this->GetDimension();
	width = this->GetDimension().GetWidth() * 1.3;
	gap = width - this->GetDimension().GetWidth() * 1.2;
	
	sf::Color color = sf::Color ( 30, 30, 50, 255 ); /* TODO Player color */
	sf::Vector2f center ( 0, 0 );
	
	/* Calc shield gfx */
	for ( int s = 0; s < nbShield_; ++s ) {
		width = this->GetDimension().GetWidth() * 1.3;
// 		gfxShields_[s] = new sf::Shape();
		gfxShields_[s].EnableFill ( false );
		gfxShields_[s].EnableOutline ( true );
		gfxShields_[s].SetOutlineWidth ( gap * 0.1f );
	
		for ( int j = 20; j; --j ) {
			width -= gap * 0.1f;
		
			color.a = j*10;
			int alpha = color.a;
		
			for ( int i = (s!=0? shieldPos_[s-1]:0); i < 180; ++i ) {
				/* Do not draw over the limit */
				if ( i > shieldPos_[s]-5 ) {
					color.a = 0;
					
					angle = i * 2 * 3.141592654f / 180;
					offset = sf::Vector2f ( cos ( angle ), sin ( angle ) );
					gfxShields_[s].AddPoint ( center + offset * width, color, color );
					
					angle = (s!=0? shieldPos_[s-1]:0) * 2 * 3.141592654f / 180;
					offset = sf::Vector2f ( cos ( angle ), sin ( angle ) );
					gfxShields_[s].AddPoint ( center + offset * width, color, color );
					
					i = 180;
					
				} else {
					angle = i * 2 * 3.141592654f / 180;
					offset = sf::Vector2f ( cos ( angle ), sin ( angle ) );
			
					gfxShields_[s].AddPoint ( center + offset * width, color, color );
				}
			}
		}
		gfxShields_[s].Rotate( shieldRotation_ );
	}
	
}


void CShip::SetRotation ( float Rotation )
{
	sf::Drawable::SetRotation( Rotation );
	
	
}



} /* namespace sprite */

