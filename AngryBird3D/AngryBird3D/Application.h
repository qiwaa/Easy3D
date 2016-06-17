#ifndef application_h_
#define application_h_
#include <tchar.h>
#include <string>
#include <iostream>

using namespace std;

class Application1
{
	int i;
public:
	static wstring getFullFilePathInResource(wstring fileName);
};

#endif