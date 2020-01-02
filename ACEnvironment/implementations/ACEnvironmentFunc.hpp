#pragma once

#include "../stdafx.h"

#include "APIdefs_Environment.h"
#include "ACExport.h"
#include "ACCast.h"

// --- PyAPI_ProjectInfo --------------------------------------------------------------------

class PyAPI_ProjectInfo {
public:
	PyAPI_ProjectInfo() : m_ProjectInfo(){
		this->m_ExtFuncs = GetExtFuncs();
		this->Load();
	}

	~PyAPI_ProjectInfo() {

	}

	void Load() {
		this->m_ExtFuncs->ACAPI_Environment(APIEnv_ProjectID, &this->m_ProjectInfo, NULL, NULL);
	}

	bool getUntitled() {
		return this->m_ProjectInfo.untitled;
	}

	bool getTeamwork() {
		return this->m_ProjectInfo.teamwork;
	}

	short getUserId() {
		return this->m_ProjectInfo.userId;
	}

	int getWorkGroupMode() {
		return this->m_ProjectInfo.workGroupMode;
	}

	unsigned long long getModiStamp() {
		return this->m_ProjectInfo.modiStamp;
	}

private:
	API_ProjectInfo m_ProjectInfo;
	ExportFuns		*m_ExtFuncs;
};

// --- PyAPI_UserInfo -----------------------------------------------------------------------

class PyAPI_UserInfo {
public:
	PyAPI_UserInfo(API_UserInfo *info){
		this->m_UserInfo = info;
	}

	~PyAPI_UserInfo() {

	}

	GS::UniString *getLoginName() {
		return new GS::UniString(this->m_UserInfo->loginName);
	}

	GS::UniString *getFullName() {
		return new GS::UniString(this->m_UserInfo->fullName);
	}

	GS::Guid getGuid() {
		return APIGuid2GSGuid(this->m_UserInfo->guid);
	}

	short getUserId() {
		return this->m_UserInfo->userId;
	}

	bool getConnected() {
		return this->m_UserInfo->connected;
	}

private:
	API_UserInfo	*m_UserInfo;
};

// --- PyAPI_SharingInfo --------------------------------------------------------------------

class PyAPI_SharingInfo
{
public:
	PyAPI_SharingInfo() : userInfos(){
		memset(&this->m_SharingInfo, 0, sizeof(API_SharingInfo));
		this->m_ExtFuncs= GetExtFuncs();
		this->Load();
	}

	~PyAPI_SharingInfo() {
		this->clear();
	}

	void Load() {
		this->clear();
		this->m_ExtFuncs->ACAPI_Environment(APIEnv_ProjectSharingID, &this->m_SharingInfo, NULL, NULL);
		for (int i = 0; i < this->m_SharingInfo.nUsers; i++) {
			API_UserInfo *user = (API_UserInfo *)(*this->m_SharingInfo.users+i);
			PyAPI_UserInfo *info = new PyAPI_UserInfo(user);
			this->userInfos.Push(info);
		}
	}

	int getNUser() {
		return this->m_SharingInfo.nUsers;
	}

	GS::Array< PyAPI_UserInfo*> *GetUserInfos() {
		return &this->userInfos;
	}

private:
	GS::Array<PyAPI_UserInfo*>	userInfos;
	API_SharingInfo				m_SharingInfo;
	ExportFuns					*m_ExtFuncs;

	void clear(){
		for (int i = 0; i < this->userInfos.GetSize(); i++) {		// clear userInfos
			PyAPI_UserInfo *info = this->userInfos.Get(i);
			this->userInfos.Delete(i);
			delete info;
		}
		this->userInfos.Clear();

		memset(&this->m_SharingInfo,0,sizeof(API_SharingInfo));
	}

};


// --- load_ProjectInfo ---------------------------------------------------------------------

void load_ProjectInfo(py::module m) {
	py::class_<PyAPI_ProjectInfo>(m, "ProjectInfo")
		.def(py::init<>())
		.def("Flush", &PyAPI_ProjectInfo::Load)
		.def("getUntitled", &PyAPI_ProjectInfo::getUntitled)
		.def("getTeamwork", &PyAPI_ProjectInfo::getTeamwork)
		.def("getUserId",&PyAPI_ProjectInfo::getUserId)
		.def("getWorkGroupMode", &PyAPI_ProjectInfo::getWorkGroupMode)
		.def("getModiStamp", &PyAPI_ProjectInfo::getModiStamp)
		;
}

// --- load_UserInfo ------------------------------------------------------------------------

void load_UserInfo(py::module m) {
	py::class_<PyAPI_UserInfo>(m, "UserInfo")
		//.def(py::init<API_UserInfo *>())
		.def("getLoginName", &PyAPI_UserInfo::getLoginName)
		.def("getFullName", &PyAPI_UserInfo::getFullName)
		.def("getGuid", &PyAPI_UserInfo::getGuid)
		.def("getUserId", &PyAPI_UserInfo::getUserId)
		.def("getConnected", &PyAPI_UserInfo::getConnected)
		;
}

// --- load_SharingInfo --------------------------------------------------------------------

void load_SharingInfo(py::module m) {
	py::class_<PyAPI_SharingInfo>(m, "SharingInfo")
		.def(py::init<>())
		.def("Load", &PyAPI_SharingInfo::Load)
		.def("getNUser",&PyAPI_SharingInfo::getNUser)
		.def("GetUserInfos", &PyAPI_SharingInfo::GetUserInfos,py::return_value_policy::reference)
		;
}