#include "Application.h"
#include "WindowsRelated.h"

wstring Application1::getFullFilePathInResource(wstring fileName)
{

	wchar_t last[1000] = L"\\AngryBird3D\\Resources\\";		//图片在工程内位置
	wchar_t buf[1000];												//图片在硬盘的位置
	GetModuleFileName(NULL,buf,sizeof(buf));
	//wcout<<"Texture Location："<<buf<<endl;
	for (int i=wcslen(buf)-1,k=2;i>=0&&k>0;i--)
	{
		if (buf[i]==L'\\')
		{
			buf[i] = L'\0';
			k--;
		}
	}

	wstring str(buf);
	str.append(last);
	str.append(fileName);

	wcout<<"Texture Location："<<str<<endl;
	return str;
}
