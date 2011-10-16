#ifndef VERSION_H
#define VERSION_H


#include "utils.h"

using namespace std;
using namespace freax::libzypp;


class Version
{
	public:
		static const int MAJOR_VERSION = 0;
		static const int MINOR_VERSION = 2;
		static const int PATCH_VERSION = 0;
		void printVersion();
		string getVersionString();
} version;

void Version::printVersion()
{
	cout << this->getVersionString() << endl;
}

string Version::getVersionString()
{
	string ret, __maj, __min, __pat, execname;
	char* execlnk;

	Utils utils;
	__maj = utils.itos(MAJOR_VERSION);
	__min = utils.itos(MINOR_VERSION);
	__pat = utils.itos(PATCH_VERSION);
	File comm("/proc/self/comm");
	ifstream scanner(comm.path().c_str());
	getline(scanner, execname);

	ret = ret.append(execname).append(" ");
	ret = ret.append(__maj).append(".").append(__min).append(".").append(__pat);
	return ret;
}

#endif // VERSION_H
