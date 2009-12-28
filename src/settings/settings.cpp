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

#include "config.hpp"

#include "../game.hpp"
#include "../util/util.hpp"
#include "settings.hpp"

namespace
{
Config config;

} // namespace


namespace settings
{

sf::VideoMode GetVideo()
{
	return config.video;
}


sf::WindowSettings GetWindowSettings()
{
	return config.windowSettings;
}


int GetStyle()
{
	int style = sf::Style::Close;

	if ( GetFullscreen() )
		style |= sf::Style::Fullscreen;

	return style;
}


void SetLang ( std::string lang )
{
	config.lang = lang;
}


std::string GetLang()
{
	return config.lang;
}



std::string GetPath()
{
	return config.path;
}


std::string GetThemePath()
{
	return "/themes/" + config.theme + "/";
}


void SetPath ( std::string path )
{
	config.path = path;
}


bool GetFullscreen()
{
	return config.fullscreen;
}


void SetFullscreen ( bool ison )
{
	config.fullscreen = ison;
	GetGameClass()->Initialize();
}


void ToggleFullscreen()
{
	config.fullscreen = !config.fullscreen;
	GetGameClass()->Initialize();
}


bool GetShowFps()
{
	return config.fps;
}


void SetShowFps ( bool ison )
{
	config.fps = ison;
}


int GetHeight()
{
	return config.video.Height;
}


void SetHeight ( int height )
{
	config.video.Height = height;
}


int GetWidth()
{
	return config.video.Width;
}


void SetWidth ( int width )
{
	config.video.Width = width;
	GetGameClass()->GetFpsStr()->SetPosition ( settings::GetWidth() - 70, 10 );
}


int GetBpp()
{
	return config.video.BitsPerPixel;
}


void SetBpp ( int bpp_ )
{
	config.video.BitsPerPixel = bpp_;
}


std::string GetTheme()
{
	return config.theme;
}


void SetTheme ( std::string theme )
{
	if ( util::isFile ( "themes/" + theme + ".ini" ) )
		config.theme = theme;
	else
		GetGameClass()->Error ( "Theme-File not found: " + theme, __PRETTY_FUNCTION__, __FILE__, __LINE__ );
}


MOUSESCOPE GetMouseScope()
{
	return config.mousescope;
}


void SetMouseScope ( MOUSESCOPE mousescope )
{
	if ( config.mousescope != mousescope ) {
		GetGameClass()->GetCursor() [mousescope]->Start();
		config.mousescope = mousescope;
	}
}



} // namespace settings
