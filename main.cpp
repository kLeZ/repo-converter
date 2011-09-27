#include <iostream>
#include <Poco/File.h>
#include <Poco/URI.h>
#include <boost/program_options.hpp>

#include "repository.h"
#include "utils.h"
#include "version.h"

#define RET_OK						0
#define RET_FILE_NOT_FOUND			1
#define RET_BAD_REPOSITORY_LIST		2
#define RET_UNABLE_TO_WRITE			3
#define RET_BOOST_PARSING_ERROR		4

using namespace std;
using namespace Poco;
using namespace freax::libzypp;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
	int ret = RET_OK;
	Utils utils;

	po::options_description generic("Generic options");
	generic.add_options()
	("help,h", "Prints this help and exits")
	("version,v", "Prints the software version information and exits");

	po::options_description switches("Switch options");
	switches.add_options()
	("not-validate-urls,u", "Don't validate the urls in the repository file, nor the converted ones")
	("debug,d", "Debug messages are printed in standard output")
	("stdout,s", "Prints the resulting .repo file to std out instead of a file");

	po::options_description values("Value options");
	values.add_options()
	("file-repo,f", po::value<string>()->required(), "Path and file name of the exported .repo file\n"
	"To export a .repo file with zypper execute the following command: zypper lr -e [file.repo]")
	("current,c", po::value<string>()->required(), "The current version of your openSuSE distribution")
	("next,n", po::value<string>()->required(), "The new version of your openSuSE distribution");

	try
	{
		po::variables_map vm;
		po::options_description all("All options");
		all.add(generic).add(switches).add(values);
		po::store(po::parse_command_line(argc, argv, all), vm);

		if(vm.empty())
		{
			version.printVersion();
			cout << all << endl;
		}
		else
		{
			string path, prev_ver, foll_ver;
			bool validateurls, tofile, showhelp, showversion;

			showhelp = vm.count("help");
			showversion = vm.count("version");
			tofile = !vm.count("stdout");
			validateurls = vm.count("not-validate-urls");
			utils.debug = vm.count("debug");

			if(vm.count("file-repo")) path = vm["file-repo"].as<string>();
			if(vm.count("current")) prev_ver = vm["current"].as<string>();
			if(vm.count("next")) foll_ver = vm["next"].as<string>();


			if(showhelp)
			{
				cout << all << endl;
			}
			else if(showversion)
			{
				version.printVersion();
			}
			else
			{
				po::notify(vm);

				if(validateurls) utils.curlInit();

				cout << "Repository File: " << path << endl;
				cout << "Current version: " << prev_ver << endl;
				cout << "Next version: " << foll_ver << endl;
				cout << "Debug mode: " << (utils.debug ? "On" : "Off") << endl;
				cout << "Print to stdout: " << (tofile ? "No" : "Yes") << endl;
				cout << "Use CURL: " << (validateurls ? "Yes" : "No") << endl;


				File *repo = new File(path);

				if(repo->isFile() && repo->exists())
				{
					vector<Repository> *repositories = utils.getRepositories(repo);

					if(!repositories->empty())
					{
						string repocontent = "";

						for(int i = 0; i < repositories->size(); i++)
						{
							Repository repository = repositories->at(i);

							cout << "Checking repository '" << repository.getName();
							cout << "' with url '" << repository.getBaseurl().toString() << "'" << endl;

							if(utils.isValid(repository.getBaseurl()) || !validateurls)
							{
								cout << "Url seems to be valid, changing to next version repository and checking again" << endl;
								string newurl = repository.getBaseurl().toString();
								newurl = newurl.replace(newurl.find_first_of(prev_ver), prev_ver.size(), foll_ver);
								URI newuri = URI(newurl);

								cout << "Checking new version url '" << newuri.toString() << "'..." << endl;

								if(utils.isValid(newuri))
								{
									cout << "Next version url is valid, so can add the repository" << endl;
									repository.setBaseurl(newuri);
									repocontent = repocontent.append(repository.toString()).append("\n").append("\n").append("\n");
								}
								else
								{
									cout << "Current version has that repository but, unfortunately,";
									cout << " seems that the new version repository doesn't exists" << endl;
								}
							}
							else
							{
								cout << "Current url seems not to be valid, am I wrong in something?" << endl;
							}
						}

						if(!repocontent.empty())
						{
							cout << "Writing the new repository file" << endl;
							repo->renameTo(path.append("~"));
							ofstream newrepo(path.c_str());

							if(newrepo.is_open())
							{
								newrepo << repocontent;
								newrepo.close();

								cout << "Congratulations! All done, now exiting..." << endl;
							}
							else
							{
								cout << "Unable to open file";
								ret = RET_UNABLE_TO_WRITE;
							}
						}
					}
					else
					{
						cout << "Repositories file empty or bad repository list" << endl;
						ret = RET_BAD_REPOSITORY_LIST;
					}
				}
				else
				{
					cout << "File doesn't exists or it is not a file" << endl;
					ret = RET_FILE_NOT_FOUND;
				}
			}

			curl_global_cleanup();
		}

		//delete options;
	}
	catch(po::error err)
	{
		cout << err.what() << endl;
		ret = RET_BOOST_PARSING_ERROR;
	}

	return ret;
}
