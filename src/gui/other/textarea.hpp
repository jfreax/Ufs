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
		
		virtual void Initialize();

		void Render();
		bool Update ( bool doIt = false );
		
		bool onLeftClick();
		void PressedKey ( sf::Key::Code code );
		
		bool AddText ( std::wstring str );

		virtual void SetCursor ( int x );
		void MoveCursor ( int x );
		
	private:
		void CalcBackground();
		
	protected:
		int cursorPos_;
		
	private:
		bool initialized_;

		sf::Shape cursor_;
		sf::Clock cursorTime_;
		
		sf::Shape* background_;

};


} /* namespace gui */


#endif // TEXTAREA_HPP
