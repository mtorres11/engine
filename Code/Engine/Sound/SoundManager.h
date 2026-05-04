#include "MapManager/MapManager.h"
#include "Math\MathTypes.h"
#include "Math\Matrix44.h"
#include "Math\Vector3.h"

#pragma once

// forward declaration
class CObject3D;

#ifndef INC_SOUND_MANAGER_H_
#define INC_SOUND_MANAGER_H_
 

/// <summary>
/// Almacena el volumen actual del canal y el canal que se está utilizando
/// </summary>
struct SChannel
{
	float							m_fVolumen;       // Volume 
	DWORD							m_dwSource;       // Channel ID
};
 
 
/// <summary>
/// Almacena el volumen actual del canal y el canal que se está utilizando
/// </summary>
struct SSoundData
{
	DWORD							m_Id;								// Identificador del sonido	
	bool							m_type;							// Tipo de sonido (Music || Sample)
	bool							m_Loop;							// Parametro loop
	bool							m_ThreeDimensional; // Parametro tercera dimensión
	float							m_Volume;           // Volumen
	Vect3f				    m_Position;					// Posición 3D del sonido		
	std::string				m_Effect;					// Tipo de efecto (En caso de que tenga. En caso contrario el valor es "none")
};

#define SAMPLER 0;
#define BGM 1;

/// <summary>
/// Estructura para eco.
/// </summary>
struct SEco
{
	float fLevel; 
	int lDelay; 
};
 


/// <summary>
/// Lee y gestiona los archivos de sonido
/// </summary>
class CSoundManager
{
	public:
		CSoundManager(): m_iMusicChannels(0), m_iSamplerChannels(2), m_pListener(NULL), m_bIsOk(false) {}
 
		virtual ~CSoundManager() { Done(); }  

		void Done            											();
		bool Init																	(const std::string& initSoundXML);
		void Release															();
		void UpdateSound													(Vect3f _Listener);
		bool IsOk                  								() const { return m_bIsOk; }
		bool Load																	(const std::string &FileName);
		void CrossFade														(const std::string &SoundName, DWORD _ulTime=5000);
		void FadeOut   														(DWORD _ulTime);
		void FadeIn   														(const std::string &SoundName, DWORD _ulTime=5000);
 

		void SetListener													(CObject3D* _pObj3D);
		void UpdateListenerPosition								(Vect3f _Listener);
		void PlaySound														(const std::string &SoundName);
		void PlaySoundVolume											(const std::string &SoundName, float _fVolume);
		void PlaySample3D													();
		
		void PlayMusic														(const std::string &SoundName);
		void PlaySample														(const std::string &SoundName);
		
		void PlayMusic3D													();
		void StopAll															();
		void StopMusics														();
		void StopSounds														();
		void SetMasterVolume											();
		void Pause																();
		void Resume																();
		 
		void TestSound														();
		DWORD m_soundId;
 
	private:	
		bool																			 m_bIsOk;	          // Initialization boolean control
		int																				 m_iMaxChannels;     // Cantidad máxima de canales establecidos en Sounds.xml
		short																			 m_iMusicChannels;   // Contador de canales de música  (1 y 2)
		short																			 m_iSamplerChannels; // Contador de canales de samples (3 al 20)
		std::vector<SChannel>											 m_Channels;     
		std::map<std::string, SSoundData>      		 m_Resources;        // Mapa que sonidos (nombre, identificador, tipo)
		CObject3D*																 m_pListener;
	 

protected:
		std::string m_FileName;
};
#endif //INC_SOUND_MANAGER_H_