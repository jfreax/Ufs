/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef TEXT_HPP
#define TEXT_HPP

#include "../../engine/ui/widget.hpp"

namespace gui
{


class CText : public CWidget
{
	public:
		CText();
		
		void Render();
		bool Update ( bool doIt );
		
		bool AddText ( std::wstring str, int position = -1 );
		
		std::wstring GetText();
		sf::String* GetRenderedText();
		
		bool DelText ( int from, int to );
		
		void SetScollbar ( int scrollbar );
		
	protected:
		sf::Font* font_;
		sf::String renderedText_;
		sf::String template_;
		
		int scrollbar_;
};



} /* namespace gui */


#endif // TEXT_HPP
