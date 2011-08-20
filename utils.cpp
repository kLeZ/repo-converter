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


#include "utils.h"

using namespace freax::libzypp;

string Utils::convertBool(bool b)
{
	return (b ? "1" : "0");
}

AnyOption *Utils::initOptions(int argc, char **argv)
{
	AnyOption *options = new AnyOption;
	Utils::initUsage(options);
	options->setCommandFlag("help", 'h');
	options->setCommandOption("file-repo", 'f');
	options->setCommandOption('c');
	options->setCommandOption('n');
	options->processCommandArgs(argc, argv);
	return options;
}

void Utils::initUsage(AnyOption *options)
{
	options->addUsage("");
	options->addUsage("Usage: ");
	options->addUsage("");
	options->addUsage(" -h			--help					Prints this help");
	options->addUsage(" -f [file]	--file-repo [file] 		Path and file name of the exported .repo file");
	options->addUsage("										To export a .repo file with zypper execute the");
	options->addUsage("										following command: zypper lr -e [file.repo]");
	options->addUsage(" -c [version]						The current version of your openSuSE distribution (11.3)");
	options->addUsage(" -n [version]						The new version of your openSuSE distribution (11.4)");
	options->addUsage("");
}

vector< Repository > Utils::getRepositories(File *repo)
{
	vector<Repository> *ret = new vector<Repository>;

	ifstream scanner(repo->path().c_str(), _S_out);
	string line;
	Repository *current = NULL;

	while(getline(scanner, line))
	{
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

	return *ret;
}

bool Utils::isValid(URI *uri)
{
	bool ret = false;
	CURL *curl = curl_easy_init();
	return ret;
}