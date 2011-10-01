#include <ios>
#include <sstream>
#include <fstream>
#include <iostream>
#include <Poco/File.h>
#include <Poco/Exception.h>
#include <Poco/URI.h>
#include <boost/program_options.hpp>
#include <curl/curl.h>

#include "repository.h"
#include "utils.h"
#include "version.h"
#include "options.h"


const int RET_OK = 0;
const int RET_FILE_NOT_FOUND = 1;
const int RET_BAD_REPOSITORY_LIST = 2;
const int RET_UNABLE_TO_WRITE = 3;
const int RET_BOOST_PARSING_ERROR = 4;
const int RET_EXCEPTION_UNCAUGHT = 98;
const int RET_GENERIC_ERROR = 99;

using namespace std;
using namespace Poco;
using namespace freax::libzypp;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
	int ret = RET_OK;
	Utils utils;

	try
	{
		options opts;
		opts.initOptions();
		
		opts.parseCommandLine(argc, argv);

		if(opts.empty())
		{
			version.printVersion();
			cout << opts << endl;
		}
		else
		{
			string path, prev_ver, foll_ver;
			bool validateurls, tofile, showhelp, showversion, backup;

			showhelp = opts.getOption<bool>("help");
			showversion = opts.getOption<bool>("version");
			tofile = !opts.getOption<bool>("stdout");
			validateurls = !opts.getOption<bool>("not-validate-urls");
			backup = opts.getOption<bool>("backup");

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

				if(utils.verbose)
				{
					cout << "Repository File: " << path << endl;
					cout << "Current version: " << prev_ver << endl;
					cout << "Next version: " << foll_ver << endl;
					cout << "Debug mode: " << (utils.debug ? "On" : "Off") << endl;
					cout << "Print to stdout: " << (tofile ? "No" : "Yes") << endl;
					cout << "Use CURL: " << (validateurls ? "Yes" : "No") << endl;
					cout << "Be Verbose: " << (utils.verbose ? "Yes" : "No") << endl;
				}

				File *repo = new File(path);

				if(repo->isFile() && repo->exists())
				{
					if(utils.debug) cout << "Repository file exists! and has a size of " << repo->getSize() << " b" << endl;

					vector<Repository> *repositories = utils.getRepositories(repo);

					if(!repositories->empty())
					{
						string repocontent = "";
						int done = 0;

						if(utils.verbose) cout << "Total repositories found: " << repositories->size() << endl;

						if(utils.verbose && validateurls) cout << "New urls has to be verified later!" << endl;

						if(utils.verbose && validateurls) cout << "New urls has to be verified later!" << endl;

						for(int i = 0; i < repositories->size(); i++)
						{
							Repository repository = repositories->at(i);
							repository.setTitle(utils.changeVersionToken(repository.getTitle(), prev_ver, foll_ver));
							repository.setName(utils.changeVersionToken(repository.getName(), prev_ver, foll_ver));
							URI newuri = utils.changeVersionUrl(repository.getBaseurl(), prev_ver, foll_ver);

							if(validateurls)
							{
								if(utils.verbose)
								{
									cout << "Checking repository '" << repository.getName();
									cout << "' with url '" << repository.getBaseurl().toString() << "'" << endl;
								}

								if(utils.isValid(repository.getBaseurl()))
								{
									if(utils.verbose)
									{
										cout << "Url seems to be valid, changing to next version and checking again" << endl;
										cout << "Checking new version url '" << newuri.toString() << "'..." << endl;
									}

									if(utils.isValid(newuri))
									{
										if(utils.verbose)
											cout << "Next version url is valid, so can add the repository" << endl;

										repository.setBaseurl(newuri);
										repocontent = repocontent.append(repository.toString());
										repocontent = repocontent.append("\n").append("\n").append("\n");
										done++;
									}
									else
									{
										if(utils.verbose)
										{
											cout << "Current version has that repository but, unfortunately,";
											cout << " seems that the new version repository doesn't exists" << endl;
										}
									}
								}
								else
								{
									if(utils.verbose)
										std::cerr << "Current url seems not to be valid, am I wrong in something?" << endl;
								}
							}
							else
							{
								repository.setBaseurl(newuri);
								repocontent = repocontent.append(repository.toString());
								repocontent = repocontent.append("\n").append("\n").append("\n");							}
						}

						if(utils.verbose) cout << "Number of repositories that can be written: " << done << endl;

						if(tofile)
						{
							if(!repocontent.empty())
							{
								if (backup)
								{
									string bakpath = path;
									bakpath.append("~");
									repo->renameTo(bakpath);
								}
								else repo->remove();
								delete repo;

								if(utils.verbose) cout << "Writing the new repository file to " << path << endl;

								ofstream newrepo(path.c_str());

								if (utils.debug) cout << "New repo file status: " << newrepo.is_open() << endl;

								if(newrepo.is_open())
								{
									if (utils.debug) cout << "Number of characters of repocontent: " << repocontent.size() << endl;

									newrepo << repocontent << endl;
									newrepo.flush();
									if (utils.debug) cout << "newrepo stream is bad: " << newrepo.bad() << endl;
									newrepo.close();

									if(utils.verbose) cout << "Congratulations! All done, now exiting..." << endl;
								}
								else
								{
									std::cerr << "Unable to open file";
									ret = RET_UNABLE_TO_WRITE;
								}
							}
						}
						else std::cerr << repocontent << endl;
					}
					else
					{
						std::cerr << "Repositories file empty or bad repository list" << endl;
						ret = RET_BAD_REPOSITORY_LIST;
					}
				}
				else
				{
					std::cerr << "File doesn't exists or it is not a file" << endl;
					ret = RET_FILE_NOT_FOUND;
				}
			}

			curl_global_cleanup();
		}
	}
	catch(po::error err)
	{
		std::cerr << err.what() << endl;
		ret = RET_BOOST_PARSING_ERROR;
	}
	catch(Poco::FileNotFoundException fnfe)
	{
		std::cerr << fnfe.what() << endl;
		ret = RET_FILE_NOT_FOUND;
	}
	catch(std::bad_exception bex)
	{
		std::cerr << bex.what() << endl;
		ret = RET_EXCEPTION_UNCAUGHT;
	}
	catch(...)
	{
		std::cerr << "Oops! Something wrong, and I don't know why. Sorry." << endl;
		ret = RET_GENERIC_ERROR;
	}

	if (ret > 0) std::cerr << "Exited with return code " << ret << endl;
	else cout << "Exited with return code " << ret << endl;

	return ret;
}
