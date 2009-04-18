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

#include <fstream>
#include <iostream>
#include <iomanip>


#include "util.hpp"
#include "settings.hpp"

namespace util
{


DataHolder::DataHolder ( void ) :
		b ( true ),
		x ( 0 ), y ( 0 ), z ( 0 ), d ( 0 ), f ( 0 ),
		sprite_one ( NULL ), sprite_two ( NULL )
{
}


bool isFile ( std::string filename )
{
	filename = settings::GetPath() + filename;

	std::ifstream file ( filename.c_str(), std::ios::in );

	if ( !file )
		return false;
	else
		return true;
}


void deleteChar ( std::string &stringToReplace, char delThis )
{
	std::string::size_type pos = stringToReplace.find ( delThis, 0 );

	while ( std::string::npos != pos )
	{
		stringToReplace.replace ( pos, 1, "" );
		pos = stringToReplace.find ( delThis, pos + 1 );
	}
}

void lowercase ( std::string& str )
{
	for ( int i = 0; str[i] != '\0'; i++ )
	{
		str[i] = tolower ( str[i] );
	}
}


std::string colorToString ( sf::Color color )
{
	std::stringstream stream;
	static std::string string[4];

	stream << ( int ) color.r << " " << ( int ) color.g << " " << ( int ) color.b << " " << color.a ;
	stream >> string[0] >> string[1] >> string[2] >> string[3];

	return " " + string[0] + " " + string[1] + " " + string[2] + " " + string[3] + " ";
}



}
