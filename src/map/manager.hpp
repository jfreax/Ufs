/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MAPMANAGER_HPP
#define MAPMANAGER_HPP

#include <vector>
#include "../engine/sprite/sprite.hpp"
#include "../engine/ui/widget.hpp"
#include "system.hpp"

class CMapManager
{
	public:
		CMapManager() {};
		~CMapManager();
		
		void Initialize(); /* TODO */
		
		void Render();
		void Update();
		
		bool MouseClick ( const int mouseX, const int mouseY, const sf::Mouse::Button button );
		bool MouseClickReleased ( const int mouseX, const int mouseY, const sf::Mouse::Button button );
		
		void MoveZoomStep ( double step );
		void Zoom ( int direction, bool fade = false );
		double GetZoomLevel() const;
		
		void Move ( sf::Vector2f newPos );

		CSystem* CreateSystem();
		sprite::CSprite* AddSprite (  int systemID, sprite::CSprite* sprite );
		std::vector < sprite::CSprite* >& GetSelectedSprites();
		
		gui::CWidget* GetSpecialWidget ( std::string name );
		void SetSpecialWidget ( std::string name, gui::CWidget* widget );
		
		void UnSetPos();
		
		sf::Rect<float> ConvertCoords ( sf::Rect<float> rect );
		sf::Vector2f ConvertCoords ( sf::Vector2f vector );
		float ConvertCoords ( float f );
		
	private:
// 		void DrawMarked();
		

	private:
		std::vector < CSystem* > systems_;
		std::vector < sprite::CSprite* > spriteList_;
		std::vector < sprite::CSprite* > selectedSpriteList_;
		
		sf::Vector2f lastPos_;
		sf::Rect< float > selectedRect_;
		
		std::map < std::string, gui::CWidget* > specialWidget_;
		
		double drawMarkedSpecialColor_;
		
		double zoomed_;
		int lastZoomDirection_;
		
};



#endif // MAPMANAGER_HPP
