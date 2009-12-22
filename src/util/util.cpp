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
#include "../settings/settings.hpp"

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


MOUSE::TYPES sfMouseToMouse ( sf::Mouse::Button button )
{
	if ( button == sf::Mouse::Left )
	{
		return MOUSE::LEFT;
	}
	else if ( button == sf::Mouse::Right )
	{
		return MOUSE::RIGHT;
	}
	else if ( button == sf::Mouse::Middle )
	{
		return MOUSE::MIDDLE;
	}
}


int random ( int a )
{
	return -a + ( rand() % ( 2*a + 1 ) );
}


int random ( int a, int b )
{
	if ( ! ( b - a + 1 ) )
		return 0;

	return a + ( rand() % ( b - a + 1 ) );
}


double random ( double a, double b )
{
	int aint = a * 1000;
	int bint = b * 1000;

	if ( ! ( b - a + 1.f ) )
	{
		return 0.f;
	}

	return ( double ) ( ( aint + ( rand() % ( bint - aint + 1 ) ) ) / 1000.f );
}


} // namespace util
