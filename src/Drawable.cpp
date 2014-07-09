#include "../inc/Drawable.h"

Drawable::Drawable(){
	this->vertices = NULL;
	this->uvs = NULL;
	this->normals = NULL;
	this->indices = NULL;
	this->shaders_loaded = false;

	this->shader_program = 0;
	this->model_matrix = glm::mat4(1.0f);
	for(short i=0; i<4; i++){
		this->coordinates_2D[0] = glm::vec3(0,0,1);
	}
	
	// Tworzenie VAO
	glGenVertexArrays(1, &(this->vao) );
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
	this->deleteTextures();
}

void Drawable::deleteTextures(){
	if(this->textures.diffuse != 0){
		glDeleteTextures(1, &(this->textures.diffuse));
		this->textures.diffuse = 0;
	}
	if(this->textures.ambient != 0){
		glDeleteTextures(1, &(this->textures.ambient));
		this->textures.ambient = 0;
	}
	if(this->textures.specular != 0){
		glDeleteTextures(1, &(this->textures.specular));
		this->textures.specular = 0;
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

Drawable& Drawable::recalculateCoordinates2D(){
	float maxX = 0.0f;
	float minX = 0.0f;
	float maxY = 0.0f;
	float minY = 0.0f;
	
	for(unsigned int i=0; i<this->vertices->size(); i++){
		if(this->vertices->at(i).operator[](0) > maxX){
			maxX = this->vertices->at(i).operator[](0);
		}
		if(this->vertices->at(i).operator[](0) < minX){
			minX = this->vertices->at(i).operator[](0);
		}
		if(this->vertices->at(i).operator[](1) > maxY){
			maxY = this->vertices->at(i).operator[](1);
		}
		if(this->vertices->at(i).operator[](1) < minY){
			minY = this->vertices->at(i).operator[](1);
		}
	}
	this->coordinates_2D[0] = glm::vec3(minX, maxY, 1);
	this->coordinates_2D[1] = glm::vec3(maxX, maxY, 1);
	this->coordinates_2D[2] = glm::vec3(maxX, minY, 1);
	this->coordinates_2D[3] = glm::vec3(minX, minY, 1);
	
	return *(this);
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
	
	this->assignVBOtoAttribute("normal", n_buf, 4);
	this->assignVBOtoAttribute("vertex", v_buf, 4);
	this->assignVBOtoAttribute("texCoord", uv_buf, 2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buf);
	
	glBindVertexArray(0);
	
	this->recalculateCoordinates2D();
	
	return *(this);
}

const std::vector< unsigned short >* Drawable::getIndices(){
	return this->indices;
}

Drawable& Drawable::draw(const glm::mat4& v, const glm::mat4& p, const std::vector<struct light*>& lights){
	// Żeby uprościć obliczenia vertex shadera
	glm::mat4 g = glm::mat4(glm::transpose(glm::inverse(glm::mat3(v*this->model_matrix))));
	
	glUseProgram(this->shader_program);
	
	glm::mat4 MVP = p * v * this->model_matrix;
	
	// Uniformy
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "V"), 1, GL_FALSE, &v[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "P"), 1, GL_FALSE, &p[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "G"), 1, GL_FALSE, &g[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "M"), 1, GL_FALSE, &(this->model_matrix[0][0]));
	this->setTextureUniforms().setLightUniforms(lights);
	
	this->activateTextures(); // Przypisywanie tekstur do jednostek teksturujących
	
	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powiązań slotów atrybutów z tablicami z danymi
	glBindVertexArray(this->vao);
	//Narysowanie obiektu
	glDrawElements(GL_TRIANGLES, this->indices->size() ,GL_UNSIGNED_SHORT, NULL); 
	glBindVertexArray(0);	
	
	return *(this);
}

Drawable& Drawable::setLightUniforms(const std::vector<struct light*>& lights){
	for(unsigned int i=0; i<lights.size(); i++){
		std::string beginning = "lights[";
		std::ostringstream ss;
		ss << i;
		beginning += ss.str() + "].";
		// Z powyższych funkcji i poniższych wywołań funkcji tworzymy stringi typu "lights[i].position"
		glUniform4fv(glGetUniformLocation(this->shader_program, (beginning + "ambient").c_str()), 1, &(lights[i]->ambient[0]));
		glUniform4fv(glGetUniformLocation(this->shader_program, (beginning + "diffuse").c_str()), 1, &(lights[i]->diffuse[0]));
		glUniform4fv(glGetUniformLocation(this->shader_program, (beginning + "specular").c_str()), 1, &(lights[i]->specular[0]));
		glUniform4fv(glGetUniformLocation(this->shader_program, (beginning + "position").c_str()), 1, &(lights[i]->position[0]));
		glUniform1i(glGetUniformLocation(this->shader_program, (beginning + "shininess").c_str()), lights[i]->shininess);
		glUniform1f(glGetUniformLocation(this->shader_program, (beginning + "k").c_str()), lights[i]->k);
	}
	return *(this);
}

Drawable& Drawable::setTextureUniforms(){
	glUniform1i(glGetUniformLocation(this->shader_program, "diffuseSamp"), this->samplers.diffuse);
	glUniform1i(glGetUniformLocation(this->shader_program, "ambientSamp"), this->samplers.ambient);	
	glUniform1i(glGetUniformLocation(this->shader_program, "specularSamp"), this->samplers.specular);

	return *(this);
}

Drawable& Drawable::activateTextures(){
	if(!this->textures.diffuse || !this->textures.ambient || !this->textures.specular){
		throw Exception("Nie załadowano wymaganych tekstur");
	}
	// Tekstura dla materiału do światła rozpraszanego
	glActiveTexture(this->samplers.diffuse);
	glBindTexture(GL_TEXTURE_2D,this->textures.diffuse);
	// Tekstura dla materiału do światła odbitego
	glActiveTexture(this->samplers.specular);
	glBindTexture(GL_TEXTURE_2D,this->textures.specular);
	// Tekstura dla materiału do światła otoczenia
	glActiveTexture(this->samplers.ambient);
	glBindTexture(GL_TEXTURE_2D,this->textures.ambient);
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

GLuint Drawable::loadTexture(const char* filename, GLuint sampler_nr){
	GLuint tex;
	TGAImg img;
	glActiveTexture(sampler_nr);
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
	
	return tex;
}

Drawable& Drawable::setAmbientTexture(const char* filename){
	if(this->textures.ambient != 0){
		glDeleteTextures(1, &(this->textures.ambient));
	}
	this->textures.ambient = loadTexture(filename, this->samplers.ambient);
	return *(this);
}

Drawable& Drawable::setSpecularTexture(const char* filename){
	if(this->textures.specular != 0){
		glDeleteTextures(1, &(this->textures.specular));
	}
	this->textures.specular = loadTexture(filename, this->samplers.specular);
	return *(this);
}

Drawable& Drawable::setDiffuseTexture(const char* filename){
	if(this->textures.diffuse != 0){
		glDeleteTextures(1, &(this->textures.diffuse));
	}
	this->textures.diffuse = loadTexture(filename, this->samplers.diffuse);
	return *(this);
}

Drawable& Drawable::setAmbientTexture(GLuint tex, GLuint sampler_nr){
	if(this->textures.ambient != 0){
		glDeleteTextures(1, &(this->textures.ambient));
	}
	this->textures.ambient = tex;
	this->samplers.ambient = sampler_nr;
	return *(this);
}

Drawable& Drawable::setSpecularTexture(GLuint tex, GLuint sampler_nr){
	if(this->textures.specular != 0){
		glDeleteTextures(1, &(this->textures.specular));
	}
	this->textures.specular = tex;
	this->samplers.specular = sampler_nr;
	return *(this);
}

Drawable& Drawable::setDiffuseTexture(GLuint tex, GLuint sampler_nr){
	if(this->textures.diffuse != 0){
		glDeleteTextures(1, &(this->textures.diffuse));
	}
	this->textures.diffuse = tex;
	this->samplers.diffuse = sampler_nr;
	return *(this);
}

const struct textures& Drawable::getTextures(){
	return this->textures;
}
const struct materials_samplers& Drawable::getSamplers(){
	return this->samplers;
}

const glm::mat4& Drawable::getModelMatrix(){
	return this->model_matrix;	
}

Drawable& Drawable::setModelMatrix(const glm::mat4 &m){
	this->model_matrix = m;
	
	return *(this);
}
