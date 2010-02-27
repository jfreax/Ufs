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

#include "../engine/sprite/sprite.hpp"
#include "../sprite/sun.hpp"
#include "../game.hpp"
#include "system.hpp"



CSystem::CSystem ( std::string name ) :
	name_ ( name )
{
	this->SetSize ( 6000, 6000 );
	
	infoText_ = name;
	
	/* Create the sun */
	sun_ = new sprite::CSun();
	sun_->SetPosition ( this->GetSizeX() * 0.5f, this->GetSizeY() * 0.5f );
	
	/* Create the tooltip window */
	tooltip_ = new gui::CSystemTooltip ( this, name );
}


void CSystem::Render ( sf::RenderTarget& Target ) const
{
	/* Or when we are on galaxy view */
	if ( GetGameClass()->GetMapManager()->GetViewMode() == GALAXY ) {
		Target.Draw ( *sun_ );
		return;
	}
	
	/* End, if this is not our current system */
	if ( &(GetGameClass()->GetMapManager()->GetCurrentSystem()) != this )
		return;
	
	/* Draw the sun */
	Target.Draw ( *sun_ );
		
	/* Draw all other sprites */
	std::list < sprite::CSprite* >::const_iterator iter = spriteList_.begin();
	std::list < sprite::CSprite* >::const_iterator iterEnd = spriteList_.end();
	for ( ; iter != iterEnd ; ++iter ) {
		Target.Draw( *(*iter) );
	}
}


void CSystem::Update()
{
	sun_->Update();
	
	/* End when we are on galaxy view */
	if ( GetGameClass()->GetMapManager()->GetViewMode() == GALAXY )
		return;
	
	/* Or end, when this is not our current system */
	if ( &(GetGameClass()->GetMapManager()->GetCurrentSystem()) != this )
		return;
	
	/* Update all other sprites */
	std::list < sprite::CSprite* >::const_iterator iter = spriteList_.begin();
	std::list < sprite::CSprite* >::const_iterator iterEnd = spriteList_.end();
	for ( ; iter != iterEnd ; ++iter ) {
		( *iter )->Update();
	}
}



sprite::CSprite* CSystem::AddSprite ( sprite::CSprite* sprite )
{
	if ( !sprite )
		return NULL;

	/* Add sprite to list */
	spriteList_.push_back ( sprite );
	
	if ( sprite->GetType() == sprite::PLANET ) {
		tooltip_->AddPlanetButton ( dynamic_cast< sprite::CPlanet* > ( sprite ) );
	}

	/* Register this sprite in system */
	sprite->SetMotherSystem ( this );
	
	return sprite;
}


std::list < sprite::CSprite* >& CSystem::GetSprites()
{
	return spriteList_;
}


sprite::CSun& CSystem::GetSun()
{
	return *sun_;
}


gui::CSystemTooltip* CSystem::GetTooltip()
{
	return tooltip_;
}




std::string CSystem::GetInfoText()
{
	return infoText_;
}


sf::Rect<float> CSystem::GetDimension() const
{
	return sf::Rect<float> ( this->GetPositionX(), this->GetPositionY(),
				 this->GetPositionX() + this->GetSizeX(), this->GetPositionY() + this->GetSizeY() );
}


// sf::Rect< float > CSystem::GetDimensionOfSun() const
// {
// 	return sf::Rect<float> ( this->GetPositionX() + sun_->GetPositionX() - sun_, this->GetPositionY() + sun_->GetPositionY(),
// 				 this->GetPositionX() + this->GetSizeX(), this->GetPositionY() + this->GetSizeY() );
// }



float CSystem::GetPositionX() const
{
	return this->GetPosition().x;
}


float CSystem::GetPositionY() const
{
	return this->GetPosition().y;
}


void CSystem::SetPositionX ( float x )
{
	this->SetPosition ( x, this->GetPositionY() );
}


void CSystem::SetPositionY ( float y )
{
	this->SetPosition ( this->GetPositionX(), y );
}


void CSystem::SetSize ( float width, float height )
{
	size_.x = width;
	size_.y = height;
}


float CSystem::GetSizeX() const
{
	return size_.x;
}


float CSystem::GetSizeY() const
{
	return size_.y;
}

