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
	alpha_ = 255;
	spriteType_ = SHIP;
	
	/* Imageresourcen manager */
	CImageResource* imageResource = GetGameClass()->GetImgResource();

	/* Load picture of the ship model TODO example! */
	miniImage_ = imageResource->Get( "images/ship/human/hunter-001.png" );
	backgroundStatic_ = new sf::Sprite (  *(imageResource->Get( "images/ship/human/hunter-001.png" )) );
	
	/* Add to graphic list */
	graphics_.Add ( backgroundStatic_ );
	
	/* Set properties */
	this->SetZoomFactor( 0.04 );
	this->SetZoomLevel ( 1 );
}


void CShip::Render ( sf::RenderTarget& Target ) const
{
	/* run sprite renderer */
	CSprite::Render ( Target );

}


void CShip::Update()
{
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	if ( oldZoom_ != zoom && zoom < 0.6 ) {
		oldZoom_ = zoom;
		static sf::Color oldColor;
		oldColor = backgroundStatic_->GetColor();
		
		alpha_ = ( (zoom-0.4f)*1275.f );
		alpha_ = alpha_ < 0 ? 0 : alpha_;
		oldColor.a = alpha_;
		backgroundStatic_->SetColor ( oldColor );
	}
	
	/* run updater from sprite-class */
	CSprite::Update();
}


void CShip::SetRotation ( float Rotation )
{
	sf::Drawable::SetRotation( Rotation );

}


} /* namespace sprite */

