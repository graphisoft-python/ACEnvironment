// ACPyModuleTutorials.cpp : 定义 DLL 应用程序的导出函数。
//

#include "../stdafx.h"
#include "../implementations/ACEnvironmentFunc.hpp"


PYBIND11_MODULE(ACEnvironment, m) {

	m.def("version", []() {	return "version 0.0.1"; });


	// --- Add bindings here ------------------------------------------------------------------

	// --- ACEnvironment
	load_ProjectInfo(m);
	load_SharingInfo(m);
	load_UserInfo(m);

	// --- Add bindings end -------------------------------------------------------------------
}