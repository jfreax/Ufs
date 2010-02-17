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
// 	nbShield_ = 4;
	
// 	gfxShields_ = new sf::Shape[nbShield_];
// 	shieldPos_ = new int[nbShield_];
// 	shieldPos_[0] = 45;
// 	shieldPos_[1] = 90;
// 	shieldPos_[2] = 135;
// 	shieldPos_[3] = 180;
// 	shieldRotation_ = 45; /* rotation */
	
	this->CalcGFX();
	
}


void CShip::Render ( sf::RenderTarget& Target ) const
{
	/* run sprite renderer */
	CSprite::Render ( Target );

}


void CShip::Update()
{
	/* run updater from sprite-class */
	CSprite::Update();
	
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	if ( oldZoom_ != zoom && zoom < 0.4 ) {
		static int alpha;
		
		oldZoom_ = zoom;
		static sf::Color oldColor;
		oldColor = backgroundStatic_.GetColor();
		
		alpha = ( (zoom-0.2f)*1275.f );
		alpha = alpha < 0 ? 0 : alpha;
		oldColor.a = alpha;
		
		std::cout << (int)oldColor.a  << std::endl;
		
		backgroundStatic_.SetColor ( oldColor );	
	}
}


void CShip::SetRotation ( float Rotation )
{
	sf::Drawable::SetRotation( Rotation );

}


} /* namespace sprite */

