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
	/* Imageresourcen manager */
	CImageResource* imageResource = GetGameClass()->GetImgResource();
	
	/* load pictures */
	backgroundStatic_.SetImage( *imageResource->Get ( "images/planet/002.png" ) );

	this->SetCenter ( backgroundStatic_.GetSize().x * 0.5, backgroundStatic_.GetSize().y * 0.5 );
	backgroundStatic_.SetCenter ( GetCenter() );
	
	shadow_.SetImage( *imageResource->Get ( "images/planet/shadow.png" ) );
	shadow_.SetCenter ( GetCenter() );
	
	cloud1_.SetImage( *imageResource->Get ( "images/planet/clouds_001.png" ) );
	cloud1_.SetCenter ( GetCenter() );
	cloud2_.SetImage( *imageResource->Get ( "images/planet/clouds_002.png" ) );
	cloud2_.SetCenter ( GetCenter() );
	
	atmosphere_.SetImage( *imageResource->Get ( "images/planet/atmosphere_001.png" ) );
	atmosphere_.SetCenter ( GetCenter() );
	
	/* Set color of planet TODO EXAMPLE DATA! */
	planetColor_ = sf::Color ( 160, 220, 255, 255 );
	
	backgroundStatic_.SetColor ( planetColor_ );
	atmosphere_.SetColor ( planetColor_ );
	
// 	cloud1_.SetColor( sf::Color ( 255, 255, 255, 60 ) );
	
	/* Set properties */
	this->SetZoomFactor( 0.1 );
	this->SetZoomLevel ( 0.2 );
}


void CPlanet::Render ( sf::RenderTarget& Target ) const
{
	/* Run sprite renderer */
	sprite::CSprite::Render ( Target );

	/* Render the atmosphere */
	Target.Draw ( atmosphere_ );
	
	/* Draw clouds */
	Target.Draw ( cloud1_ );
	Target.Draw ( cloud2_ );
	
	/* Draw shadow */
	Target.Draw ( shadow_ );
	
}


void CPlanet::Update ( void )
{
	/* Run updater from sprite-class */
	CSprite::Update();
	
	double frame = GetGameClass()->GetApp()->GetFrameTime();
	
	/* Let rotate the whole planet */
	backgroundStatic_.Rotate ( 2.f * frame );
	
	/* Rotate the clouds */
	cloud1_.Rotate ( 1.4f * frame );
	cloud2_.Rotate ( -1.4f * frame );
}


} /* namespace sprite */
