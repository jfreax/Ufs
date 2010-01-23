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

#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "../../engine/ui/widget.hpp"
#include "textarea.hpp"

namespace gui
{
	
class CTerminal : public CTextArea
{
	public:
		CTerminal();
		void Initialize();
		bool Update ( bool doIt );
		
		void PressedKey ( sf::Key::Code code );
		
		void Run();
		
		void SetCursor ( int x );
		
	private:
		void OverwriteLastCommandWith (  std::wstring str );
		std::vector< std::string > FindFunction ( std::string str );
		void PressedTab();
		
	private:
		bool initialized_;
		
		std::vector< std::string > commands_;
		std::vector< std::string > history_;
		int historyPos_;
};


} /* namespace gui */


#endif // TERMINAL_HPP
