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
#include "utils.h"

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
