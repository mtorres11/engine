#include "CameraKeyController.h"
#include "CameraManager.h"
#include "Camera.h"
#include <assert.h>
//#include "Core/Engine.h"
//#include "Engine/PSParams.h"

CCameraKeyController::CCameraKeyController(const std::string &FileName, CCamera* _pCamera, bool _bCycle, bool _bReverse)
: m_CurrentKey(0)
, m_NextKey(1)
, m_CurrentTime(0.0f)
, m_TotalTime(0.0f)
, m_Cycle(_bCycle)
, m_Reverse(_bReverse)
, m_Finished(false)
, m_pCamera(_pCamera)
{
	LoadXML(FileName);
	if(m_Reverse)
		m_CurrentTime = m_TotalTime;
}

void CCameraKeyController::LoadXML(const std::string &FileName)
{
  //LoadFile(FileName.c_str());
  CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CCameraKeyController::LoadXML->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

  CXMLTreeNode  l_treeNode = parser["camera_key_controller"];
  m_Name = l_treeNode.GetPszProperty("name");

	if (l_treeNode.Exists() && !l_treeNode.IsComment())
	{
		int count = l_treeNode.GetNumChildren();

    float l_fTime = 0.0f;
    for (int i = 0; i < count; ++i)
		{
      CCameraInfo l_CameraInfo(l_treeNode(i));
      float l_fTime = l_treeNode(i).GetFloatProperty("time", 0.f);
      //m_TotalTime += l_fTime;
      CCameraKey l_CameraKey = CCameraKey(l_CameraInfo, l_fTime);
      m_Keys.push_back(l_CameraKey);
    }

    if(!m_Keys.empty())
			m_TotalTime = m_Keys[m_Keys.size()-1].m_Time;
  }
}

void CCameraKeyController::GetCurrentKey()
{
	if(m_Cycle)
	{
		if(m_Reverse)
			while(m_CurrentTime<0.0f)
				m_CurrentTime+=m_TotalTime;
		else
			while(m_CurrentTime>m_TotalTime)
				m_CurrentTime-=m_TotalTime;

	}
	else
	{
		if(m_CurrentTime>m_TotalTime)
		{
			m_CurrentTime=m_TotalTime;
			m_Finished=true;
		}
		else if(m_CurrentTime<0.0f)
		{
			m_CurrentTime=0.0f;
			m_Finished=true;
		}
	}

	if(m_Reverse)
	{
		while(!(m_CurrentTime>=m_Keys[m_CurrentKey].m_Time && m_CurrentTime<=m_Keys[m_NextKey].m_Time))
		{
			m_NextKey=m_CurrentKey;
			m_CurrentKey=(m_CurrentKey==0) ? m_Keys.size()-1 : m_CurrentKey-1;
		}
	}
	else
	{
		while(m_CurrentTime>m_Keys[m_NextKey].m_Time || m_CurrentTime<m_Keys[m_CurrentKey].m_Time)
		{
			m_NextKey=(m_NextKey+1)%m_Keys.size();
			m_CurrentKey=(m_NextKey==0) ? m_Keys.size()-1 : m_NextKey-1;
		}
	}
}

void CCameraKeyController::Update(float ElapsedTime)
{
	if(m_Keys.size()<2 || m_TotalTime<=0.0f)
		return;
	
	if(m_Reverse)
		m_CurrentTime-=ElapsedTime;//*0.001f;
	else
		m_CurrentTime+=ElapsedTime;//*0.001f;

	GetCurrentKey();
	float l_Pct=(m_CurrentTime-m_Keys[m_CurrentKey].m_Time)/(m_Keys[m_NextKey].m_Time-m_Keys[m_CurrentKey].m_Time);

	assert(l_Pct>=0.0f && l_Pct<=1.0f);
	
	//Vect3f l_Up = m_Keys[m_CurrentKey].m_CameraInfo.m_Up*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_Up*l_Pct;
  //m_LookAt = m_Keys[m_CurrentKey].m_CameraInfo.m_LookAt*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_LookAt*l_Pct;
	//float l_fAspectRatio = 16.0f / 9.0f;
	
  Vect3f l_V3fPosition = m_Keys[m_CurrentKey].m_CameraInfo.m_Eye*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_Eye*l_Pct;
  float l_fYaw = m_Keys[m_CurrentKey].m_CameraInfo.m_fYaw*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_fYaw*l_Pct; //m_Keys[m_CurrentKey].m_CameraInfo.m_fYaw;
  float l_fPitch = m_Keys[m_CurrentKey].m_CameraInfo.m_fPitch*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_fPitch*l_Pct; //m_Keys[m_CurrentKey].m_CameraInfo.m_fPitch;
  float l_fRoll = m_Keys[m_CurrentKey].m_CameraInfo.m_fRoll*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_fRoll*l_Pct; //m_Keys[m_CurrentKey].m_CameraInfo.m_fRoll;
	float l_fFOV = m_Keys[m_CurrentKey].m_CameraInfo.m_FOV*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_FOV*l_Pct;
	float l_fZNear = m_Keys[m_CurrentKey].m_CameraInfo.m_NearPlane*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_NearPlane*l_Pct;
	float l_fZFar = m_Keys[m_CurrentKey].m_CameraInfo.m_FarPlane*(1.0f-l_Pct)+m_Keys[m_NextKey].m_CameraInfo.m_FarPlane*l_Pct;

  m_pCamera->SetParameters(l_V3fPosition, l_fYaw, l_fPitch, l_fRoll, l_fFOV/*, l_fAspectRatio*/, l_fZNear, l_fZFar);
}
