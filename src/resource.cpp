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

#include "resource.hpp"
#include "game.hpp"


sf::Image* CImageResource::Load ( const std::string& str )
{
	sf::Image* image = new sf::Image();

	if ( !image->LoadFromFile ( settings::GetPath() + str ) ) {
		GetGameClass()->Error ( "ImageManager failed to load: " + str, __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		delete image;
		image = NULL;
	}

	return image;
}


sf::Font* CFontResource::Load ( const std::string& str )
{
	sf::Font* font = new sf::Font();

	if ( !font->LoadFromFile ( settings::GetPath() + str ) ) {
		GetGameClass()->Error ( "FontManager failed to load: " + str, __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		delete font;
		font = NULL;
	}

	return font;
}


sf::SoundBuffer* CSoundResource::Load ( const std::string& str )
{
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	
	if ( !buffer->LoadFromFile ( str ) ) {
		GetGameClass()->Error ( "SoundBufferManager failed to load: " + str, __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		delete buffer;
		buffer = NULL;
	}

	return buffer;
}

