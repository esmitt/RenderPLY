#include "GLSLProgram.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include "OGLBasic.h"
using namespace std;

CGLSLProgram::CGLSLProgram(void)
{
	m_vIdShader[VERTEX] = 0;
	m_vIdShader[FRAGMENT] = 0;
	m_vIdShader[GEOMETRY] = 0;
	m_vIdShader[TESSELATION] = 0;
	m_mapVarShader.clear();
	m_mapSubroutines.clear();
	m_uIdProgram = -1;
}


CGLSLProgram::~CGLSLProgram(void)
{
	m_mapVarShader.clear();
	m_mapSubroutines.clear();
	deleteShaders();
}

void CGLSLProgram::deleteShaders()
{
	if (m_uIdProgram > 0)
	{
		glDeleteShader(m_vIdShader[VERTEX]);
		glDeleteShader(m_vIdShader[FRAGMENT]);
		glDeleteShader(m_vIdShader[GEOMETRY]);
		glDeleteProgram(m_uIdProgram);
		m_uIdProgram - 1;
		TRACE("Program deleted!\n");
	}
}

///
bool CGLSLProgram::loadShaderFile(std::string strFilename, GLuint iHandle)
{
	std::ifstream shaderSource(strFilename.c_str());
	if (!shaderSource.is_open())
  {
   std::cerr<< " File not found "<< strFilename.c_str()<< endl;
   return false;
  }
	// now read in the data
  string strSource = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
  shaderSource.close();
  strSource+="\0";
	//pass the code to OGL
	const char* data=strSource.c_str();
  glShaderSource(iHandle, 1, &data, NULL);
	return true;
}

void CGLSLProgram::recompileShader(std::string strFileName, SHADERTYPE typeShader)
{
	glDetachShader(m_uIdProgram, m_vIdShader[typeShader]);
	loadShader(strFileName, typeShader);
	glAttachShader(m_uIdProgram, m_vIdShader[typeShader]);
	//link the program
	glLinkProgram(m_uIdProgram);
	checkLinkingErrors();
	glDeleteShader(m_vIdShader[typeShader]);
	std::cout << "de pinga" << std::endl;
}

void CGLSLProgram::loadShader(std::string strFileName, SHADERTYPE typeShader)
{
	GLuint hShader = 0;
	GLint status;

	//Create shader object
	switch (typeShader)
  {
    case VERTEX : { hShader = glCreateShader(GL_VERTEX_SHADER); break; }
    case FRAGMENT : { hShader = glCreateShader(GL_FRAGMENT_SHADER); break; }
    case GEOMETRY : { hShader = glCreateShader(GL_GEOMETRY_SHADER); break; }
	case TESSELATION : { hShader = 0; std::cerr<<"not implemented.... yet :-)" << std::endl; }
  }

	if(loadShaderFile(strFileName, hShader))
	{
		//now compile the shader
		glCompileShader(hShader);
		glGetShaderiv(hShader, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE)
		{
			char infoLog[1024];
			glGetShaderInfoLog(hShader, 1024, NULL, infoLog);
			TRACE("The shader at %s  failed to compile with the following errors : %s\n", strFileName.c_str(), infoLog);
			glDeleteShader(hShader);
		}
		else	//here, everything is OK
		{
			//TRACE("The shader at %s was compiled without errors.\n", strFileName.c_str());
			m_vIdShader[typeShader] = hShader;
		}
	}
	else
	{
		TRACE("something wrong loading the shader located in %s .\n");
		glDeleteShader(hShader);
	}
}

void CGLSLProgram::checkLinkingErrors()
{
	GLint infologLength = 0;
	glGetProgramiv(m_uIdProgram, GL_INFO_LOG_LENGTH, &infologLength);
  //std::cerr<<"Link Log Length "<<infologLength<<"\n";

  if(infologLength > 1)
  {
    char *infoLog = new char[infologLength];
    GLint charsWritten  = 0;

    glGetProgramInfoLog(m_uIdProgram, infologLength, &charsWritten, infoLog);

		TRACE(infoLog);
    delete [] infoLog;
    glGetProgramiv(m_uIdProgram, GL_LINK_STATUS, &infologLength);
    if(infologLength == GL_FALSE)
    {
      TRACE("Program link failed exiting\n");
      exit(EXIT_FAILURE);
    }
  }
}

void CGLSLProgram::create()
{
	m_uIdProgram = glCreateProgram();
	//attach the shaders
	if(m_vIdShader[VERTEX] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[VERTEX]);
	if(m_vIdShader[FRAGMENT] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[FRAGMENT]);
	if(m_vIdShader[GEOMETRY] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[GEOMETRY]);
		//delete the shaders
	checkLinkingErrors();
}

void CGLSLProgram::create_link()
{
	m_uIdProgram = glCreateProgram();
	//attach the shaders
	if(m_vIdShader[VERTEX] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[VERTEX]);
	if(m_vIdShader[FRAGMENT] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[FRAGMENT]);
	if(m_vIdShader[GEOMETRY] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[GEOMETRY]);
	//link the program
	glLinkProgram(m_uIdProgram);
	//check errors on linking
	checkLinkingErrors();
	//delete the shaders
	//glDeleteShader(m_vIdShader[VERTEX]);
	//glDeleteShader(m_vIdShader[FRAGMENT]);
	//glDeleteShader(m_vIdShader[GEOMETRY]);
}

void CGLSLProgram::link()
{
	//link the program
	glLinkProgram(m_uIdProgram);
	//check errors on linking
	checkLinkingErrors();
	//glDeleteShader(m_vIdShader[VERTEX]);
	//glDeleteShader(m_vIdShader[FRAGMENT]);
	//glDeleteShader(m_vIdShader[GEOMETRY]);
}

void CGLSLProgram::enable()
{
	glUseProgram(m_uIdProgram);
}

void CGLSLProgram::disable()
{
	glUseProgram(0);
}

GLuint CGLSLProgram::getId()
{
	return m_uIdProgram;
}

void CGLSLProgram::addAttribute(std::string strParName)
{
	m_mapVarShader[strParName] = glGetAttribLocation(m_uIdProgram, strParName.c_str());
}

GLint CGLSLProgram::getLocation(std::string strParName)
{
	std::map<string, GLint>::iterator it = m_mapVarShader.find(strParName);
	if(it == m_mapVarShader.end())
		return -1;
	else
		return m_mapVarShader[strParName];
}

void CGLSLProgram::addUniform(std::string strParName)
{
	m_mapVarShader[strParName] = glGetUniformLocation(m_uIdProgram, strParName.c_str());
}

void CGLSLProgram::addUniformSubroutine(std::string strParName, int iShaderType)
{
	//m_vRoutinesIds.front()
	m_mapVarShader[strParName] = glGetSubroutineUniformLocation(m_uIdProgram, iShaderType, strParName.c_str());
}

void CGLSLProgram::addSubroutine(std::string strFunctionName, unsigned int iShaderType)
{
	//m_vRoutinesIds.push_back(glGetSubroutineIndex(m_uIdProgram, iShaderType, strFunctionName.c_str()));
	m_mapSubroutines[strFunctionName] = glGetSubroutineUniformLocation(m_uIdProgram, iShaderType, strFunctionName.c_str());
}

//only for 1 subroutine at time
void CGLSLProgram::setSubroutine(std::string strUniformName, std::string strSubRoutine, int iShaderType)
{
	int index = getLocation(strUniformName);
	if(index > -1)
	{
		GLuint routine_index = glGetSubroutineIndex(m_uIdProgram, iShaderType, strSubRoutine.c_str());
		glUniformSubroutinesuiv(iShaderType, 1, &routine_index);
	}
}

void CGLSLProgram::showDebugging()
{
	GLint iMaxLength, iNumAttrib, iNumUniform;
	glGetProgramiv(m_uIdProgram, GL_ACTIVE_ATTRIBUTES, &iNumAttrib);
	glGetProgramiv(m_uIdProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &iMaxLength);
	GLchar* name = new GLchar[iMaxLength];
	GLint written, size, location;
	GLenum type;
	std::cout << "Index | Attribute Name" << std::endl;
	std::cout << std::string(23, '-') << std::endl;
	for(int i = 0; i < iNumAttrib; i++)
	{
		glGetActiveAttrib(m_uIdProgram, i, iMaxLength, &written,	&size, &type, name);
		location = glGetAttribLocation(m_uIdProgram, name);
		std::cout << setw(5) << location << " | " << name << "," << std::endl;
	}
	delete name;
	glGetProgramiv(m_uIdProgram, GL_ACTIVE_UNIFORMS, &iNumUniform);
	glGetProgramiv(m_uIdProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &iMaxLength);
	name = new GLchar[iMaxLength];
	std::cout << "Index | Uniform Name" << std::endl;
	std::cout << std::string(21, '-') << std::endl;
	for (int i = 0; i < iNumUniform; i++)
	{
		glGetActiveUniform(m_uIdProgram, i, iMaxLength, &written,	&size, &type, name);
		location = glGetUniformLocation(m_uIdProgram, name);
		std::cout << setw(5) << location << " | " << name << "," << std::endl;
	}
	delete name;
	/*
	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(m_uIdProgram, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
	glGetProgramInterfaceiv(m_uIdProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveAttribs);
	
	std::vector<GLchar> nameData(256);
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);
	std::vector<GLint> values(properties.size());
	//std::cout << numActiveAttribs << endl;
	for(int attrib = 0; attrib < numActiveAttribs; ++attrib)
	{
		glGetProgramResourceiv(m_uIdProgram, GL_PROGRAM_INPUT, attrib, properties.size(),
			&properties[0], values.size(), NULL, &values[0]);
		
		nameData.resize(properties[0]); //The length of the name.
		glGetProgramResourceName(m_uIdProgram, GL_PROGRAM_INPUT, attrib, nameData.size(), NULL, &nameData[0]);
		std::string name((char*)&nameData[0], nameData.size() - 1);
		//std::string name(nameData[0], nameData.size() - 1);
		std::cout << name << endl;
	}
	*/
}