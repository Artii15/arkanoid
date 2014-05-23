#include "../inc/Drawable.h"

Drawable::Drawable(){
	this->vertices = NULL;
	this->texture_vertices = NULL;
	this->vertex_normals = NULL;
	this->vertex_indices = NULL;
	this->texture_indices = NULL;
	this->normal_indices = NULL;
	this->shader_program = 0;
}

Drawable::~Drawable(){
	this->deleteVertices();	
	this->deleteTextureVertices();
	this->deleteVertexNormals();
	this->deleteVertexIndices();
	this->deleteNormalIndices();
	this->deleteTextureIndices();
	if( this->shader_program != 0 ){
		glDeleteProgram( this->shader_program );
	}
}

void Drawable::deleteVertexIndices(){
	if( this->vertex_indices != NULL ){
		delete this->vertex_indices;
	}
}

void Drawable::deleteTextureIndices(){
	if( this->texture_indices != NULL ){
		delete this->texture_indices;
	}
}

void Drawable::deleteNormalIndices(){
	if( this->normal_indices != NULL ){
		delete this->normal_indices;
	}
}

void Drawable::deleteVertices(){
	if( this->vertices != NULL ){
		for(unsigned int i=0; i < this->vertices->size(); i++){
			if( this->vertices->at(i) != NULL ){
				delete this->vertices->at(i);
			}
		}
		delete this->vertices;
	}
}

void Drawable::deleteTextureVertices(){
	if( this->texture_vertices != NULL ){
		for(unsigned int i=0; i < this->texture_vertices->size(); i++){
			if( this->texture_vertices->at(i) != NULL ){
				delete this->texture_vertices->at(i);
			}
		}
		delete this->texture_vertices;
	}
}

void Drawable::deleteVertexNormals(){
	if( this->vertex_normals != NULL ){
		for(unsigned int i=0; i < this->vertex_normals->size(); i++){
			if( this->vertex_normals->at(i) != NULL ){
				delete this->vertex_normals->at(i);
			}
		}
		delete this->vertex_normals;
	}
}

const std::vector< glm::vec4* >* Drawable::getVertices(){
	return this->vertices;	
}

Drawable& Drawable::loadObj(const char *path){
	std::ifstream file(path, std::ios::in);
	
	if( !file.is_open() ){
		throw Exception("Nie udało się otworzyć pliku modelu");
	}
	
	this->deleteVertices();
	this->deleteTextureVertices();
	this->deleteVertexNormals();
	this->deleteVertexIndices();
	this->deleteTextureIndices();
	this->deleteNormalIndices();
	
	this->vertices = new std::vector< glm::vec4* >();
	this->texture_vertices = new std::vector< glm::vec2* >();
	this->vertex_normals = new std::vector< glm::vec4* >();
	this->vertex_indices = new std::vector< unsigned int >();
	this->texture_indices = new std::vector< unsigned int >();
	this->normal_indices = new std::vector< unsigned int >();

	while( file.good() ){
		std::string buf = "";
		getline(file, buf);
		std::string prefix = buf.substr(0, 2);
		
		if( prefix == "vn" ){
			this->vertex_normals->push_back( this->strToVec4( buf ) );
			continue;
		}
		if( prefix == "vt" ){
			this->texture_vertices->push_back( this->strToVec2( buf ) );
			continue;
		}
		if( prefix == "v " ){
			this->vertices->push_back( this->strToVec4( buf ) );
			continue;
		}
		if( prefix == "f " ){
			this->fillIndices(buf, this->vertex_indices, this->texture_indices, this->normal_indices); 
			continue;
		}
	}
	
	file.close();
	return *(this);
}

glm::vec4* Drawable::strToVec4( std::string line ){
	float tmp[3];
	
	short count = 0;
	bool reading = false;
	std::string buf = "";
	line += "\n";
	for(unsigned int i=0; i<line.length(); i++){
		if( ((int)line[i] < 48 || (int)line[i] > 57) && line[i] != '.' && line[i] != '-' ){
			if(reading){
				reading = false;
				tmp[count++] = ::atof( buf.c_str() );
				buf = "";
				if(count > 2){
					break;
				}
			}
			continue;
		}
		reading = true;
		buf += line[i];
	}
	
	return new glm::vec4(tmp[0], tmp[1], tmp[2], 1.0f);
}

glm::vec2* Drawable::strToVec2( std::string line ){
	float tmp[2];
	
	short count = 0;
	bool reading = false;
	std::string buf = "";
	line += "\n";
	for(unsigned int i=0; i<line.length(); i++){
		if( ((int)line[i] < 48 || (int)line[i] > 57) && line[i] != '.' && line[i] != '-' ){
			if(reading){
				reading = false;
				tmp[count++] = ::atof( buf.c_str() );
				buf = "";
				if(count > 1){
					break;
				}
			}
			continue;
		}
		reading = true;
		buf += line[i];
	}
	
	return new glm::vec2(tmp[0], tmp[1]);
}

void Drawable::fillIndices( std::string line, std::vector< unsigned int > *vi, std::vector< unsigned int > *ti, std::vector< unsigned int > *ni ){
	unsigned int i=0;
	unsigned short nr = 0;
	std::string buf = "";
	line += "\n";
	while( i < line.length() ){
		if( ((int)line[i] < 49 || (int)line[i] > 57) ){
			if( line[i] == '/' ){
				if( nr == 0 ){
					vi->push_back( ::atoi( buf.c_str() ) - 1 );
					nr++;
				}
				else{
					ti->push_back( ::atoi( buf.c_str() ) - 1 );
					nr++;
				}
				buf = "";
			}
			else{
				if( nr == 2 ){
					ni->push_back( ::atoi( buf.c_str() ) - 1 );
					nr = 0;
					buf = "";
				}
			}
		}
		else{
			buf += line[i];
		}
		i++;
	}
}

Drawable& Drawable::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return *(this);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	
	this->shader_program = ProgramID;
	
	return *(this);
}
