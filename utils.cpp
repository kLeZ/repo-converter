/*
  This class encloses the utility functions, procedures and algorithms for the entire project.
  Copyright (C) 2011 Alessandro Accardo <julius8774@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include "utils.h"

using namespace freax::libzypp;

string Utils::convertBool(bool b)
{
	return (b ? "1" : "0");
}

vector<Repository> *Utils::getRepositories(File *repo)
{
	vector<Repository> *ret = new vector<Repository>();

	ifstream scanner(repo->path().c_str(), ios::out);
	string line;
	Repository *current = NULL;

	while(getline(scanner, line))
	{
		if (debug) cout << line << endl;

		if(line[0] == '[')
		{
			line = line.substr(1, line.size() - 2);

			if(current)
			{
				ret->push_back(*current);
			}

			current = new Repository(line);
		}
		else if(!line.empty())
		{
			size_t eqIndex = line.find_first_of('=');
			string key, value;
			key = line.substr(0, eqIndex);
			value = line.substr(eqIndex + 1);

			if(key == "name")
			{
				current->setName(value);
			}
			else if(key == "enabled")
			{
				current->setEnabled(atoi(value.c_str()));
			}
			else if(key == "autorefresh")
			{
				current->setAutorefresh(atoi(value.c_str()));
			}
			else if(key == "baseurl")
			{
				current->setBaseurl(*(new URI(value)));
			}
			else if(key == "path")
			{
				current->setPath(*(new File(value)));
			}
			else if(key == "type")
			{
				current->setType(value);
			}
			else if(key == "keeppackages")
			{
				current->setKeeppackages(atoi(value.c_str()));
			}
			else if(key == "gpgcheck")
			{
				current->setGpgcheck(atoi(value.c_str()));
			}
		}
	}

	return ret;
}

void Utils::curlInit()
{
	curl_global_init(CURL_GLOBAL_ALL);
}


bool Utils::isValid(URI uri)
{
	bool ret = false;
	int retcode = -1;
	CURL *curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_HEADER, 1) ;
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1) ;
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1) ;
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10) ;

	// suppress all output by sending it to the null device
	FILE *null_device = std::fopen("/dev/null", "w") ;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, null_device) ;


	curl_easy_setopt(curl, CURLOPT_URL, uri.toString().c_str());
	retcode = curl_easy_perform(curl);
	ret = retcode == 0;
	curl_easy_cleanup(curl);
	return ret;
}

string Utils::itos(int i)
{
	ostringstream ss;
	ss << i;
	string ret = ss.str();
	return ret;
}
