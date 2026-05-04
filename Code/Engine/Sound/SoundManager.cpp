#include "../Base/Logger/Logger.h"
#include "Exceptions/Exception.h"
#include "Base.h"
#include "Bass.h"
#include "bass_fx.h"
#include "SoundManager.h"
#include "xml/XMLTreeNode.h"
#include "../Graphics/Object3D/Object3D.h"
#include "../PhysX/CharacterController/PhysicController.h"


#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )
  
/// <summary>
/// Posición de la cámara.
/// </summary>
CPhysicController * g_PhysXController = 0;


//TODO:
// Find Pitch (Listo)
// Añadir parametro de volumen a los sonidos (Listo)
// Implementar efecto de prueba
// Implementar Pitch con random
// Probar fireloop sin 3D (Check)
// Sonido de fuego en formato .wav. (Listo)


//NOTAS:
//*Los sonidos que se quieran reproducir infinitamente (looped) tienen que estar en formato .wav.
//*Los .wav tienen que tener una tasa de sampleo de 44100 Hz.


//BASS_ERRORS:
//19 BASS_ERROR_ILLTYPE: 


/// <summary>
/// Inicializa los canales de sonido.
/// </summary>
///<param name="initSoundXML">Ubicación del fichero xml.</param>
///<returns>Boleano que representa si la operación se realizó exitosamente.</returns>
bool CSoundManager::Init (const std::string& initSoundXML)
{
	m_bIsOk = BASS_Init(1, 44100, BASS_DEVICE_3D, 0, NULL) ? true : false;
 

	if (m_bIsOk == true)
	{
		// Parsea el xml
		m_bIsOk = Load(initSoundXML);

		if (m_bIsOk == true)
		{
			m_bIsOk = BASS_Set3DPosition(NULL, NULL, NULL, NULL) ? true : false; 
				
			if (m_bIsOk == true)
			{
				m_bIsOk = BASS_Set3DFactors(1.0, 1.0, 1.0) ? true : false;
				BASS_Apply3D();

				if (m_bIsOk == true)
				{					
					// Asigna el master volume. Usar 1 para volumen máximo
					//m_bIsOk = BASS_SetVolume(1.0f) ? true : false;

					if (m_bIsOk == true)
					{
						m_bIsOk = BASS_Start() ? true : false;

						if (m_bIsOk == true)
						{
							std::string msg_info = "CSoundManager::Init: Sonidos cargados exitosamente.";
							LOGGER->AddNewLog(ELL_INFORMATION, msg_info.c_str());
							return m_bIsOk;
						}
					}
				}
			}
		}
	}
	std::string msg_error = "CSoundManager::Init: Error cargando sonidos.";
	LOGGER->AddNewLog(ELL_INFORMATION, msg_error.c_str());
	return m_bIsOk;
}


/// <summary>
/// Carga el .xml de sonido.
/// </summary>
///<param name="&FileName">Ubicación del archivo xml.</param>
bool CSoundManager::Load(const std::string &FileName)
{
	SSoundData soundData;

  bool l_bIsOk = true;
  m_FileName = FileName;
  CXMLTreeNode filexml;
  if (!filexml.LoadFile(m_FileName.c_str()))
  {
    // Guarda mensaje de error en el log
		std::string msg_error = "CSoundManager::Load->Error al intentar abrir el archivo: ";
		msg_error.append(m_FileName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    l_bIsOk = false;
  }
  else
  {
    CXMLTreeNode cores = filexml["sounds"];
    if (cores.Exists())
    {
			m_iMaxChannels = cores.GetIntProperty("maxChannels");

			// Asignando número de canales de acuerdo al parametro extraído del .xml
			m_Channels.resize(m_iMaxChannels);
 
			if (!cores.IsComment())
			{
				int32 numNodes = cores.GetNumChildren();		
				for(int i=0; i < numNodes; i++)
				{
					if (!cores(i).IsComment())
					{
						// Recibe parametros de los sonidos
						std::string l_sCoreName = cores(i).GetPszProperty("name");
						std::string l_sCoreType = cores(i).GetPszProperty("type");
						std::string l_sCorePath = cores(i).GetPszProperty("path");
            bool l_bCoreLoop = cores(i).GetBoolProperty("loop");
						bool l_bCore3D = cores(i).GetBoolProperty("threeDimensional");
						std::string l_sCorePriority = cores(i).GetPszProperty("priority");
						std::string l_sCoreEffect = cores(i).GetPszProperty("effect");
            float l_fCoreVolume = cores(i).GetFloatProperty("volume");
 
						int iMask = 0;
 
						int PathLastCharacters = l_sCorePath.length() - 3;

						std::string substring = l_sCorePath.substr(PathLastCharacters,3);

						if (substring != "wav")
						{
							// Convert the sample (MP3/MP2/MP1 only) to mono, if it's not already. 
							iMask = BASS_SAMPLE_MONO;
						}
						 
						// Asigna propiedad de loop
						if (l_bCoreLoop)
						{
							iMask |= BASS_SAMPLE_LOOP;
							soundData.m_Loop = true;
						}
						else
						{
							soundData.m_Loop = false;
						}

						// Asigna propiedad 3D y posición 3D
						if (l_bCore3D)
						{
							iMask |= BASS_SAMPLE_3D;
							soundData.m_ThreeDimensional = true;
							soundData.m_Position = cores(i).GetVect3fProperty("position", Vect3f(0.0, 0.0, 0.0));
						}
						else
						{
							soundData.m_ThreeDimensional = false;
						}

						if (l_sCoreType == "sample")
						{
							// Genera identificador para sonido
							soundData.m_type = SAMPLER;
							
							soundData.m_Id = BASS_SampleLoad(false, l_sCorePath.c_str(), 0, 0, m_iMaxChannels, iMask);
						
						}
						else
						{
							// Genera identificador para música
							soundData.m_Id =  BASS_StreamCreateFile(false, l_sCorePath.c_str(), 0, 0, iMask);
							soundData.m_type = BGM;
						}
 
						// Añadiendo volumen
						soundData.m_Volume = l_fCoreVolume;

						// Añade sonido al mapa
						m_Resources[l_sCoreName] = soundData;
					}
				}
			}
    }
    else
    {
      l_bIsOk = false;
    }
  }
  return l_bIsOk;
}


/// <summary>
/// Reproduce sonidos (LShift + J).
/// </summary>
///<param name="SoundName">Nombre del sonido a reproducir.</param>
void CSoundManager::PlaySound (const std::string &SoundName)
{
	bool pass = false;
	std::map<std::string, SSoundData>::iterator iter = m_Resources.find(SoundName);
  
	if(iter == m_Resources.end())
	{
		// Guarda mensaje de error en el log
    std::string msg_error = "CSoundManager::PlaySound->No se encontró el identificador del sonido: ";
		msg_error.append(SoundName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return;
	}

	SSoundData sData = iter->second;
	    
	if (sData.m_type == false) // False si es sample 
	{
		sData.m_Id = BASS_SampleGetChannel(sData.m_Id, FALSE);
		//Agregarlo a canal de sample en el vector m_Channels; 
		//Canales 2 - 19

		if (m_iSamplerChannels == 19)
		{
			// El número de canales de sonidos ha llegado a su máximo
			// Liberar el canal 2 y asignar este sonido a este canal
			m_iSamplerChannels = 2;
		}
		else
		{
			// Asignar esta música al canal que corresponde
			++m_iSamplerChannels;
		}

		m_Channels[m_iSamplerChannels].m_dwSource = sData.m_Id;
		m_Channels[m_iSamplerChannels].m_fVolumen = 1.f;
	}
	// True si es música
	else
	{
	  
		// En caso de que los dos canales de música ya estén asignados
		if (m_iMusicChannels == 1)
		{
			m_iMusicChannels = 0;
		}
		else
		{
			// Asignar esta música al canal 1
			++m_iMusicChannels;
		}

		//Agregarlo a canal de música en el vector m_Channels; 
		m_Channels[m_iMusicChannels].m_dwSource = sData.m_Id;
		m_Channels[m_iMusicChannels].m_fVolumen = 1.f;
	}
  
	// Añade la configuración del canal 
  pass = BASS_ChannelSetAttribute(sData.m_Id, BASS_ATTRIB_VOL, sData.m_Volume) ? true : false;
	
	if (pass == false)
	{
			int errorCode = BASS_ErrorGetCode();
			errorCode = 0;
	}
 
	// Looping sound
	if (sData.m_Loop == true)  
	{
		//BASS_ChannelFlags(sData.m_Id, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP); // set LOOP flag to true
	}

	if (sData.m_ThreeDimensional == true)
	{
		BASS_3DVECTOR vector;

		vector.x = sData.m_Position.x;
		vector.y = sData.m_Position.y;
		vector.z = sData.m_Position.z;
 

		bool result = BASS_ChannelSet3DPosition(sData.m_Id, &vector, NULL, NULL)?true:false;

		if (result == true)
		{
			BASS_Apply3D();
		}
		else
		{
			int errorCode = BASS_ErrorGetCode();
			errorCode = 0;
		}
	}

	if (pass)
	{
		// Reproducción del canal 
		pass = BASS_ChannelPlay(sData.m_Id, FALSE)?true:false;

		if (pass)
		{
			std::string msg_debug = "Sonido reproducido con éxito";
		}
	}	
}
/// <summary>
/// Reproduce sonidos usando el volumen dado.
/// </summary>
///<param name="SoundName">Nombre del sonido a reproducir.</param>
void CSoundManager::PlaySoundVolume (const std::string &SoundName, float _fVolume)
{
	bool pass = false;
	std::map<std::string, SSoundData>::iterator iter = m_Resources.find(SoundName);
  
	if(iter == m_Resources.end())
	{
		// Guarda mensaje de error en el log
    std::string msg_error = "CSoundManager::PlaySound->No se encontró el identificador del sonido: ";
		msg_error.append(SoundName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return;
	}

	SSoundData sData = iter->second;
	    
	if (sData.m_type == false) // False si es sample 
	{
		sData.m_Id = BASS_SampleGetChannel(sData.m_Id, FALSE);
		//Agregarlo a canal de sample en el vector m_Channels; 
		//Canales 2 - 19

		if (m_iSamplerChannels == 19)
		{
			// El número de canales de sonidos ha llegado a su máximo
			// Liberar el canal 2 y asignar este sonido a este canal
			m_iSamplerChannels = 2;
		}
		else
		{
			// Asignar esta música al canal que corresponde
			++m_iSamplerChannels;
		}

		m_Channels[m_iSamplerChannels].m_dwSource = sData.m_Id;
		m_Channels[m_iSamplerChannels].m_fVolumen = _fVolume;
	}
	// True si es música
	else
	{
	  
		// En caso de que los dos canales de música ya estén asignados
		if (m_iMusicChannels == 1)
		{
			m_iMusicChannels = 0;
		}
		else
		{
			// Asignar esta música al canal 1
			++m_iMusicChannels;
		}

		//Agregarlo a canal de música en el vector m_Channels; 
		m_Channels[m_iMusicChannels].m_dwSource = sData.m_Id;
		m_Channels[m_iMusicChannels].m_fVolumen = _fVolume;
	}
  
	// Añade la configuración del canal 
  pass = BASS_ChannelSetAttribute(sData.m_Id, BASS_ATTRIB_VOL, _fVolume) ? true : false;
	
	if (pass == false)
	{
			int errorCode = BASS_ErrorGetCode();
			errorCode = 0;
	}
 
	// Looping sound
	if (sData.m_Loop == true)  
	{
		//BASS_ChannelFlags(sData.m_Id, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP); // set LOOP flag to true
	}

	if (sData.m_ThreeDimensional == true)
	{
		BASS_3DVECTOR vector;

		vector.x = sData.m_Position.x;
		vector.y = sData.m_Position.y;
		vector.z = sData.m_Position.z;
 

		bool result = BASS_ChannelSet3DPosition(sData.m_Id, &vector, NULL, NULL)?true:false;

		if (result == true)
		{
			BASS_Apply3D();
		}
		else
		{
			int errorCode = BASS_ErrorGetCode();
			errorCode = 0;
		}
	}

	if (pass)
	{
		// Reproducción del canal 
		pass = BASS_ChannelPlay(sData.m_Id, FALSE)?true:false;

		if (pass)
		{
			std::string msg_debug = "Sonido reproducido con éxito";
		}
	}	
}
  

/// <summary>
/// Transición entre dos músicas (LShift + K).
/// </summary>
///<param name="MusicName">Nombre de la música a reproducir en el Fade in.</param>
void CSoundManager::CrossFade (const std::string &MusicName, DWORD _ulTime)
{
	// Apagar la música que está sonando actualmente
	FadeOut(_ulTime);
	
	// Fade in de la música que va a sonar	 
	FadeIn(MusicName, _ulTime);

	DWORD MusicId = m_Channels[m_iMusicChannels + 1].m_dwSource;
}
   

/// <summary>
/// Fadeout de la música que esté sonando (LShift + D).
/// </summary>
void CSoundManager::FadeOut(DWORD _ulTime)
{
	try
	{
		//Obtener Id de la música
		DWORD MusicId =  m_Channels[m_iMusicChannels].m_dwSource;
		BASS_ChannelSlideAttribute(MusicId, BASS_ATTRIB_VOL, -1, _ulTime);
	}
	catch(CException e)
  {
		// Guarda mensaje de error en el log
		std::string msg_error = "CSoundManager::FadeOut-> El proceso no se llevó a cabo exitosamente: ";
  }
}
 

/// <summary>
/// FadeIn de la música que está por sonar (LShift + S).
/// </summary>
/// <param name="&MusicName">Nueva música a reproducir.</param>
void CSoundManager::FadeIn(const std::string &MusicName, DWORD _ulTime)
{
	try
	{
		bool pass = true;
		std::map<std::string, SSoundData>::iterator iter = m_Resources.find(MusicName);
  
		if(iter == m_Resources.end())
		{
			// Guarda mensaje de error en el log
      std::string msg_error = "CSoundManager::FadeIn->No se encontró el identificador de la música: ";
			msg_error.append(MusicName.c_str());
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			return;
		}

		SSoundData sData = iter->second;
	 
		// En caso de que los dos canales de música ya estén asignados
		if (m_iMusicChannels == 1)
		{
			m_iMusicChannels = 0;
		}
		else
		{
			// Asignar esta música al canal 1
			m_iMusicChannels = m_iMusicChannels + 1;
		}
	 
		//Agregarlo a canal de música en el vector m_Channels; 
		m_Channels[m_iMusicChannels].m_dwSource = sData.m_Id;
		m_Channels[m_iMusicChannels].m_fVolumen = 1;
	   
		if (pass)
		{
			// Reproducción del canal 
			BASS_ChannelSetAttribute(sData.m_Id, BASS_ATTRIB_VOL, 0);
 
			BASS_ChannelSlideAttribute(sData.m_Id, BASS_ATTRIB_VOL, sData.m_Volume, _ulTime);

			pass = BASS_ChannelPlay(sData.m_Id, FALSE)?true:false;
 
			if (pass)
			{
				std::string msg_debug = "Sonido reproducido con éxito";
			}
		}	
	}
	catch(CException e)
  {
		// Guarda mensaje de error en el log
		std::string msg_error = "CSoundManager::FadeIn-> El proceso no se llevó a cabo exitosamente.";
  }
}
 

/// <summary>
/// Pausa el audio (LShift + P).
/// </summary>
void CSoundManager::Pause()
{
	bool result = false;

	result = BASS_Pause()?true:false;

	if (result == false)
	{
		// Guarda mensaje de error en el log
		std::string msg_error = "CSoundManager::Pause->Error al pausar el audio.";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
}


/// <summary>
/// Reanuda la reproducción de audio (LShift + R).
/// </summary>
void CSoundManager::Resume()
{
	bool result = BASS_Start()?true:false;

	if (result == false)
	{
		// Guarda mensaje de error en el log
		std::string msg_error = "CSoundManager::Pause->Error al reanudar la reproducción del audio.";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
}


/// <summary>
/// Libera memoria.
/// </summary>
void CSoundManager::Release () 
{
	LOGGER->AddNewLog(ELL_INFORMATION, "SoundManager:: shutting down");
	LOGGER->AddNewLog(ELL_INFORMATION, "SoundManager:: offline (ok)");

	//borramos el mapa de sonidos
	m_Resources.clear();

	//borrando el vector de canales
	m_Channels.clear();

	//CHECKED_DELETE(m_pListener);
}


/// <summary>
/// Finalizes data.
/// </summary>
void CSoundManager::Done ()
{
	if (IsOk())
	{
		BASS_Stop();
		BASS_Free();
		Release();
		m_bIsOk = false;
	}
}
 

/// <summary>
///  Obtiene la posición del jugador.
/// </summary>
/// <param name="_pObj3D">Nueva posición del jugador.</param>
void CSoundManager::SetListener(CObject3D* _pObj3D)
{
	m_pListener = _pObj3D;
}
 

/// <summary>
///  Método invocado para actualizar la posición del listener.
/// </summary>
void CSoundManager::UpdateSound(Vect3f _Listener)
{
	UpdateListenerPosition(_Listener);
}


/// <summary>
///  Actualiza la posición del listener.
/// </summary>
void CSoundManager::UpdateListenerPosition(Vect3f _Listener)
{
	if (!m_pListener)
		return;

	bool result = false;
 
	//Vect3f posCamera	= m_pListener->GetPosition();
	//Vect3f posCamera	= g_PhysXController->GetPosition();
  // dejo el nombre de "posCamera" porque asi lo tenia Javier, pero en realidad paso como paramero la posicion del controller del player
  // TODO: (Andres) finalmente, creo que se podria pasar como parametro un object3D para no tener que calcular los vectores, front y top. (o algo asi)
  Vect3f posCamera = _Listener;


	// 1. Convert Vect3f& Pos to BASS_3DVECTOR
	BASS_3DVECTOR pos;
 

	pos.x = posCamera.x;
	pos.y = posCamera.y;
	pos.z = posCamera.z;

	BASS_3DVECTOR top;
	top.x = 0;
	top.y = 1;
	top.z = 0;
	
	//top += pos; 
	top.x = top.x +	pos.x;
	//top.y = top.y +	pos.y;
	top.y = top.y +	1;
	top.z = top.z +	pos.z;
 
	BASS_3DVECTOR front;
 
	front.x = cos(m_pListener->GetYaw());
	front.y = 0;
	front.z = sin(m_pListener->GetYaw());

	//front += pos;
	front.x = front.x +	pos.x;
	front.y = front.y +	pos.y;
	front.z = front.z +	pos.z;
 
	// 2. Set listener's new position
	result = BASS_Set3DPosition(&pos,NULL, &front, &top)?true:false;
	BASS_Apply3D();
}
 

/// <summary>
/// Reproduce un sonido a efecto de realizar pruebas (LShift + H).
/// </summary>
void CSoundManager::TestSound ()
{
	DWORD TestSoundId = BASS_SampleLoad(false, "Data/Sounds/Sample/Muerte/swordSwing.wav", 0, 0, 1, BASS_SAMPLE_MONO);

	// Inicializa el canal 
	BASS_SampleGetChannel(TestSoundId, FALSE);
	// Añade la configuración del canal 
	m_bIsOk = BASS_ChannelSetAttribute(TestSoundId, BASS_ATTRIB_VOL, 100)?true:false;

	// Asigna propiedad de loop
	//BASS_ChannelFlags(TestSoundId, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP); // set LOOP flag

	// Generador de float aleatorio dentro del siguiente rango [-60....0....+60]  
	//float randomSemitoneValue = (rand() % ((-60) - 60 + 1)) + (-60);


	// TO DO: No funciona
	//bool worked = BASS_ChannelSetAttribute(TestSoundId,BASS_ATTRIB_TEMPO_PITCH,randomSemitoneValue)?true:false;

	//if (worked == false)
	//{
	//	int errorCode = BASS_ErrorGetCode();
	//	errorCode = 0;
	//}


	// Reproducción del canal 
	BASS_ChannelPlay(TestSoundId, FALSE);
}
   

///// <summary>
///// Transición entre dos músicas (LShift + K).
///// </summary>
/////<param name="MusicName">Nombre de la música a reproducir en el Fade in.</param>
//void CSoundManager::CrossFade (const std::string &MusicName)
//{
//	// Apagar la música que está sonando actualmente
//	FadeOut();
//	
//	// Fade in de la música que va a sonar	
//	PlayMusic(MusicName);
//}
 

/// <summary>
/// Reproduce música (LShift + G). Método que queda documentado como prueba. Usar Playsound().
/// </summary>
///<param name="MusicName">Nombre de la música a reproducir.</param>
//void CSoundManager::PlayMusic (const std::string &MusicName)
//{
//
//	bool pass = true;
//	std::map<std::string, SSoundData>::iterator iter = m_Resources.find(MusicName);
//  
//	if(iter == m_Resources.end())
//	{
//		// Guarda mensaje de error en el log
//		std::string msg_error = "CSoundManager::PlayMusic->No se encontró el identificador de la música";
//		msg_error.append(m_FileName.c_str());
//		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
//		return;
//	}
//
//	SSoundData sData = iter->second;
// 
//	// En caso de que los dos canales de música ya estén asignados
//	if (m_iMusicChannels == 1)
//	{
//		m_iMusicChannels = 0;
//	}
//	else
//	{
//		// Asignar esta música al canal 1
//		m_iMusicChannels = m_iMusicChannels + 1;
//	}
// 
//	//Agregarlo a canal de música en el vector m_Channels; 
//	m_Channels[m_iMusicChannels].m_dwSource = sData.m_Id;
//	m_Channels[m_iMusicChannels].m_fVolumen = 1;
//   
//	if (pass)
//	{
//		// Reproducción del canal 
//		pass = BASS_ChannelPlay(sData.m_Id, FALSE);
//
//		if (pass)
//		{
//			std::string msg_debug = "Sonido reproducido con éxito";
//		}
//	}	
//}
 

/// <summary>
/// Reproduce samples (LShift + F). Método que queda documentado como prueba. Usar Playsound().
/// </summary>
///<param name="SampleName">Nombre del sample a reproducir.</param>
//void CSoundManager::PlaySample (const std::string &SampleName)
//{
//	bool pass = true;
//	std::map<std::string, SSoundData>::iterator iter = m_Resources.find(SampleName);
//  
//	if(iter == m_Resources.end())
//	{
//		// Guarda mensaje de error en el log
//		std::string msg_error = "CSoundManager::PlaySample->No se encontró el identificador del sample";
//		msg_error.append(m_FileName.c_str());
//		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
//		return;
//	}
//
//	SSoundData sData = iter->second;
// 
//	sData.m_Id = BASS_SampleGetChannel(sData.m_Id, FALSE);
//	//Agregarlo a canal de sample en el vector m_Channels; 
//	//Canales 2 - 19
//
//	if (m_iSamplerChannels == 19)
//	{
//		// El número de canales de sonidos ha llegado a su máximo
//		// Liberar el canal 2 y asignar este sonido a este canal
//		m_iSamplerChannels = 2;
//	}
//	else
//	{
//		// Asignar esta música al canal que corresponde
//		m_iSamplerChannels = m_iSamplerChannels + 1;
//	}
//
//	m_Channels[m_iSamplerChannels].m_dwSource = sData.m_Id;
//	m_Channels[m_iSamplerChannels].m_fVolumen = 1;
//
//	// Añade la configuración del canal 
//	pass = BASS_ChannelSetAttribute(sData.m_Id, BASS_ATTRIB_VOL, 100);
//	
//	if (!pass)
//	{
//		// Reproducción del canal 
//		pass = BASS_ChannelPlay(sData.m_Id, FALSE);
//
//		// Es aquí donde está el problema
//		if (pass)
//		{
//			std::string msg_debug = "Sonido reproducido con éxito";
//		}
//	}	
//}