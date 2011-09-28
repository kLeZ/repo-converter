#ifndef VERSION_H
#define VERSION_H

#include <ios>
#include <sstream>
#include <fstream>
#include <iostream>
#include <Poco/File.h>

#include "utils.h"

using namespace std;
using namespace freax::libzypp;


class Version
{
	public:
		static const int MAJOR_VERSION = 0;
		static const int MINOR_VERSION = 1;
		static const int PATCH_VERSION = 0;
		void printVersion();
		string getVersionString();
} version;

void Version::printVersion()
{
	cout << version.getVersionString() << endl;
}

string Version::getVersionString()
{
	Utils utils;
	string ret, __maj, __min, __pat, execname;
	char* execlnk;

	__maj = utils.itos(MAJOR_VERSION);
	__min = utils.itos(MINOR_VERSION);
	__pat = utils.itos(PATCH_VERSION);
	File comm("/proc/self/comm");
	ifstream scanner(comm.path().c_str(), ios::out);
	getline(scanner, execname);

	ret = ret.append(execname).append(" ");
	ret = ret.append(__maj).append(".").append(__min).append(".").append(__pat);
	return ret;
}

#endif // VERSION_H
