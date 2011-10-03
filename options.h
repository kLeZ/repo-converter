/*
    This class contains all the options for the repo-converter program
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


#ifndef OPTIONS_H
#define OPTIONS_H

#include <boost/program_options.hpp>
#include <ostream>


using namespace std;
namespace po = boost::program_options;

namespace freax
{
	namespace libzypp
	{
		enum verbosity
		{
			quiet = 0,
			verbose = 1,
			very_verbose = 2,
			debug = 3
		};

		enum option
		{
			help = 0,
			version = 1,
			stdout = 2,
			not_validate_urls = 3,
			backup = 4,
			file_repo = 5,
			current = 6,
			next = 7
		};

		class options
		{
			private:
				bool debug;
				bool verbose;
				bool curl_verbose;
				po::options_description all;
				po::variables_map vm;

			public:
				void initOptions();
				void parseCommandLine(int argc, char **argv);
				void notify();

				template <class T> T getOption(option op);

				friend ostream& operator<<(ostream& output, const options& p);

				bool empty();
				verbosity verbosityLevel();
		};
	}
}

#endif // OPTIONS_H
