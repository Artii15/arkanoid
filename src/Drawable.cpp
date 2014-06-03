#include "../inc/Drawable.h"

Drawable::Drawable(){
	this->vertices = NULL;
	this->uvs = NULL;
	this->normals = NULL;
	this->indices = NULL;

	this->shader_program = 0;
	this->model_matrix = new glm::mat4(1.0f);
}

Drawable::~Drawable(){
	this->deleteVertices();	
	this->deleteUVs();
	this->deleteNormals();
	this->deleteIndices();
	if( this->shader_program != 0 ){
		glDeleteProgram( this->shader_program );
	}
	delete this->model_matrix;
}

void Drawable::deleteIndices(){
	if( this->indices != NULL ){
		delete this->indices;
	}
}

void Drawable::deleteVertices(){
	if( this->vertices != NULL ){
		delete this->vertices;
	}
}

void Drawable::deleteUVs(){
	if( this->uvs != NULL ){
		delete this->uvs;
	}
}

void Drawable::deleteNormals(){
	if( this->normals != NULL ){
		delete this->normals;
	}
}

const std::vector< glm::vec4 >* Drawable::getVertices(){
	return this->vertices;	
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

GLuint Drawable::getShaderProgram(){
	return this->shader_program;
}

const glm::mat4* Drawable::getModelMatrix(){
	return this->model_matrix;
}

Drawable& Drawable::setModelMatrix( glm::mat4* matrix ){
	delete this->model_matrix;
	this->model_matrix = matrix;	
	
	return *(this);
}

Drawable& Drawable::loadObj(const char *path){
	
	std::vector< glm::vec4 > tmp_vertices;
	std::vector< glm::vec2 > tmp_uvs;
	std::vector< glm::vec4 > tmp_normals;
	
	if( !loadOBJ(path, tmp_vertices, tmp_uvs, tmp_normals ) ){
		throw new Exception("Nie udało się otworzyć pliku modelu");
	}
	
	this->deleteVertices();
	this->deleteUVs();
	this->deleteNormals();
	
	indexVBO(tmp_vertices, tmp_uvs, tmp_normals, *(this->indices), *(this->vertices), *(this->uvs), *(this->normals));
	
	return *(this);
}
