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

#include "system.hpp"


CSystem::CSystem()
{
	this->SetSize ( 8000, 8000 );
}


void CSystem::Render ( sf::RenderTarget& Target ) const
{
	/* Draw sprites */
	for ( int i = 0; spriteList_.end() != spriteList_.begin()+i; ++i )
		Target.Draw( **(spriteList_.begin()+i) );
}


void CSystem::Update()
{
	std::vector < sprite::CSprite* >::iterator iter = spriteList_.begin();
	std::vector < sprite::CSprite* >::iterator iterEnd = spriteList_.end();
	for ( ; iter != iterEnd ; ++iter )
		( *iter )->Update();
}



sprite::CSprite* CSystem::AddSprite ( sprite::CSprite* sprite )
{
	if ( !sprite )
		return NULL;
	else
		spriteList_.push_back ( sprite );	
	
	return sprite;
}


std::vector< sprite::CSprite*, std::allocator< sprite::CSprite* > >& CSystem::GetSprites()
{
	return spriteList_;
}


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