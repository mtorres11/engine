#ifndef CAMERAKEYCONTROLLER_H_
#define CAMERAKEYCONTROLLER_H_

#include <vector>

#include "XML/XMLTreeNode.h"
#include "Math/MathUtils.h"
#include "Math/Vector3.h"
//#include "ThPSCamera.h"
//#include "Camera/CameraController.h"
//#include "expat/XMLParser.h"
//#include "Utils.h"

class CCamera;

class CCameraInfo
{
public:
	float					m_NearPlane, m_FarPlane;
	float					m_FOV;
  float         m_fYaw, m_fPitch, m_fRoll;
	Vect3f				m_Eye;//, m_LookAt;
	//Vect3f				m_Up;

	CCameraInfo()
		: m_NearPlane(1.0f)
		, m_FarPlane(10.0f)
		, m_FOV(1.0f)
		, m_fYaw(0.0f)
		, m_fPitch(0.0f)
		, m_fRoll(0.0f)
		, m_Eye(1.0f,0.0f,0.0f)
	{
	}
	CCameraInfo(CCamera* _pCamera, const Vect3f &Eye, float _fYaw, float _fPitch, float _fRoll, float NearPlane, float FarPlane, float FOV)
		: m_NearPlane(NearPlane)
		, m_FarPlane(FarPlane)
		, m_FOV(FOV)
		, m_fYaw(_fYaw)
		, m_fPitch(_fPitch)
		, m_fRoll(_fRoll)
		, m_Eye(Eye)
	{
	}
	CCameraInfo(CXMLTreeNode &atts)
	{
    m_FarPlane = atts.GetFloatProperty("far_plane", 0.0f);
    m_FOV = mathUtils::Deg2Rad(atts.GetFloatProperty("fov", 0.0f));
    m_fYaw = mathUtils::Deg2Rad(atts.GetFloatProperty("yaw", 0.0f));
    m_fPitch = mathUtils::Deg2Rad(atts.GetFloatProperty("pitch", 0.0f));
    m_fRoll = mathUtils::Deg2Rad(atts.GetFloatProperty("roll", 0.0f));
    //m_LookAt = atts.GetVect3fProperty("look_at", Vect3f(0.0f, 0.0f, 0.0f));
    m_NearPlane = atts.GetFloatProperty("near_plane", 0.0f);
    m_Eye = atts.GetVect3fProperty("pos",Vect3f(0.0f, 0.0f, 0.0f));
		//m_Up = Vect3f(0.0f, 1.0f, 0.0f);
	}
};

class CCameraKeyController
{
private:
	class CCameraKey
	{
		public:
			CCameraInfo						m_CameraInfo;
			float							m_Time;
			CCameraKey(CCameraInfo &CameraInfo, float Time)
				: m_CameraInfo(CameraInfo)
				, m_Time(Time)
			{
			}
	};

	std::vector<CCameraKey>					m_Keys;
	size_t									m_CurrentKey, m_NextKey;
	float									m_CurrentTime, m_TotalTime;
	bool									m_Cycle;
	bool									m_Reverse;

  bool                  m_Finished;
  CCamera*              m_pCamera;
  std::string           m_Name;

	void LoadXML(const std::string &FileName);
	void GetCurrentKey();
public:
  CCameraKeyController(const std::string &FileName, CCamera* _pCamera, bool _bCycle = false, bool _bReverse = false);
  bool Finished() { return m_Finished; }
	void Update(float ElapsedTime);
	void SetCurrentTime(float CurrentTime) {m_CurrentTime=CurrentTime;}
	void ResetTime() { m_CurrentTime=0.0f; m_Finished = false; }
};
#endif
		
