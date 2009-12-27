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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../util/util.hpp"

namespace settings
{

sf::VideoMode GetVideo();
sf::WindowSettings GetWindowSettings();
int GetStyle();

std::string GetLang();
void SetLang ( std::string lang );

std::string GetPath();
std::string GetThemePath();
void SetPath ( std::string path_ );

bool GetFullscreen();
void SetFullscreen ( bool ison );
void ToggleFullscreen();

bool GetShowFps();
void SetShowFps ( bool ison );

int GetHeight();
void SetHeight ( int height_ );

int GetWidth();
void SetWidth ( int width_ );

int GetBpp();
void SetBpp ( int bpp_ );

std::string GetTheme();
void SetTheme ( std::string theme_ );

MOUSESCOPE GetMouseScope();
void SetMouseScope ( MOUSESCOPE mousescope_ );

}



#endif // GAME_HPP
