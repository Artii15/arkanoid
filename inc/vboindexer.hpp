#ifndef VBOINDEXER_HPP
#define VBOINDEXER_HPP

void indexVBO(
	std::vector<glm::vec4> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec4> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec4> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec4> & out_normals
);

#endif
