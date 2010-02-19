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
	
	infoText_ = "<b>" + name + "</b>" + "\nandere gesülz";
	
	sun_ = new sprite::CSun();
	sun_->SetPosition ( this->GetSizeX() * 0.5, this->GetSizeY() * 0.5f );
}


void CSystem::Render ( sf::RenderTarget& Target ) const
{
	Target.Draw ( *sun_ );
	
	if ( GetGameClass()->GetMapManager()->GetViewMode() == GALAXY )
		return;
	
	/* Draw sprites */
	for ( int i = 0; spriteList_.end() != spriteList_.begin()+i; ++i ) {
		Target.Draw( **(spriteList_.begin()+i) );
	}
}


void CSystem::Update()
{
	sun_->Update();
	
	if ( GetGameClass()->GetMapManager()->GetViewMode() == GALAXY )
		return;
	
	/* Update all other sprites */
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	for ( ; iter != iterEnd ; ++iter ) {
		( *iter )->Update();
	}
}



sprite::CSprite* CSystem::AddSprite ( sprite::CSprite* sprite )
{
	if ( !sprite )
		return NULL;
	else {
		spriteList_.push_back ( sprite );
	}
	
	return sprite;
}


std::vector< sprite::CSprite*, std::allocator< sprite::CSprite* > >& CSystem::GetSprites()
{
	return spriteList_;
}


sprite::CSun& CSystem::GetSun()
{
	return *sun_;
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

