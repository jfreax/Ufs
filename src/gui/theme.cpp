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

#include <typeinfo>

#include "theme.hpp"
#include "../settings.hpp"

namespace gui
{


CTheme::CTheme ( void )
{
	ThemeHolder defaultforall; // Standardeigenschaften für _alle_ GUI Objekte

	defaultforall.minSize = sf::Vector2f ( 10.f, 10.f );
	defaultforall.maxSize = sf::Vector2f ( 5000.f, 5000.f );
	defaultforall.backgroundColor = window_.borderColor = sf::Color ( 100, 100, 100 );
	defaultforall.titlebarColor = sf::Color ( 0, 0, 100 );
	defaultforall.border = 1;
	defaultforall.titlebar = 10;
	defaultforall.fontSize = 12;
	defaultforall.iconClose = sf::Vector3f ( -20, 0, 5 );

	window_ = button_ = defaultforall;


	headerList_.insert ( std::make_pair< std::string, ThemeHolder* > ( "window", &window_ ) );
	headerList_.insert ( std::make_pair< std::string, ThemeHolder* > ( "button", &button_ ) );

	std::map< std::string, ThemeHolder* >::iterator iter = headerList_.begin();
	std::map< std::string, ThemeHolder* >::iterator iterEnd = headerList_.end();

	for ( ; iter != iterEnd; ++iter )
	{
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "minsize", &iter->second->ThemeHolder::minSize ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "maxsize", &iter->second->ThemeHolder::maxSize ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "backgroundimage", &iter->second->ThemeHolder::background ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "backgroundcolor", &iter->second->ThemeHolder::backgroundColor ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "titlebarcolor", &iter->second->ThemeHolder::titlebarColor ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "border", &iter->second->ThemeHolder::border ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "titlebar", &iter->second->ThemeHolder::titlebar ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "fontsize", &iter->second->ThemeHolder::fontSize ) ) );
		paramList_.insert ( std::make_pair< std::string, Parameter > ( iter->first, Parameter ( "close", &iter->second->ThemeHolder::iconClose ) ) );
	}

}


void CTheme::Open ( std::string filename )
{
	themeFile_.Open ( filename, false, true );

	std::stringstream stream;
	std::string strTemp;

	std::map< std::string, ThemeHolder* >::iterator iter = headerList_.begin();
	std::map< std::string, ThemeHolder* >::iterator iterEnd = headerList_.end();
	std::map< std::string, Parameter >::iterator paramIter = paramList_.begin();
	std::map< std::string, Parameter >::iterator paramIterEnd = paramList_.end();

	for ( ; paramIter != paramIterEnd; paramIter++ )
	{
		stream.str ( "" );
		stream.clear();

		strTemp = themeFile_.GetValue ( paramIter->first, paramIter->second.name );

		if ( !strTemp.empty() )
		{
			switch ( paramIter->second.type )
			{

				case INTEGER:
					stream << strTemp && stream >> *paramIter->second.i;
					break;

				case FLOAT:
					stream << strTemp && stream >> *paramIter->second.f;
					break;

				case STRING:
					*paramIter->second.s = strTemp;
					break;

				case COLOR:
					stream << strTemp && stream >> paramIter->second.c->r >> paramIter->second.c->g >> paramIter->second.c->b >> paramIter->second.c->a;
					break;

				case IMAGE:

					if ( !paramIter->second.img->LoadFromFile ( settings::GetThemePath() + strTemp ) )
					{
						// TODO Resourcenmanager
						std::cerr << "Error loading backgroundimage: " << strTemp << std::endl;
					}

					break;

				case VECTOR_INTEGER:
					stream << strTemp && stream >> paramIter->second.vi->x >> paramIter->second.vi->y;
					break;

				case VECTOR_FLOAT:
					stream << strTemp && stream >> paramIter->second.vf->x >> paramIter->second.vf->y;
					break;

				case VECTOR3_FLOAT:
					stream << strTemp && stream >> paramIter->second.v3f->x >> paramIter->second.v3f->y >> paramIter->second.v3f->z;
					break;

				default:
					*paramIter->second.s = strTemp;
					break;
			}
		}
	}

//   }
}


} // namespace gui
