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

#ifndef ACTION_HPP
#define ACTION_HPP

#include <sstream>
#include "gui/widget.hpp"
#include "util.hpp"

/*
	widget_ - Pointer auf Widgets das die Aktion ausführt
	stream_ - Die eigendlichen Paramter getrennt durch Leerzeichen
*/

namespace action
{


/*
	void
*/
bool quit ( gui::CWidget* widget, util::DataHolder& args );

/*
	1.) <optional> Typ: string | Funktion: "id" oder "name" ( "" schließt das Mutterfenster des Widgets )
	2.) <optional> Typ: int | Funktion: enthällt entweder die Id des Fensters oder den Namen
*/
bool closeWindow ( gui::CWidget* widget, util::DataHolder& args );


namespace gfx
{

/*
		Funktion: Einfärben eines sf::Sprites
		---------
	sprite_one -> Objekt das eingefärbt werden soll
	f -> Zeit in Millisekunden bis Ende (optional)
	x -> verbleibende Frames bis Ende (optional)
	b -> 1 = addieren und 0 = subtrahieren
	color_one -> Endfarbe
	color_two -> Differenzfarbe (+)
	
*/
bool inking ( gui::CWidget* widget, util::DataHolder& args );


} // namespace action::gfx



} // namespace action

#endif // ACTION_HPP
