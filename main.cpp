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


int main(int argc, char **argv)
{
	int ret = RET_OK;

	try
	{
		Utils utils;
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

			showhelp = opts.getBOption(enums::help);
			showversion = opts.getBOption(enums::version);
			tofile = !opts.getBOption(enums::stdout);
			validateurls = !opts.getBOption(enums::not_validate_urls);
			backup = opts.getBOption(enums::backup);

			path = opts.getSOption(enums::file_repo);
			prev_ver = opts.getSOption(enums::current);
			foll_ver = opts.getSOption(enums::next);

			if(showhelp)
			{
				cout << opts << endl;
			}
			else if(showversion)
			{
				version.printVersion();
			}
			else
			{
				opts.notify();

				if(validateurls) utils.curlInit();

				if(opts.verbosityLevel() >= enums::verbose)
				{
					cout << "Repository File: " << path << endl;
					cout << "Current version: " << prev_ver << endl;
					cout << "Next version: " << foll_ver << endl;
					cout << "Debug mode: " << (opts.verbosityLevel() == enums::debug ? "On" : "Off") << endl;
					cout << "Print to stdout: " << (tofile ? "No" : "Yes") << endl;
					cout << "Use CURL: " << (validateurls ? "Yes" : "No") << endl;
					cout << "Be Verbose: " << (opts.verbosityLevel() == enums::verbose ? "Yes" : "No") << endl;
				}

				File *repo = new File(path);

				if(repo->isFile() && repo->exists())
				{
					if(opts.verbosityLevel() >= enums::debug)
						cout << "Repository file exists! and has a size of " << repo->getSize() << " b" << endl;

					vector<Repository> *repositories = Repository::getRepositories(repo);

					if(!repositories->empty())
					{
						string repocontent = "";
						int done = 0;

						if(opts.verbosityLevel() >= enums::verbose)
							cout << "Total repositories found: " << repositories->size() << endl;

						if(opts.verbosityLevel() >= enums::verbose && validateurls)
							cout << "New urls has to be verified later!" << endl;

						if(opts.verbosityLevel() >= enums::verbose && validateurls)
							cout << "New urls has to be verified later!" << endl;

						for(int i = 0; i < repositories->size(); i++)
						{
							Repository repository = repositories->at(i);
							repository.setTitle(utils.changeVersionToken(repository.getTitle(), prev_ver, foll_ver));
							repository.setName(utils.changeVersionToken(repository.getName(), prev_ver, foll_ver));
							URI newuri = utils.changeVersionUrl(repository.getBaseurl(), prev_ver, foll_ver);

							if(validateurls)
							{
								if(opts.verbosityLevel() >= enums::verbose)
								{
									cout << "Checking repository '" << repository.getName();
									cout << "' with url '" << repository.getBaseurl().toString() << "'" << endl;
								}

								if(utils.isValid(repository.getBaseurl()))
								{
									if(opts.verbosityLevel() >= enums::verbose)
									{
										cout << "Url seems to be valid, changing to next version and checking again" << endl;
										cout << "Checking new version url '" << newuri.toString() << "'..." << endl;
									}

									if(utils.isValid(newuri))
									{
										if(opts.verbosityLevel() >= enums::verbose)
											cout << "Next version url is valid, so can add the repository" << endl;

										repository.setBaseurl(newuri);
										repocontent = repocontent.append(repository.toString());
										repocontent = repocontent.append("\n").append("\n").append("\n");
										done++;
									}
									else
									{
										if(opts.verbosityLevel() >= enums::verbose)
										{
											cout << "Current version has that repository but, unfortunately,";
											cout << " seems that the new version repository doesn't exists" << endl;
										}
									}
								}
								else
								{
									if(opts.verbosityLevel() >= enums::verbose)
										std::cerr << "Current url seems not to be valid, am I wrong in something?" << endl;
								}
							}
							else
							{
								repository.setBaseurl(newuri);
								repocontent = repocontent.append(repository.toString());
								repocontent = repocontent.append("\n").append("\n").append("\n");
							}
						}

						if(opts.verbosityLevel() >= enums::verbose)
							cout << "Number of repositories that can be written: " << done << endl;

						if(tofile)
						{
							if(!repocontent.empty())
							{
								if(backup)
								{
									string bakpath = path;
									bakpath.append("~");
									repo->renameTo(bakpath);
								}
								else repo->remove();

								delete repo;

								if(opts.verbosityLevel() >= enums::verbose)
									cout << "Writing the new repository file to " << path << endl;

								ofstream newrepo(path.c_str());

								if(opts.verbosityLevel() >= enums::debug)
									cout << "New repo file status: " << newrepo.is_open() << endl;

								if(newrepo.is_open())
								{
									if(opts.verbosityLevel() >= enums::debug)
										cout << "Number of characters of repocontent: " << repocontent.size() << endl;

									newrepo << repocontent << endl;
									newrepo.flush();

									if(opts.verbosityLevel() >= enums::debug)
										cout << "newrepo stream is bad: " << newrepo.bad() << endl;

									newrepo.close();

									if(opts.verbosityLevel() >= enums::verbose)
										cout << "Congratulations! All done, now exiting..." << endl;
								}
								else
								{
									std::cerr << "Unable to open file";
									ret = RET_UNABLE_TO_WRITE;
								}
							}
						}
						else std::cout << repocontent << endl;
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

	if(ret > 0) std::cerr << "Exited with return code " << ret << endl;
	else cout << "Exited with return code " << ret << endl;

	return ret;
}
