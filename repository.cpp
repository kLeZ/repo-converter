/*
  Repository class that represents a repository section in the .repo file
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

#include "repository.h"

using namespace freax::libzypp;

Repository::Repository(string title)
{
	setTitle(title);
}

string Repository::getTitle()
{
	return this->title;
}

URI Repository::getBaseurl()
{
	return this->baseurl;
}

string Repository::getName()
{
	return this->name;
}

File Repository::getPath()
{
	return this->path;
}

string Repository::getType()
{
	return this->type;
}

bool Repository::isAutorefresh()
{
	return this->autorefresh;
}

bool Repository::isEnabled()
{
	return this->enabled;
}

bool Repository::isGpgcheck()
{
	return this->gpgcheck;
}

bool Repository::isKeeppackages()
{
	return this->keeppackages;
}

void Repository::setAutorefresh(bool autorefresh)
{
	this->autorefresh = autorefresh;
}

void Repository::setBaseurl(URI baseurl)
{
	this->baseurl = baseurl;
}

void Repository::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

void Repository::setGpgcheck(bool gpgcheck)
{
	this->gpgcheck = gpgcheck;
}

void Repository::setKeeppackages(bool keeppackages)
{
	this->keeppackages = keeppackages;
}

void Repository::setName(string name)
{
	this->name = name;
}

void Repository::setPath(File path)
{
	this->path = path;
}

void Repository::setTitle(string title)
{
	this->title = title;
}

void Repository::setType(string type)
{
	this->type = type;
}

string Repository::toString()
{
	Utils utils;
	string builder;
	builder.append("[");
	builder.append(this->title);
	builder.append("]");
	builder.append("\n");
	builder.append("name=");
	builder.append(this->name);
	builder.append("\n");
	builder.append("enabled=");
	builder.append(utils.convertBool(this->enabled));
	builder.append("\n");
	builder.append("autorefresh=");
	builder.append(utils.convertBool(this->autorefresh));
	builder.append("\n");
	builder.append("baseurl=");
	builder.append(this->baseurl.toString());
	builder.append("\n");
	builder.append("path=");
	builder.append(this->path.path());
	builder.append("\n");
	builder.append("type=");
	builder.append(this->type);
	builder.append("\n");
	builder.append("keeppackages=");
	builder.append(utils.convertBool(this->keeppackages));
	builder.append("\n");
	builder.append("gpgcheck=");
	builder.append(utils.convertBool(this->gpgcheck));
	builder.append("\n");
	return builder;
}

vector<Repository> *Repository::getRepositories(File *repo)
{
	vector<Repository> *ret = new vector<Repository>();
	Utils utils;
	ifstream scanner(repo->path().c_str());
	
	if(utils.verbosityLevel >= enums::debug)
		std::cout << "Scanner status: " << scanner.is_open() << std::endl;
	
	string line;
	Repository *current = NULL;
	
	while(getline(scanner, line))
	{
		if(utils.verbosityLevel >= enums::debug)
			std::cout << line << endl;
		
		if(line[0] == '[')
		{
			line = line.substr(1, line.size() - 2);
			
			if(utils.verbosityLevel >= enums::debug) std::cout << "Trimmed title: " << line << endl;
			
			if(current)
			{
				ret->push_back(*current);
				
				if(utils.verbosityLevel >= enums::debug)
					std::cout << "Elements are now " << ret->size() << "\n" << "Just pushed:\n" << current->toString() << endl;
			}
			
			current = new Repository(line);
		}
		else if(!line.empty())
		{
			size_t eqIndex = line.find_first_of('=');
			string key, value;
			key = line.substr(0, eqIndex);
			value = line.substr(eqIndex + 1);
			
			if(utils.verbosityLevel >= enums::debug)
				std::cout << "Key is " << key << " and value is " << value << endl;
			
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