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

#include <vector>

#include <SFML/Graphics.hpp>
#include "animation.hpp"

#ifndef GFX_GRAPHIC_HPP
#define GFX_GRAPHIC_HPP


class CGraphic : public sf::Drawable
{
	public:
		CGraphic();
		~CGraphic();
		
		virtual void Render ( sf::RenderTarget& Target ) const;
		
		void Clear();
		
		sf::Sprite* Add ( sf::Image* image );
		sf::Sprite* Add ( sf::Sprite* object );
		sf::Drawable* Add ( sf::Drawable* object );
		
		sf::Vector2f GetSize() const;
	
	private:
		std::vector < sf::Drawable* > objects_;
		
		sf::Vector2f size_;
	
};

#endif // GRAPHIC_HPP
