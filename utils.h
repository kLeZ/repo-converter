/*
    This class encloses the utility functions, procedures and algorithms for the entire project.
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


#ifndef FREAX_LIBZYPP_UTILS_H
#define FREAX_LIBZYPP_UTILS_H

#include <string>
#include <fstream>
#include <sstream>
#include <Poco/File.h>
#include <curl/curl.h>
#include <stdlib.h>

#include "repository.h"


namespace freax
{
	namespace libzypp
	{
		using namespace std;

		class Utils
		{
			public:
				bool debug;
				string convertBool(bool b);
				vector<Repository> *getRepositories(File *repo);
				bool isValid(URI uri);
				void curlInit();
				string itos(int i);
		};
	}
}


#endif // FREAX_LIBZYPP_UTILS_H
