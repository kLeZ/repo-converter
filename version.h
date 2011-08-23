#ifndef VERSION_H
#define VERSION_H

#include <string>

#include "utils.h"

using namespace std;
using namespace freax::libzypp;


class Version
{
	public:
		static const int MAJOR_VERSION = 0;
		static const int MINOR_VERSION = 1;
		static const int PATCH_VERSION = 0;
		static string getVersionString();
};

string Version::getVersionString()
{
	string ret;
	string __maj;
	string __min;
	string __pat;

	char* execname;

	__maj = Utils::itos(MAJOR_VERSION);
	__min = Utils::itos(MINOR_VERSION);
	__pat = Utils::itos(PATCH_VERSION);
	readlink("/proc/self/exe", execname, 50);
	ret = ret.append(execname).append(" ");
	ret = ret.append(__maj).append(".").append(__min).append(".").append(__pat);
	return ret;
}

#endif // VERSION_H
