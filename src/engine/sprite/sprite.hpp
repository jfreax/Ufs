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

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../animation.hpp"


namespace sprite
{


class CSprite : public sf::Drawable
{
	public:
		CSprite();
		CSprite ( const CSprite& instance );
		
		~CSprite();
		
		virtual void Render ( sf::RenderTarget& Target ) const;
		virtual void DrawMarker();
		
		virtual void Update();
		
		unsigned int GetId();
		
		unsigned int GetPlayer();
		void SetPlayer ( unsigned int player );
		
		sf::Rect<float> GetDimension();
		sf::Image* GetMiniImage();
		
		void Scale ( double scale );
		
		void SetZoomLevel ( float zLevel );
		float GetZoomLevel();
		
		void SetZoomFactor ( float factor );
		float GetZoomFactor();

	protected:
		unsigned int id_;
		unsigned int player_;
		
		CAnimation* background_;
		sf::Image* miniImage_;
		sf::Shape mask_;
		
		/*  */
		float zoomLevel_;
		float zoomFactor_;
};


} /* namespace sprite */

#endif /* SPRITE_HPP */
