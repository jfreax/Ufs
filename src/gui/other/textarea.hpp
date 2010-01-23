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

#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP


#include "../../engine/ui/widget.hpp"
#include "text.hpp"

namespace gui
{
	

class CTextArea : public CText
{
	public:
		CTextArea();
		
		void Initialize();

		void Render();
		bool Update ( bool doIt = false );
		
		bool onLeftClick();
		void PressedKey ( sf::Key::Code code );
		
		bool AddText ( std::wstring str );

		
		void SetCursor ( int x );
		void MoveCursor ( int x );
		
	private:
		bool initialized_;
	
		int cursorPos_;
		sf::Shape cursor_;

};


} /* namespace gui */


#endif // TEXTAREA_HPP
