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

	GS::UniString* getProjectPath() {
		return this->m_ProjectInfo.projectPath;
	}

	GS::UniString* getProjectName() {
		return this->m_ProjectInfo.projectName;
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

// --- PyAPI_ProjectNoteInfo ----------------------------------------------------------------

class PyAPI_ProjectNoteInfo {
public:
	PyAPI_ProjectNoteInfo() : m_ProjectNoteInfo() {
		this->m_ExtFuncs = GetExtFuncs();
		this->Load();
	}

	~PyAPI_ProjectNoteInfo() {

	}

	void Load() {
		this->m_ExtFuncs->ACAPI_Environment(APIEnv_GetProjectNotesID, &this->m_ProjectNoteInfo, NULL, NULL);
	}

	GS::UniString *getClient() {
		return new GS::UniString(this->m_ProjectNoteInfo.client);
	}

	GS::UniString *getProjectName() {
		return new GS::UniString(this->m_ProjectNoteInfo.projectName);
	}

	GS::UniString *getCompany() {
		return new GS::UniString(this->m_ProjectNoteInfo.company);
	}

	GS::UniString *getStreet() {
		return new GS::UniString(this->m_ProjectNoteInfo.street);
	}

	GS::UniString *getCity() {
		return new GS::UniString(this->m_ProjectNoteInfo.city);
	}

	GS::UniString *getCountry() {
		return new GS::UniString(this->m_ProjectNoteInfo.country);
	}

	GS::UniString *getCode() {
		return new GS::UniString(this->m_ProjectNoteInfo.code);
	}

	GS::UniString *getArchitect() {
		return new GS::UniString(this->m_ProjectNoteInfo.architect);
	}

	GS::UniString *getDraftsmen() {
		return new GS::UniString(this->m_ProjectNoteInfo.draftsmen);
	}

	GS::UniString *getProjectStatus() {
		return new GS::UniString(this->m_ProjectNoteInfo.projectStatus);
	}

	GS::UniString *getDateOfIssue() {
		return new GS::UniString(this->m_ProjectNoteInfo.dateOfIssue);
	}

	GS::UniString *getKeywords() {
		return new GS::UniString(this->m_ProjectNoteInfo.keywords);
	}

	GS::UniString *getNotes() {
		return new GS::UniString(this->m_ProjectNoteInfo.notes);
	}

private:
	API_ProjectNoteInfo m_ProjectNoteInfo;
	ExportFuns			*m_ExtFuncs;
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
		.def("getProjectPath", &PyAPI_ProjectInfo::getProjectPath, py::return_value_policy::reference)
		.def("getProjectName", &PyAPI_ProjectInfo::getProjectName, py::return_value_policy::reference)
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

// --- load_ProjectNoteInfo ----------------------------------------------------------------

void load_ProjectNoteInfo(py::module m) {
	py::class_<PyAPI_ProjectNoteInfo>(m, "ProjectNoteInfo")
		.def(py::init<>())
		.def("getClient", &PyAPI_ProjectNoteInfo::getClient, py::return_value_policy::reference)
		.def("getProjectName", &PyAPI_ProjectNoteInfo::getProjectName, py::return_value_policy::reference)
		.def("getCompany", &PyAPI_ProjectNoteInfo::getCompany, py::return_value_policy::reference)
		.def("getStreet", &PyAPI_ProjectNoteInfo::getStreet, py::return_value_policy::reference)
		.def("getCity", &PyAPI_ProjectNoteInfo::getCity, py::return_value_policy::reference)
		.def("getCountry", &PyAPI_ProjectNoteInfo::getCountry, py::return_value_policy::reference)
		.def("getCode", &PyAPI_ProjectNoteInfo::getCode, py::return_value_policy::reference)
		.def("getArchitect", &PyAPI_ProjectNoteInfo::getArchitect, py::return_value_policy::reference)
		.def("getDraftsmen", &PyAPI_ProjectNoteInfo::getDraftsmen, py::return_value_policy::reference)
		.def("getProjectStatus", &PyAPI_ProjectNoteInfo::getProjectStatus, py::return_value_policy::reference)
		.def("getDateOfIssue", &PyAPI_ProjectNoteInfo::getDateOfIssue, py::return_value_policy::reference)
		.def("getKeywords", &PyAPI_ProjectNoteInfo::getKeywords, py::return_value_policy::reference)
		.def("getNotes", &PyAPI_ProjectNoteInfo::getNotes, py::return_value_policy::reference)
		;
}