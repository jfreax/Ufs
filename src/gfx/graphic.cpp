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

#include "graphic.hpp"


CGraphic::CGraphic()
{
	size_.x = size_.y = 0.f;
}


CGraphic::~CGraphic()
{
	for ( int i = 0; objects_.end() != objects_.begin()+i; ++i ) {
		delete *(objects_.begin()+i);
	}
}




void CGraphic::Render ( sf::RenderTarget& Target ) const
{
	for ( int i = 0; objects_.end() != objects_.begin()+i; ++i ) {
		Target.Draw ( **(objects_.begin()+i) );
	}
}


void CGraphic::Clear()
{
	for ( int i = 0; objects_.end() != objects_.begin()+i; ++i ) {
		delete *(objects_.begin()+i);
	}
	objects_.clear();
}


sf::Sprite* CGraphic::Add ( sf::Image* image )
{
	sf::Sprite* sprite = new sf::Sprite ( *image );
	this->Add ( sprite );
}


sf::Sprite* CGraphic::Add ( sf::Sprite* object )
{
	if ( !object )
		return NULL;
	
	/* Adjust size */
	if ( object->GetSize().x > size_.x )
		size_.x = object->GetSize().x;
	if ( object->GetSize().y > size_.y )
		size_.y = object->GetSize().y;
	
	objects_.push_back ( object );
	return object;
}


sf::Drawable* CGraphic::Add ( sf::Drawable* object )
{
	if ( !object )
		return NULL;
	
	objects_.push_back ( object );
	return object;
}


sf::Vector2f CGraphic::GetSize() const
{
	return size_;
}


