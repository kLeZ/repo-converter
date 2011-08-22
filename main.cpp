#include <iostream>
#include <Poco/File.h>
#include <Poco/URI.h>

#include "repository.h"
#include "anyoption.h"
#include "utils.h"

using namespace std;
using namespace Poco;
using namespace freax::libzypp;

int main(int argc, char **argv)
{
	AnyOption *options = Utils::initOptions(argc, argv);

	if(!options->hasOptions())
	{
		options->printUsage();
	}
	else
	{
		Utils::curlInit();
		string path;
		string prev_ver;
		string foll_ver;

		if(options->getFlag('h') || options->getFlag("help"))
			options->printUsage();
		else
		{
			if(options->getValue("file-repo") != NULL || options->getValue('f') != NULL)
				path = options->getValue('f');

			if(options->getValue('c') != NULL)
				prev_ver = options->getValue('c');

			if(options->getValue('n') != NULL)
				foll_ver = options->getValue('n');


			cout << "Repository File: " << path << "\n";
			cout << "Current version: " << prev_ver << "\n";
			cout << "Next version: " << foll_ver << "\n";


			File *repo = new File(path);

			if(repo->isFile() && repo->exists())
			{
				vector<Repository> repositories = Utils::getRepositories(repo);

				if(!repositories.empty())
				{
					string repocontent = "";

					for(int i = 0; i < repositories.size(); i++)
					{
						Repository repository = repositories[i];

						cout << "Checking repository '" << repository.getName();
						cout << "' with url '" << repository.getBaseurl().toString() << "'\n";

						if(Utils::isValid(repository.getBaseurl()))
						{
							cout << "Url seems to be valid, changing to next version repository and checking again\n";
							string newurl = repository.getBaseurl().toString();
							newurl = newurl.replace(newurl.find_first_of(prev_ver), prev_ver.size(), foll_ver);
							URI newuri = URI(newurl);

							cout << "Checking new version url '" << newuri.toString() << "'...";

							if(Utils::isValid(newuri))
							{
								cout << "Next version url is valid, so can add the repository\n";
								repository.setBaseurl(newuri);
								repocontent = repocontent.append(repository.toString()).append("\n").append("\n").append("\n");
							}
							else
							{
								cout << "Current version has that repository but, unfortunately,";
								cout << " seems that the new version repository doesn't exists\n";
							}
						}
						else
						{
							cout << "Current url seems not to be valid, am I wrong in something?\n";
						}
					}

					if(!repocontent.empty())
					{
						cout << "Writing the new repository file\n";
						repo->renameTo(path.append("~"));
						ofstream newrepo(path.c_str());

						if(newrepo.is_open())
						{
							newrepo << repocontent;
							newrepo.close();

							cout << "Congratulations! All done, now exiting...\n";
						}
						else cout << "Unable to open file";
					}
				}
				else
				{
					cout << "Repositories file empty or bad repository list\n";
				}
			}
			else
			{
				cout << "File doesn't exists or it is not a file\n";
			}
		}

		curl_global_cleanup();
	}

	options->~AnyOption();
	return 0;
}
