#include "IA.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <float.h>
#include "RenderManager.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

using namespace std;

std::map<std::string, SNodo*>C_IA::m_Grafo;

#define CABECERA 0xFF77
#define FINFICHERO 0x77FF
#define MAXNUMEROCHARNODO 16
#define MAXNUMEROVECINOS 4

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool C_IA::Init()
{
  m_bIsOk = true;

  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void C_IA::Done()
{
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void C_IA::Release()
{
  m_vMejorCamino.erase(m_vMejorCamino.begin(), m_vMejorCamino.end());
  m_vMejorCamino.clear();

  m_Info.erase(m_Info.begin(), m_Info.end());

  std::map<std::string,SNodo*>::iterator l_iterGrafo = m_Grafo.begin();
  for(;l_iterGrafo != m_Grafo.end();++l_iterGrafo)
  {
    CHECKED_DELETE(l_iterGrafo->second);
  }
  m_Grafo.clear();
}

//----------------------------------------------------------------------------
// Busca el nodo mas cercano a una posicion
//----------------------------------------------------------------------------
std::string C_IA::BuscaNodoMasCercano(Vect3f posicion)
{
  std::string l_sNodo;
  float maxDistance = FLT_MAX;
  std::map<std::string,SNodo*>::iterator l_iterGrafo = m_Grafo.begin();

  for(;l_iterGrafo != m_Grafo.end(); ++l_iterGrafo)
  {
    float dist = posicion.SqDistance(l_iterGrafo->second->posicion);
    if(dist < maxDistance)
    {
      maxDistance = dist;
      l_sNodo = l_iterGrafo->first;
    }
  }
  return l_sNodo;
}

//----------------------------------------------------------------------------
// Dibuja Dummies en la escena
//----------------------------------------------------------------------------
void C_IA::DibujaDummies(CRenderManager*rm)
{
  std::map<std::string, SNodo*>::iterator l_iter = m_Grafo.begin();
  std::vector<SArista>::iterator l_iterVecinos;
  for(;l_iter != m_Grafo.end();++l_iter)
  {//TODO..girar x, z correctamente si es necesario
    //dibuja nodos
    rm->DrawCircleH(l_iter->second->posicion.x, l_iter->second->posicion.z, 1.0f, colGREEN);
    l_iterVecinos = l_iter->second->vecinos.begin();

    for(;l_iterVecinos != l_iter->second->vecinos.end();++l_iterVecinos)
    {
      rm->DrawLine (l_iter->second->posicion, m_Grafo[l_iterVecinos->idVecino]->posicion,colWHITE);
    }
  }
}

//----------------------------------------------------------------------------
// Load
//----------------------------------------------------------------------------
bool C_IA::CargaGrafo(const std::string &_Filename)
{
  ifstream l_File(_Filename.c_str(), ios::in|ios::binary);
  if(l_File.is_open())
  {
    unsigned short l_Header = 0;
    unsigned short l_Footer = 0;

    //Lectura del Header
    l_File.read((char *)&l_Header, sizeof(unsigned short));
    if(l_Header != CABECERA)
    {
      LOGGER->AddNewLog(ELL_WARNING, "IA::Load Encabezado incorrecto al leer el archivo '%'",_Filename);
      l_File.close(); 
      return false;
    }
  
    //Numero de dummies para adquirir sus posiciones
    unsigned short l_NumDummies = 0;
    l_File.read((char *)&l_NumDummies, sizeof(unsigned short));
    
    for(int i=0; i < l_NumDummies ; i++)
    {
      //Nombre de nodo
      unsigned short l_iTamNombreNodo = 0;
      l_File.read((char *)&l_iTamNombreNodo, sizeof(unsigned short));      
      if(l_iTamNombreNodo > MAXNUMEROCHARNODO)//maximo de caracteres
      {
        LOGGER->AddNewLog(ELL_WARNING, "IA::Load tamaño del nombre del nodo incorrecto al leer el archivo '%'",_Filename);
        l_File.close(); 
        return false;
      }
      char* l_sNombreNodo = new char[l_iTamNombreNodo+1];
      l_File.read(l_sNombreNodo, sizeof(char) * (l_iTamNombreNodo + 1));
        
      //Posicion
      Vect3f miPosicion(0.0f,0.0f,0.0f);
      l_File.read((char*)&miPosicion.x, sizeof(float));
      l_File.read((char*)&miPosicion.y, sizeof(float));
      l_File.read((char*)&miPosicion.z, sizeof(float));


      SNodo *nodo = new SNodo();
      //TODO..cambio coordenadas de x por z
      nodo->posicion.x = miPosicion.x - 5.0f;
      nodo->posicion.y = miPosicion.y;
      nodo->posicion.z = miPosicion.z + 5.0f;
      
      m_Grafo[l_sNombreNodo] = nodo;
      delete[]l_sNombreNodo;
    }

    for(int i=0; i < l_NumDummies ; i++)
    {
      //Nombre de nodo
      unsigned short l_iTamNombreNodo = 0;
      l_File.read((char *)&l_iTamNombreNodo, sizeof(unsigned short));      
      if(l_iTamNombreNodo > MAXNUMEROCHARNODO)//maximo de caracteres
      {
        LOGGER->AddNewLog(ELL_WARNING, "IA::Load tamaño del nombre del nodo incorrecto al leer el archivo '%'",_Filename);
        l_File.close(); 
        return false;
      }
      char* l_sNombreNodo = new char[l_iTamNombreNodo+1];
      l_File.read(l_sNombreNodo, sizeof(char) * (l_iTamNombreNodo + 1));
  
      //Numero de vecinos
      unsigned short l_iNumVecinos = 0;
      l_File.read((char *)&l_iNumVecinos, sizeof(unsigned short));      
      if(l_iNumVecinos > MAXNUMEROVECINOS)//maximo numero de vecinos
      {
        LOGGER->AddNewLog(ELL_WARNING, "IA::Load numero de vecinos incorrectos al leer el archivo '%'",_Filename);
        l_File.close(); 
        return false;
      }

      //para vecinos
      for(int i=0;i<l_iNumVecinos;i++)
      {
        //Nombre de nodo
        unsigned short l_iTamNombreNodoVecino = 0;
        l_File.read((char *)&l_iTamNombreNodoVecino, sizeof(unsigned short));      
        if(l_iTamNombreNodoVecino > MAXNUMEROCHARNODO)//maximo de caracteres
        {
          LOGGER->AddNewLog(ELL_WARNING, "IA::Load tamaño del nombre del nodo incorrecto al leer el archivo '%'",_Filename);
          l_File.close(); 
          return false;
        }
        char* l_sNombreNodoVecino = new char[l_iTamNombreNodoVecino + 1];
        l_File.read(l_sNombreNodoVecino, sizeof(char) * (l_iTamNombreNodoVecino + 1));

        //Calcula distancia
        Vect3f l_PosicionNodoActual(0.0f,0.0f,0.0f);
        Vect3f l_PosicionVecino(0.0f,0.0f,0.0f);
        l_PosicionNodoActual = m_Grafo[l_sNombreNodo]->posicion;
        l_PosicionVecino = m_Grafo[l_sNombreNodoVecino]->posicion;

        float l_fPesoArista = l_PosicionNodoActual.Distance(l_PosicionVecino);

        SNodo* _Nodo;
        map<string,SNodo*>::iterator l_MapaIterator = m_Grafo.find(l_sNombreNodo);
        _Nodo = l_MapaIterator->second;
        //nodos vecinos en vector vecinos con nombre y distancia
        SArista l_Arista;
        l_Arista.distancia = l_fPesoArista;
        l_Arista.idVecino = l_sNombreNodoVecino;
        
        //nodo principal en el mapa
        _Nodo->vecinos.push_back(l_Arista);
        delete[] l_sNombreNodoVecino;
      }
      delete[] l_sNombreNodo;
    }

    l_File.read((char *)&l_Footer, sizeof(unsigned short));

    if(l_Footer != FINFICHERO)
    {
      LOGGER->AddNewLog(ELL_WARNING, "IA::Load fin fichero incorrecto al leer el archivo '%'",_Filename);
      l_File.close(); 
      return false;
    }

  }
  return true;
}

//----------------------------------------------------------------------------
// Busqueda camino mas corto
//----------------------------------------------------------------------------
void C_IA::Dijsktra(const std::string &_origen,const std::string &_destino)
{   
  m_Info.clear();
  m_vMejorCamino.clear();
  map<string,SNodo*>::iterator l_iter = m_Grafo.begin();

  std::string l_sNombreNodoActual = _origen;
  
  for(;l_iter != m_Grafo.end();++l_iter)
  {
    SInfoNodo info;
    info.s_bVisitado = false;
    info.s_fCosteActual = 0.0;
    info.s_sPadre = ""; 
    m_Info[l_iter->first] = info;
  }

   m_Info[l_sNombreNodoActual].s_bVisitado = true;
   m_Info[l_sNombreNodoActual].s_fCosteActual = 0.0f;
   m_Info[l_sNombreNodoActual].s_sPadre = l_sNombreNodoActual;

   std::map<string,SNodo*>::iterator l_iterGrafo;
   std::vector<SArista>::iterator l_iterVectorAristas;
   std::map<string, SInfoNodo>::iterator l_IterInfo; 

   while(strcmp(l_sNombreNodoActual.c_str(),_destino.c_str()))
  {
    std::string l_sNombrePadre = m_Info[l_sNombreNodoActual].s_sPadre;
    float l_fCostePadre = m_Info[l_sNombreNodoActual].s_fCosteActual;
    l_iterGrafo = m_Grafo.find(l_sNombreNodoActual);
    
    l_iterVectorAristas = l_iterGrafo->second->vecinos.begin();

    for(;l_iterVectorAristas != l_iterGrafo->second->vecinos.end();++l_iterVectorAristas)
    {
      if(!m_Info[l_iterVectorAristas->idVecino].s_bVisitado)
      {
        if((m_Info[l_iterVectorAristas->idVecino].s_fCosteActual == 0.0f)
          ||((m_Info[l_iterVectorAristas->idVecino].s_fCosteActual) > (l_fCostePadre + l_iterVectorAristas->distancia)))
        {    
            m_Info[l_iterVectorAristas->idVecino].s_sPadre = l_sNombreNodoActual;
            m_Info[l_iterVectorAristas->idVecino].s_fCosteActual = l_fCostePadre + l_iterVectorAristas->distancia; 
        }
      }
    }     

    l_IterInfo = m_Info.begin();
    
    float costeminimo = FLT_MAX;
    std::string l_sMejorNodo;
    for( ; l_IterInfo != m_Info.end() ; ++l_IterInfo)
    { 
      if((!l_IterInfo->second.s_bVisitado)&&  
        (strcmp(l_IterInfo->second.s_sPadre.c_str(),""))&&  
        (l_IterInfo->second.s_fCosteActual < costeminimo)) //el coste es menor que el minimo
      {
        l_sMejorNodo = l_IterInfo->first;
        costeminimo = l_IterInfo->second.s_fCosteActual;
      }      
    }  

    m_Info[l_sMejorNodo].s_bVisitado = true;
    l_sNombreNodoActual = l_sMejorNodo;

    if(l_sNombreNodoActual == _destino)
    {
      while(strcmp(_origen.c_str(), l_sNombreNodoActual.c_str()))
      {
        l_iterGrafo = m_Grafo.find(l_sNombreNodoActual);
        m_vMejorCamino.push_back(l_iterGrafo->first);
        l_sNombreNodoActual = m_Info[l_iterGrafo->first].s_sPadre;
      }
      break;
    }
  }
}

//----------------------------------------------------------------------------
// Busqueda camino mas corto A*
//----------------------------------------------------------------------------
void C_IA::AStar(const std::string &_origen,const std::string &_destino, CHeuristica &_funcionHeuristica)
{   
  m_Info.clear();
  m_vMejorCamino.clear();
  map<string,SNodo*>::iterator l_iter = m_Grafo.begin();
  std::string l_sNombreNodoActual = _origen;
  
  for(;l_iter != m_Grafo.end();++l_iter)
  {
    SInfoNodo info;
    info.s_bVisitado = false;
    info.s_fCosteActual = 0.0;
    info.s_sPadre = ""; 
    m_Info[l_iter->first] = info;  
  }

   m_Info[l_sNombreNodoActual].s_bVisitado = true;
   m_Info[l_sNombreNodoActual].s_fCosteActual = 0.0f;
   m_Info[l_sNombreNodoActual].s_sPadre = l_sNombreNodoActual;

   std::map<string,SNodo*>::iterator l_iterGrafo;
   std::vector<SArista>::iterator l_iterVectorAristas;
   std::map<string, SInfoNodo>::iterator l_IterInfo; 

   std::map<string,SNodo*>::iterator l_iterGrafoNodo_Final = m_Grafo.find(_destino);
   Vect3f p2(l_iterGrafoNodo_Final->second->posicion); 
   
   while(strcmp(l_sNombreNodoActual.c_str(),_destino.c_str()))
  {
    std::string l_sNombrePadre = m_Info[l_sNombreNodoActual].s_sPadre;
    float l_fCostePadre = m_Info[l_sNombreNodoActual].s_fCosteActual;
    l_iterGrafo = m_Grafo.find(l_sNombreNodoActual);
    Vect3f p1(l_iterGrafo->second->posicion);

    l_iterVectorAristas = l_iterGrafo->second->vecinos.begin();

    for(;l_iterVectorAristas != l_iterGrafo->second->vecinos.end();++l_iterVectorAristas)
    {
      if(!m_Info[l_iterVectorAristas->idVecino].s_bVisitado)
      {
        if((m_Info[l_iterVectorAristas->idVecino].s_fCosteActual == 0.0f)
          ||((m_Info[l_iterVectorAristas->idVecino].s_fCosteActual) > 
          (l_fCostePadre + l_iterVectorAristas->distancia + _funcionHeuristica.AplicaHeuristica(p1,p2))))
        {    
            m_Info[l_iterVectorAristas->idVecino].s_sPadre = l_sNombreNodoActual;
            m_Info[l_iterVectorAristas->idVecino].s_fCosteActual 
              = l_fCostePadre 
              + l_iterVectorAristas->distancia 
              + _funcionHeuristica.AplicaHeuristica(p1,p2); 
        }
      }
    }     

    l_IterInfo = m_Info.begin();
    
    float costeminimo = FLT_MAX;

    std::string l_sMejorNodo;
    for( ; l_IterInfo != m_Info.end() ; ++l_IterInfo)
    { 
      if((!l_IterInfo->second.s_bVisitado)&&  
        (strcmp(l_IterInfo->second.s_sPadre.c_str(),""))&&  
        (l_IterInfo->second.s_fCosteActual < costeminimo)) 
      {
        l_sMejorNodo = l_IterInfo->first;
        costeminimo = l_IterInfo->second.s_fCosteActual;
      }      
    }  

    m_Info[l_sMejorNodo].s_bVisitado = true;
    l_sNombreNodoActual = l_sMejorNodo;

    if(l_sNombreNodoActual == _destino)
    {
      while(strcmp(_origen.c_str(), l_sNombreNodoActual.c_str()))
      {
        l_iterGrafo = m_Grafo.find(l_sNombreNodoActual);
        m_vMejorCamino.push_back(l_iterGrafo->first);
        l_sNombreNodoActual = m_Info[l_iterGrafo->first].s_sPadre;
      }
      break;
    }
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------