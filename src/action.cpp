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

#include "gui/widget.hpp"
#include "game.hpp"
#include "action.hpp"

namespace action
{


bool quit ( gui::CWidget* widget, util::DataHolder& args )
{
	GetGameClass()->Stop();
}


bool closeWindow ( gui::CWidget* widget, util::DataHolder& args )
{
	if ( args.string_one.empty() )
	{
		GetGameClass()->GetGuiManager()->CloseWindow ( widget->GetMotherWin() );
	}
}


namespace gfx
{


bool inking ( gui::CWidget* widget, util::DataHolder& args )
{
	++args.frame;

	if ( !args.sprite_one )
	{
		return false;
	}

	if ( args.f )
	{ // Nach abgelaufender Zeit löschen
		if ( args.f <= 0.f )
		{
			return false;
		}

		args.f -= GetGameClass()->GetApp()->GetFrameTime();
	}
	if ( args.x )
	{ // Nach x-Frames löschen
		if ( args.x > args.frame )
		{
			return false;
		}
	}

	bool end = true;

	sf::Color spriteColor = args.sprite_one->GetColor();
	sf::Color endColor = spriteColor;
	if ( args.b )
	{
		endColor = args.sprite_one->GetColor() + args.color_two;

		if ( args.color_one.r > spriteColor.r )
		{
			endColor.r = spriteColor.r + args.color_two.r;
			end = false;
		}
		if ( args.color_one.g > spriteColor.g )
		{
			endColor.g = spriteColor.g + args.color_two.g;
			end = false;
		}
		if ( args.color_one.b > spriteColor.b )
		{

			endColor.b = spriteColor.b + args.color_two.b;
			end = false;
		}
		if ( args.color_one.a > spriteColor.a )
		{
			endColor.a = spriteColor.a + args.color_two.a;
			end = false;
		}
	}
	else
	{
		if ( args.color_one.r < spriteColor.r )
		{
			endColor.r = spriteColor.r - args.color_two.r;
			end = false;
		}
		if ( args.color_one.g < spriteColor.g )
		{
			endColor.g = spriteColor.g - args.color_two.g;
			end = false;
		}
		if ( args.color_one.b < spriteColor.b )
		{
			endColor.b = spriteColor.b - args.color_two.b;
			end = false;
		}
		if ( args.color_one.a < spriteColor.a )
		{
			endColor.a = spriteColor.a - args.color_two.a;
			end = false;
		}
	}

	// Neue Farbe speichern
	args.sprite_one->SetColor ( endColor );

	return !end;
}



} // namespace action::gfx


} // namespace action
