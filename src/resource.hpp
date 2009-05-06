/* MIT License:

Copyright (c) 2008 Adam W. Brown (www.darkrockstudios.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE. */

#ifndef RESOURCE_H
#define RESOURCE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <map>

#include "settings.hpp"

template< class T >
class CResource
{
	public:
		typedef std::pair< std::string, T* > Resource;
		typedef std::map< std::string, T* > ResourceMap;

		CResource()
		{
		}

		~CResource()
		{
			ReleaseAllResources();
		}

		T* Get ( const std::string& strId )
		{
			T* resource = Find ( strId );
			if ( resource == NULL )
			{
				resource = Load ( strId );
				// If the resource loaded successfully, add it do the resource map
				if ( resource != NULL )
					m_resource.insert ( Resource ( strId, resource ) );
			}
			return resource;
		}

		void Release ( const std::string& strId )
		{
			T* resource = Find ( strId );
			if ( resource != NULL )
			{
				delete resource;
				m_resource.erase ( m_resource.find ( strId ) );
			}
		}

		void ReleaseAllResources()
		{
			while ( m_resource.begin() != m_resource.end() )
			{
				delete m_resource.begin()->second;
				m_resource.erase ( m_resource.begin() );
			}
		}

	private:
		ResourceMap m_resource;

		T* Find ( const std::string& strId )
		{
			T* resource = NULL;

			typename ResourceMap::iterator it = m_resource.find ( strId );
			if ( it != m_resource.end() )
			{
				resource = it->second;
			}

			return resource;
		}

	protected:
		virtual T* Load ( const std::string& str ) = 0;
};


class CImageResource : public CResource< sf::Image >
{
	public:

	protected:
		virtual sf::Image* Load ( const std::string& str );
};


class CFontResource : public CResource < sf::Font >
{
	public:

	protected:
		virtual sf::Font* Load ( const std::string& str );
};


class CSoundResource : public CResource < sf::SoundBuffer >
{
	public:

	protected:
		virtual sf::SoundBuffer* Load ( const std::string& str );

};




#endif // RESOURCE_H


// namespace sf
// {
// class Image;
// }
// /*
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License version 2 as published by the Free Software Foundation.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public License
//    along with this library; see the file COPYING.LIB.  If not, write to
//    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
//    Boston, MA 02110-1301, USA.
// */
//
// #ifndef RESOURCE_HPP
// #define RESOURCE_HPP
//
// #include <iostream>
// #include <typeinfo>
// #include <map>
//
// #include <SFML/Graphics.hpp>
//
// #include "util.hpp"
// #include "settings.hpp"
//
// template < typename T >
// class CResource
// {
//   public:
//     T* Get ( std::string name_ ) {
//       if ( !data [ name_ ] ) {
//         return Push ( name_ );
//       } else {
//         return data [ name_ ];
//       }
//     }
//
//
//     T* Push ( std::string filename_ ) {
//       if ( typeid ( T ).name() == typeid ( sf::Image ).name() ) {
//
//         sf::Image* img = new sf::Image;
//
//         if ( !img->LoadFromFile ( settings::GetPath() + filename_ ) ) {
//           // TODO Logfile
//           std::cerr << "Error loading backgroundimage: " << filename_ << std::endl;
//         }
//
//         data [ filename_ ] = (T*)img;
//         return (T*)img;
//       }
//     }
//
//   private:
//     std::map< std::string, T* > data;
// };
//
// #endif // RESOURCE_HPP
