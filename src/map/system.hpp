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

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../engine/sprite/sprite.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <bits/stl_vector.h>


class CSystem : public sf::Drawable
{
	public:
		CSystem ();
		
		void Render ( sf::RenderTarget& Target ) const;
		void Update();
		
		sprite::CSprite* AddSprite ( sprite::CSprite* sprite );
		std::vector < sprite::CSprite* >& GetSprites();
		
		float GetPositionX() const;
		float GetPositionY() const;
		void SetPositionX ( float x );
		void SetPositionY ( float y );
		
		void SetSize ( float width, float height );
		float GetSizeX() const;
		float GetSizeY() const;
		
	private:
		std::vector < sprite::CSprite* >  spriteList_;
		
		sf::Vector2f size_;
		
};

#endif // SYSTEM_HPP
