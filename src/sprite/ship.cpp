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
	background_ = new CAnimation ( imageResource->Get( "images/ship/human/hunter-001.png" ), 1, 0.05f );
	background_->SetCenter ( GetCenter() );
	
	
	/* Set properties */
	SetZoomLevel ( .4 );
// 	zoomLevel_ = 0.1;
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
}

}

