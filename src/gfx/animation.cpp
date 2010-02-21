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

#include "../game.hpp"
#include "animation.hpp"



CAnimation::CAnimation ( sf::Image* image, int frames, double timePerFrame ) :
		image_ ( image ), frames_ ( frames ),
		timePerFrame_ ( timePerFrame ),
		frame_ ( 1 ),
		startAt_ ( 1 ),
		frameDiff_ ( 1 ),
		blur_ ( 1 )
{
	if ( frames_ )
	{
		perPixel_ = false;
		frameWidth_ = image_->GetWidth() / frames_;
	}
	else
	{
		frameWidth_ = image_->GetHeight();
		
		perPixel_ = true;
		frames_ = image_->GetWidth() - frameWidth_;
	}
	
	clipArea_ = sf::IntRect ( 0, 0, frameWidth_, image_->GetHeight() );
	this->Start();
}


void CAnimation::Render ( sf::RenderTarget& Target ) const
{
	if ( run_ )
	{
		float width  = static_cast<float> ( clipArea_.GetWidth() );
		float height = static_cast<float> ( clipArea_.GetHeight() );
		sf::FloatRect rect = image_->GetTexCoords ( clipArea_ );

		/* Bild für OpenGl "binden" */
		image_->Bind();

		/* Per OpenGL ein Quadrat zeichnen (mit dem Bild drauf) */
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


void CAnimation::Start ( void )
{
	frame_ = startAt_;
	run_ = true;
}


void CAnimation::Stop ( void )
{
	run_ = false;
}


void CAnimation::Update ( void )
{
	if ( timer_.GetElapsedTime() >= timePerFrame_ ) {
		timer_.Reset();

		frame_ += frameDiff_;
		if ( frame_ > frames_ )
			frame_ = startAt_;
		if ( frame_ < 0 )
			frame_ = frames_;

		/* Bild ist nicht in einzelne Frames eingeteilt,
		   Animation ist ein "Kameraschwenk" über das Bild */
		if ( perPixel_ )
			clipArea_ = sf::IntRect ( ( frame_ - 1 ), 0, frameWidth_ + frame_, image_->GetHeight() );
		/* Bild ist in Frames eingeteilt, die Positionsdatei herausbekommen */
		else
			clipArea_ = sf::IntRect ( frameWidth_ * ( frame_ - 1 ), 0, frameWidth_ * frame_, image_->GetHeight() );
	}
}


void CAnimation::SetFrameWidth ( int frameWidth )
{
	frameWidth_ = frameWidth;

	if ( perPixel_ )
		frames_ = image_->GetWidth() - frameWidth_;

	clipArea_ = sf::IntRect ( 0, 0, frameWidth_, image_->GetHeight() );
}


void CAnimation::SetStartAt ( int startAt_ )
{
	startAt_ = startAt_;
}


void CAnimation::SetFrameDiff ( int diff )
{
	frameDiff_ = diff;
}


void CAnimation::SetBlur ( int blur )
{
	blur_ = blur;
}


sf::Vector2f CAnimation::GetSize()
{
	return sf::Vector2f ( frameWidth_, image_->GetHeight() );
}

