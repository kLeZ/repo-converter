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

using namespace std;
using namespace freax::libzypp;


string Utils::convertBool(bool b)
{
	return (b ? "1" : "0");
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

	curl_easy_setopt(curl, CURLOPT_HEADER, 1);
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);

	// suppress all output by sending it to the null device
	if(verbosityLevel < enums::debug) curl_easy_setopt(curl, CURLOPT_WRITEDATA, fopen("/dev/null", "w"));


	curl_easy_setopt(curl, CURLOPT_URL, uri.toString().c_str());

	if(verbosityLevel >= enums::very_verbose)
		curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

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

URI Utils::changeVersionUrl(URI current, string curr_ver, string next_ver)
{
	string newurl = current.toString();
	newurl = changeVersionToken(newurl, curr_ver, next_ver);
	URI newuri = URI(newurl);
	return newuri;
}

string Utils::changeVersionToken(string current, string curr_ver, string next_ver)
{
	string newurl = current;

	if(verbosityLevel >= enums::debug) std::cout << "Replacing version in url '" << newurl << "'" << std::endl;

	int fndidx = newurl.find(curr_ver);

	if(verbosityLevel >= enums::debug) std::cout << "Version token found at " << fndidx << std::endl;

	if(fndidx > 0 && fndidx < newurl.size())
		newurl = newurl.replace(fndidx, curr_ver.size(), next_ver);

	if(verbosityLevel >= enums::debug) std::cout << "Replaced url will be '" << newurl << "'" << std::endl;

	return newurl;
}
