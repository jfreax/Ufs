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

#include <iostream>

#include "game.hpp"
#include "animation.hpp"

CAnimation::CAnimation ( sf::Image* image_, int frames_, float timePerFrame_ ) :
		image ( image_ ), frames ( frames_ ), timePerFrame ( timePerFrame_ ), frame ( 1 ), startAt ( 1 )
{
	frameWidth = image->GetWidth() / frames;
	clipArea = sf::IntRect ( 0, 0, frameWidth, image->GetHeight() );

	this->Start();
}
// // 

void CAnimation::Start ( void )
{
//   timer.Reset();
	frame = startAt;
	run = true;
}


void CAnimation::Stop ( void )
{
	run = false;
}


void CAnimation::Update ( void )
{
	if ( timer.GetElapsedTime() >= timePerFrame )
	{
		timer.Reset();

		if ( ++frame > frames )
		{
			frame = startAt;
		}

		clipArea = sf::IntRect ( frameWidth * ( frame - 1 ), 0, frameWidth * frame, image->GetHeight() );

	}
}


void CAnimation::Render ( sf::RenderTarget& ) const
{
	if ( run )
	{
		float width  = static_cast<float> ( clipArea.GetWidth() );
		float height = static_cast<float> ( clipArea.GetHeight() );

		sf::FloatRect rect = image->GetTexCoords ( clipArea );

		image->Bind();

		// Draw the animation's triangles
		glBegin ( GL_QUADS );
		{
			glTexCoord2f ( rect.Left,  rect.Top );
			glVertex2f ( 0,     0 );

			glTexCoord2f ( rect.Left,  rect.Bottom );
			glVertex2f ( 0,     height );

			glTexCoord2f ( rect.Right, rect.Bottom );
			glVertex2f ( width, height );

			glTexCoord2f ( rect.Right, rect.Top );
			glVertex2f ( width, 0 ) ;
		}
		glEnd();
	}
}


void CAnimation::SetStartAt ( int startAt_ )
{
	startAt = startAt_;
}


