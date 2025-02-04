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

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

class Parser
{
	public:
		std::stringstream operator[] ( std::string headerName );

		bool Open ( std::string filename = "", bool index = false, bool make_lower = false );
		bool Save ( std::string file );

		int CountKeys ( std::string searchSection, std::string searchedKey );
		int CountKeys ( std::string searchSection );
		int CountKeys ( unsigned int sectionNr );

		std::string GetValue ( std::string section, std::string key );
		std::string GetValue ( unsigned int sectionNr, std::string key );
		std::string GetValue ( unsigned int sectionNr, unsigned int keyNr );
		std::string GetValue ( std::string section, unsigned int keyNr );

		std::string GetKey ( unsigned int sectionNr, unsigned int keyNr );
		std::string GetKey ( std::string section, unsigned int keyNr );

	private:
		std::vector <std::string> headers;

		typedef std::map < std::string, std::string > Section;
		std::map < std::string, Section > data;

};

#endif // PARSER_HPP
