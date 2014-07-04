#include "../inc/Drawable.h"

Drawable::Drawable(){
	this->vertices = NULL;
	this->uvs = NULL;
	this->normals = NULL;
	this->indices = NULL;
	this->shaders_loaded = false;

	this->shader_program = 0;
	this->model_matrix = glm::mat4(1.0f);
	
	// Tworzenie VAO
	glGenVertexArrays(1, &(this->vao) );
	this->time = glutGet(GLUT_ELAPSED_TIME);
	this->texture = 0;
}

Drawable::~Drawable(){
	this->deleteVertices();	
	this->deleteUVs();
	this->deleteNormals();
	this->deleteIndices();
	if( this->shader_program != 0 ){
		glDeleteProgram( this->shader_program );
	}
	
	// Usuwanie VBO
	for (unsigned int i=0; i<VBOs.size(); i++){
		glDeleteBuffers(1,&(VBOs[i]));
	}
	
	glDeleteVertexArrays( 1, &(this->vao) );
	//Wykasuj program shaderów
	glDeleteProgram(this->shader_program);
	// Zwalnianie pamięci po teksturach
	if( this->texture != 0 ){
		glDeleteTextures(1, &(this->texture));
	}
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
	
	if( this->shaders_loaded ){
		printf("Shadery już zostały załadowane\n");
		return *(this);
	}
	
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
	this->shaders_loaded = true;
	
	return *(this);
}

GLuint Drawable::getShaderProgram(){
	return this->shader_program;
}

Drawable& Drawable::loadObj(const char *path){
	
	if( !this->shaders_loaded ){
		throw new Exception("Próba ładowania obiektu, bez załadowanych shaderów");
	}
	
	std::vector< glm::vec4 > tmp_vertices;
	std::vector< glm::vec2 > tmp_uvs;
	std::vector< glm::vec4 > tmp_normals;
	
	if( !loadOBJ(path, tmp_vertices, tmp_uvs, tmp_normals ) ){
		throw new Exception("Nie udało się otworzyć pliku modelu");
	}
	
	this->deleteVertices();
	this->deleteUVs();
	this->deleteNormals();
	this->deleteIndices();
	
	this->vertices = new std::vector< glm::vec4 >();
	this->normals = new std::vector< glm::vec4 >();
	this->uvs = new std::vector< glm::vec2 >();
	this->indices = new std::vector< unsigned short >();
	
	indexVBO(tmp_vertices, tmp_uvs, tmp_normals, *(this->indices), *(this->vertices), *(this->uvs), *(this->normals) );
	
	GLuint v_buf = this->makeBuffer(&(this->vertices->at(0)), this->vertices->size(), sizeof(glm::vec4));
	GLuint uv_buf = this->makeBuffer(&(this->uvs->at(0)), this->uvs->size(), sizeof(glm::vec2));
	GLuint n_buf = this->makeBuffer(&(this->normals->at(0)), this->normals->size(), sizeof(glm::vec4));
	GLuint i_buf = this->makeElementBuffer(&(this->indices->at(0)), this->indices->size(), sizeof(unsigned short));
	
	glBindVertexArray(this->vao);
	
	this->assignVBOtoAttribute("vertex", v_buf, 4);
	this->assignVBOtoAttribute("uvs", uv_buf, 2);
	this->assignVBOtoAttribute("normals", n_buf, 4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buf);
	
	glBindVertexArray(0);
	
	return *(this);
}

const std::vector< unsigned short >* Drawable::getIndices(){
	return this->indices;
}

Drawable& Drawable::draw(const glm::mat4& v, const glm::mat4& p){
	double cur_time = glutGet(GLUT_ELAPSED_TIME);
	float dt = (cur_time - this->time)/1000.f;
	this->time = cur_time;
	
	this->model_matrix = glm::rotate(this->model_matrix, 3.0f*dt, glm::vec3(0.5f, 1.0f, 0.5f));
	
	glUseProgram(this->shader_program);
	
	glm::mat4 MVP = p * v * this->model_matrix;
	
	// Uniformy
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform1i( glGetUniformLocation(this->shader_program, "textureMap0"), 0 );
	
	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powišzań slotów atrybutów z tablicami z danymi
	glBindVertexArray(this->vao);
	
	if(this->texture){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,this->texture);
	}
	
	
	//Narysowanie obiektu
	glDrawElements(GL_TRIANGLES, this->indices->size() ,GL_UNSIGNED_SHORT, NULL); 
	//Posprzštanie po sobie (niekonieczne w sumie jeżeli korzystamy z VAO dla każdego rysowanego obiektu)
	glBindVertexArray(0);	
	
	return *(this);
}

GLuint Drawable::makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;
	
	glGenBuffers(1,&handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który będzie zawierał tablicę danych
	glBindBuffer(GL_ARRAY_BUFFER,handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicę do VBO
	this->VBOs.push_back(handle);
	
	return handle;
}

GLuint Drawable::makeElementBuffer(void *data, int vertexCount, int vertexSize){
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * vertexSize, data , GL_STATIC_DRAW);
	this->VBOs.push_back(elementbuffer);
	
	return elementbuffer;
}

Drawable& Drawable::assignVBOtoAttribute(const char* attributeName, GLuint bufVBO, int variableSize){
	GLuint location = glGetAttribLocation(this->shader_program, attributeName); //Pobierz numery slotów dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER,bufVBO);  //Uaktywnij uchwyt VBO 
	glEnableVertexAttribArray(location); //Włącz używanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location,variableSize,GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location mają być brane z aktywnego VBO
	
	return *(this);
}

Drawable& Drawable::loadTexture(const char* filename){
	GLuint tex;
	TGAImg img;
	glActiveTexture(GL_TEXTURE0);
	if (img.Load(filename)==IMG_OK) {
		glGenTextures(1,&tex); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D,tex); //Przetwarzaj uchwyt tex
	if (img.GetBPP()==24) //Obrazek 24bit
	 	glTexImage2D(GL_TEXTURE_2D,0,3,img.GetWidth(),img.GetHeight(),0,
	  	GL_RGB,GL_UNSIGNED_BYTE,img.GetImg());
	else if (img.GetBPP()==32) //Obrazek 32bit
	 	glTexImage2D(GL_TEXTURE_2D,0,4,img.GetWidth(),img.GetHeight(),0,
	  	GL_RGBA,GL_UNSIGNED_BYTE,img.GetImg());     
	else {
	  	printf("Nieobsługiwany format obrazka w pliku: %s \n",filename);
	}
	} else {
		printf("Błąd przy wczytywaniu pliku: %s\n",filename);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	this->texture = tex;
	
	return *(this);
}
