// Copyright 2016 Chase Willden and The CondorLang Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "path.h"

namespace Condor {
namespace internal{

	Path::Path(Isolate* iso){
		isolate = iso;
	}

	Path* Path::New(Isolate* iso){
		void* p = (Path*) iso->GetMemory(sizeof(Path));
		Path* path = new(p) Path(iso);
		return path;
	}

	// TODO: Set Locale - http://askubuntu.com/questions/236924/matlab-not-working
	void Path::SetBase(std::string str){
		#ifdef _WIN32
			std::replace(str.begin(), str.end(), '/', '\\');
			int BUFSIZE = 4096;
		    TCHAR  buffer[BUFSIZE];
		    TCHAR** lppPart = {NULL};

		 	GetFullPathName(str.c_str(), (DWORD) BUFSIZE, buffer, lppPart);
		    std::string pth(buffer);
		    base = pth;
		#else
		  char buffer[PATH_MAX + 1];
			base = realpath(str.c_str(),  buffer);
		#endif
	}

	std::string Path::GetFromBase(std::string str){
		char ch = '/';
		#ifdef _WIN32
			ch = '\\';
		#endif
		int idx = -1;
		for (int i = (int) base.length() - 1; i >= 0; i--){
			if (base.at((unsigned long long int) i) == ch){
				idx = i;
				break;
			}
		}
		std::string folder = base.substr(0, (unsigned long long int) idx);
		folder += ch + str;


		#ifdef _WIN32
			std::replace(folder.begin(), folder.end(), '/', '\\');
			int BUFSIZE = 4096;
		    TCHAR  buffer[BUFSIZE];
		    TCHAR** lppPart={NULL};

		    GetFullPathName(folder.c_str(), (DWORD) BUFSIZE, buffer, lppPart);
		    std::string pth(buffer);
		    return pth;
		#else
			try {
				return realpath(folder.c_str(), NULL);
			}
			catch (...){
				return "-1";
			}
		#endif
	}

	std::string Path::GetCWD(){
 		#ifdef _WIN32
 			char buffer[MAX_PATH];
	    GetModuleFileName(NULL, buffer, MAX_PATH);
	    std::size_t found = std::string(buffer).rfind("\\Condor\\build");
	    if (found != std::string::npos){
	    	return std::string(buffer).substr(0, found) + "\\Condor";
	    }
	    return "";
 		#else
 			char cCurrentPath[FILENAME_MAX];
 			if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){} // TODO: Throw Error
 			std::string result(cCurrentPath);
	 		return result;
 		#endif
	}

	std::string Path::GetLibDir(){
		std::string path = Path::GetCWD();
		#ifdef _WIN32
			path += "\\libs\\";
		#else
			path += "/libs/";
		#endif
		return path;
	}

} // namespace internal
}