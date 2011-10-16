/*
    <one line to give the program's name and a brief idea of what it does.>
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


#include "options.h"


namespace freax
{
	namespace libzypp
	{
		ostream& operator<< (ostream &out, options &opts)
		{
			out << opts.all;
			return out;
		}
	}
}


using namespace freax::libzypp;
namespace po = boost::program_options;


void options::initOptions()
{
	po::options_description generic("Generic options");
	generic.add_options()
	("help,h", "Prints this help and exits")
	("version,V", "Prints the software version information and exits");

	po::options_description switches("Switch options");
	switches.add_options()
	("not-validate-urls,u", "Don't validate the urls in the repository file, nor the converted ones")
	("debug,d", "Debug messages are printed in standard output")
	("verbose,v", "Verbose output will be displayed, this does not affect eventual stdout option")
	("curl-verbose,C", "Verbose output for the curl engine")
	("stdout,s", "Prints the resulting .repo file to std out instead of a file")
	("backup,b", "Makes a backup of the input file, instead of overwriting it");

	po::options_description values("Value options");
	values.add_options()
	("file-repo,f", po::value<string>()->required(), "Path and file name of the exported .repo file\n"
	"To export a .repo file with zypper execute the following command: zypper lr -e [file.repo]")
	("current,c", po::value<string>()->required(), "The current version of your openSuSE distribution")
	("next,n", po::value<string>()->required(), "The new version of your openSuSE distribution");

	all.add(generic).add(switches).add(values);
}

bool options::empty()
{
	return vm.empty();
}

bool options::getFlagOption(enums::option op)
{
	bool ret;
	switch (op)
	{
		case (enums::help):
			ret = vm.count("help");
			break;
		case (enums::version):
			ret = vm.count("version");
			break;
		case (enums::stdout):
			ret = vm.count("stdout");
			break;
		case (enums::not_validate_urls):
			ret = vm.count("not-validate-urls");
			break;
		case (enums::backup):
			ret = vm.count("backup");
			break;
	}
	return ret;
}

string options::getValueOption(enums::option op)
{
	string ret;
	switch (op)
	{
		case (enums::file_repo):
			if (vm.count("file-repo"))
				ret = vm["file-repo"].as<string>();
			break;
		case (enums::current):
			if (vm.count("current"))
				ret = vm["current"].as<string>();
			break;
		case (enums::next):
			if (vm.count("next"))
				ret = vm["next"].as<string>();
			break;
	}
	return ret;
}

void options::notify()
{
	po::notify(vm);
}

void options::parseCommandLine(int argc, char **argv)
{
	po::store(po::parse_command_line(argc, argv, all), vm);

	if (!vm.empty())
	{
		debug = vm.count("debug");
		verbose = vm.count("verbose");
		curl_verbose = vm.count("curl-verbose");
	}
}

enums::verbosity options::verbosityLevel()
{
	enums::verbosity ret = enums::quiet;
	if (debug) ret = enums::debug;
	else if (curl_verbose) ret = enums::very_verbose;
	else if (verbose) ret = enums::verbose;
	return ret;
}
