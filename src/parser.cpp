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
#include "settings/settings.hpp"
#include "util/util.hpp"
#include "parser.hpp"
#include "game.hpp"

bool Parser::Open ( std::string filename, bool index, bool make_lower )
{
	if ( filename.empty() )
	{
		return false;
	}

	static int nr = 0;

	/* Datei öffnen */
	filename = settings::GetPath() + filename;
	
	std::ifstream file ( filename.c_str(), std::ios::in );

	/* Auf erfolg prüfen */
	if ( !file ) {
		GetGameClass()->Error ( "Could not open file: " + filename,  __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		return false;
	}

	/* Temporale Variablen deklarieren */
	std::string line = "";
	std::string sectionName = "";
	std::string normalLine = line;
	
	unsigned int pos = 0;

	/* Datei Zeile für Zeile durchgehen */
	while ( std::getline ( file, normalLine ) )
	{
		++nr;
		line = normalLine;
		


		pos = line.find_first_not_of ( " \t" );

		if ( pos == std::string::npos )
		{
			line.erase();
		}
		else
		{
			line.erase ( 0, pos );
		}

		if ( line.empty() || line.at ( 0 ) == '#' ||  line.at ( 0 ) == ';' )
		{
		}
		else
			if ( line.at ( 0 ) == '[' )
			{
				// neue Sektion gefunden
				pos = line.find ( ']', 1 );

				if ( pos == std::string::npos )
				{
					std::cerr << "Could not find ']'" << std::endl;
				}

				sectionName = line.substr ( 1, pos - 1 );

				if ( sectionName.length() == 0 )
				{
					std::cerr << std::string ( "Empty Section Name" ) << std::endl;
				}
				else
				{
					if ( index )
						sectionName += "-" + util::lCast< std::string > ( nr );

					headers.push_back ( sectionName );
				}

				if ( make_lower )
				{
					util::lowercase ( sectionName );
				}

				// neue Sektion hinzufügen
				data.insert ( std::make_pair ( sectionName, Section() ) );

			}
			else
			{
				// neues key-value-Paar gefunden
				pos = line.find ( '=', 1 );

				if ( pos == std::string::npos )
				{
					std::cerr << std::string ( "Could not find '='" ) << std::endl;
				}

				std::string key = line.substr ( 0, pos );
				std::string value = line.substr ( pos + 1, line.length() - pos - 1 );

				// unnötige leerzeichen am beginn und ende entfernen
				key = key.substr ( key.find_first_not_of ( ' ', 0 ), key.length() );
				key = key.substr ( 0, key.find_first_of ( ' ', 0 ) );
				value = value.substr ( value.find_first_not_of ( ' ', 0 ), value.length() );

				if ( key.length() == 0 || value.length() == 0 )
				{
					std::cerr << std::string ( "Empty key or empty value" ) << std::endl;
				}

				// neues key-value-Paar hinzufügen
				if ( sectionName == "" )
				{
					std::cerr << std::string ( "Key-Value-Pair without Section" ) << std::endl;
				}

				// mehrere Einträge (keys) mit dem gleichen Namen indexieren ( "-i" )
				int countedKeys = CountKeys ( sectionName, key );

				if ( make_lower )
				{
					util::lowercase ( key );
					util::lowercase ( value );
				}

				if ( countedKeys )
				{
					data[sectionName].insert ( std::make_pair ( key + "-" + util::lCast< std::string > ( countedKeys ), value ) );
				}
				else
				{
					data[sectionName].insert ( std::make_pair ( key, value ) );
				}

			}
	}
	
	return true;
}


int Parser::CountKeys ( std::string searchSection, std::string searchedKey )
{
	int i = 1;
	for ( ; data[searchSection].count ( searchedKey + "-" + util::lCast< std::string > ( i ) ); ++i );

	return data[searchSection].count ( searchedKey ) + i - 1;
}


int Parser::CountKeys ( std::string searchSection )
{
	return data[searchSection].size();
}


int Parser::CountKeys ( unsigned int sectionNr )
{
	return data[headers[sectionNr]].size();
}


std::string Parser::GetValue ( std::string section, std::string key )
{
	if ( data.count ( section ) )
	{
		if ( data[section].count ( key ) )
		{
			return data[section][key];
		}
	}

	/* Sektion oder Key existiert nicht */
	return "";
}


std::string Parser::GetValue ( unsigned int sectionNr, std::string key )
{
	if ( headers.size() <= sectionNr )
		return "";

	return Parser::GetValue ( headers[sectionNr], key );
}


std::string Parser::GetValue ( unsigned int sectionNr, unsigned int keyNr )
{
	if ( headers.size() <= sectionNr )
	{
		return "";
	}

	if ( headers[sectionNr].size() <= keyNr )
	{
		return "";
	}

	std::map < std::string, std::string >::iterator it =  data[headers[sectionNr]].begin();


	for ( int i = 0 ; it != data[headers[sectionNr]].end() && i < keyNr; ++it, ++i )
	{
	}

	return it->second;
}


std::string Parser::GetValue ( std::string section, unsigned int keyNr )
{
	if ( !data.count ( section ) )
		return "";

	if ( section.size() <= keyNr )
		return "";

	std::map < std::string, std::string >::iterator it =  data[section].begin();

	int i = 0;

	for ( ; it != data[section].end() && i < keyNr; ++it, ++i );

	return it->second;
}


std::string Parser::GetKey ( unsigned int sectionNr, unsigned int keyNr )
{
	if ( headers.size() <= sectionNr )
		return "";

	if ( headers[sectionNr].size() <= keyNr )
		return "";

	std::map < std::string, std::string >::iterator it =  data[headers[sectionNr]].begin();

	int i = 0;

	for ( ; it != data[headers[sectionNr]].end() && i < keyNr; ++it, ++i );

	return it->first;
}


std::string Parser::GetKey ( std::string section, unsigned int keyNr )
{
	if ( !data.count ( section ) )
		return "";

	if ( section.size() <= keyNr )
		return "";

	std::map < std::string, std::string >::iterator it =  data[section].begin();

	int i = 0;

	for ( ; it != data[section].end() && i < keyNr; ++it, ++i );

	return it->first;
}

