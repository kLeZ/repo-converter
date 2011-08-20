#include <iostream>
#include <Poco/File.h>
#include <curl/curl.h>

#include "repository.h"
#include "anyoption.h"
#include "utils.h"

using namespace std;
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
		curl_global_init(CURL_GLOBAL_ALL);
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

			cout << "Path is " + path + "\n";
			cout << "Current version is " + prev_ver + "\n";
			cout << "Next version is " + foll_ver + "\n";


			File *repo = new File(path);

			if(repo->isFile() && repo->exists())
			{
				vector<Repository> repositories = Utils::getRepositories(repo);

				if(!repositories.empty())
				{
					for(int i = 0; i < repositories.size(); i++)
					{
						cout << repositories[i].toString();
						cout << "\n\n";
					}
				}
			}
		}
	}

	options->~AnyOption();
	return 0;
}
