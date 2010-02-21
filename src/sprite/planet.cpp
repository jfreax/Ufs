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
#include "planet.hpp"

namespace sprite
{


CPlanet::CPlanet()
{
	spriteType_ = PLANET;
	
	/* Imageresourcen manager */
	CImageResource* imageResource = GetGameClass()->GetImgResource();
	
	/* load pictures */
	backgroundStatic_ = new sf::Sprite ( *imageResource->Get ( "images/planet/002.png" ) );

	this->SetCenter ( backgroundStatic_->GetSize().x * 0.5, backgroundStatic_->GetSize().y * 0.5 );
	backgroundStatic_->SetCenter ( GetCenter() );
	
	shadow_ = new sf::Sprite ( *imageResource->Get ( "images/planet/shadow.png" ) );
	shadow_->SetCenter ( GetCenter() );
	
	cloud1_ = new sf::Sprite ( *imageResource->Get ( "images/planet/clouds_001.png" ) );
	cloud1_->SetCenter ( GetCenter() );
	
	cloud2_ = new sf::Sprite ( *imageResource->Get ( "images/planet/clouds_002.png" ) );
	cloud2_->SetCenter ( GetCenter() );
	
	atmosphere_ = new sf::Sprite ( *imageResource->Get ( "images/planet/atmosphere_001.png" ) );
	atmosphere_->SetCenter ( GetCenter() );
	
	/* Set color of planet TODO EXAMPLE DATA! */
	planetColor_ = sf::Color ( 160, 220, 255, 255 );
	
	backgroundStatic_->SetColor ( planetColor_ );
	atmosphere_->SetColor ( planetColor_ );
	
	/* Add to graphic list */
	graphics_.Add ( backgroundStatic_ );
	graphics_.Add ( atmosphere_ );
	graphics_.Add ( cloud1_ );
	graphics_.Add ( cloud2_ );
	graphics_.Add ( shadow_ );
	
	/* Set properties */
	this->SetZoomFactor( 0.1 );
	this->SetZoomLevel ( 0.2 );
}


void CPlanet::Update ( void )
{
	double zoom = GetGameClass()->GetMapManager()->GetZoomLevel();
	if ( oldZoom_ != zoom && zoom < 0.4 ) {
		static sf::Color oldColor;
		oldZoom_ = zoom;
		
		alpha_ = ( (zoom-0.2f)*1275.f );
		alpha_ = alpha_ < 0 ? 0 : alpha_;
		
		oldColor = backgroundStatic_->GetColor();
		oldColor.a = alpha_;
		
		backgroundStatic_->SetColor ( oldColor );
		atmosphere_->SetColor ( oldColor );
		cloud1_->SetColor ( oldColor );
		cloud2_->SetColor ( oldColor );
		GetGfxMarker().SetColor ( oldColor );
		
		oldColor = shadow_->GetColor();
		oldColor.a = alpha_ == 0 ? 0 : 255;
		shadow_->SetColor ( oldColor );
	}
	
	/* Run updater from sprite-class */
	CSprite::Update();
	
	double frame = GetGameClass()->GetApp()->GetFrameTime();
	
	/* Let rotate the whole planet */
	backgroundStatic_->Rotate ( 2.f * frame );
	
	/* Rotate the clouds */
	cloud1_->Rotate ( 1.4f * frame );
	cloud2_->Rotate ( -1.4f * frame );
}


} /* namespace sprite */
