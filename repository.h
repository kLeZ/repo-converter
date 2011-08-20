/*
    Header file of the Repository class that serves as a container for the .repo file data
    Copyright (C) 2011  Alessandro Accardo <julius8774@gmail.com>

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


#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <iostream>
#include <Poco/URI.h>
#include <Poco/File.h>

namespace freax
{
	namespace libzypp
	{
		using namespace std;
		using namespace Poco;

		class Repository
		{
			private:
				string title;
				string name;
				bool enabled;
				bool autorefresh;
				URI baseurl;
				File path;
				string type;
				bool keeppackages;
				bool gpgcheck;
			public:
				/* Constructor */
				Repository(string title);
				
				/* Getters */
				string getTitle();
				string getName();
				bool isEnabled();
				bool isAutorefresh();
				URI getBaseurl();
				File getPath();
				string getType();
				bool isKeeppackages();
				bool isGpgcheck();

				/* Setters */
				void setTitle(string title);
				void setName(string name);
				void setEnabled(bool enabled);
				void setAutorefresh(bool autorefresh);
				void setBaseurl(URI baseurl);
				void setPath(File path);
				void setType(string type);
				void setKeeppackages(bool keeppackages);
				void setGpgcheck(bool gpgcheck);

				/* Others */
				string toString();
		};
	}
}

#endif // REPOSITORY_H
