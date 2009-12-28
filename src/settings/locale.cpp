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

#line 17 "settings/locale.cpp"

#include "../util/util.hpp"
#include "../game.hpp"
#include "locale.hpp"
#include "settings.hpp"



std::string l ( HEADERNAME header, std::string var )
{
	static Parser* file = GetGameClass()->GetLocale()->GetFile();

	switch ( header ) {
		case SYS:
			return file->GetValue ( "SYSTEM", var );
			break;
		case TOOLTIP:
			return file->GetValue ( "TOOLTIP", var );
			break;
	}

	return "";
}


CLocale::CLocale ( std::string lang )
{
	file.Open ( "locale/" + lang + ".ini" );
}
