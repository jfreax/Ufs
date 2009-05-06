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

// #include <GL/glu.h>   // Header File For The GLu32 Library

// #include <GL/glew.h>/**/
// #include <GL/gl.h>

#include <SFML/Graphics.hpp>

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

class CAnimation : public sf::Drawable
{
	public:
		CAnimation ( sf::Image* image, int frames, double timePerFrame );
		
		virtual void Render ( sf::RenderTarget& ) const;

		void Start ( void );
		void Stop ( void );
		void Update ( void );

		void SetFrameWidth ( int frameWidth );

		void SetStartAt ( int startAt_ );
		void SetFrameDiff ( int diff );
		
		void SetBlur ( int blur );
	private:
		bool run_;
		sf::Clock timer_;
		int startAt_;

		sf::Image* image_;
		sf::IntRect clipArea_;

		int frameWidth_;
		int frames_;
		double timePerFrame_;

		int frame_;
		int frameDiff_;

		bool perPixel_;
		
		int blur_;

};

#endif // ANIMATION_HPP
