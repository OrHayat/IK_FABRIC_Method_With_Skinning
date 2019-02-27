#include "obj_loader.h"

static inline uint FindNextChar(uint start, const char* str, uint length, char token) {
	uint result = start;

	while (result < length) {
		result++;
		if (str[result] == token)
			break;
	}

	return result;
}
OBJIndex OBJfactory(int p, int t, int n)
{
	OBJIndex result;
	result.vertexIndex = p - 1;
	result.uvIndex = 0;
	//result.normalIndex = 0;
	//result.faceIndex =-1;
	//result.edgeIndex = -1;
	//result.isEdgeUpdated = true;
	if (n >= 0)
		//	result.normalIndex = n - 1;
		if (t >= 0)
			result.uvIndex = t - 1;
	return result;
}

static inline uint ParseOBJIndexValue(const std::string& token, uint start, uint end) {
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, uint start, uint end) {
	return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim) {
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	uint strLength = s.length();
	uint start = 0;
	uint end = 0;

	while (end <= strLength) {
		while (end <= strLength) {
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}

glm::vec2 OBJModel::ParseOBJVec2(const std::string& line) {
	uint tokenLength = line.length();
	const char* tokenString = line.c_str();

	uint vertIndexStart = 3;

	while (vertIndexStart < tokenLength) {
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	uint vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec2(x, y);
}

glm::vec3 OBJModel::ParseOBJVec3(const std::string& line) {
	uint tokenLength = line.length();
	const char* tokenString = line.c_str();

	uint vertIndexStart = 2;

	while (vertIndexStart < tokenLength) {
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	uint vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x, y, z);
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token) {
	uint tokenLength = token.length();
	const char* tokenString = token.c_str();

	uint vertIndexStart = 0;
	uint vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	OBJIndex result;
	result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	result.uvIndex = 0;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	hasUVs = true;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	hasNormals = true;

	return result;
}

void OBJModel::CreateOBJFace(const std::string& line, std::set<Triangle>& trianglesSet) {
	std::vector<std::string> tokens = SplitString(line, ' ');

	/*
	trianglesSet.emplace(ParseOBJIndex(tokens[1]),
	ParseOBJIndex(tokens[2]),
	ParseOBJIndex(tokens[3]));

	if(tokens.size() > 4U) { //triangulation
	trianglesSet.emplace(ParseOBJIndex(tokens[1]),
	ParseOBJIndex(tokens[3]),
	ParseOBJIndex(tokens[4]));
	}
	*/
	const char* str = line.c_str();
	int v1, t1, n1;
	int v2, t2, n2;
	int v3, t3, n3;
	int v4, t4, n4;
	if (sscanf_s(str, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1,
		&n1, &v2, &t2, &n2, &v3, &t3, &n3, &v4, &t4, &n4) == 12)
	{
		OBJIndex o1 = OBJfactory(v1, t1, n1);
		OBJIndex o2 = OBJfactory(v2, t2, n2);
		OBJIndex o3 = OBJfactory(v3, t3, n3);
		OBJIndex o4 = OBJfactory(v4, t4, n4);
		trianglesSet.emplace(o1, o2, o3);
		trianglesSet.emplace(o1, o3, o4);
		/*OBJIndices.push_back(o1);
		OBJIndices.push_back(o2);
		OBJIndices.push_back(o3);
		OBJIndices.push_back(o1);
		OBJIndices.push_back(o3);
		OBJIndices.push_back(o4);*/
		return;
	}
	if (sscanf_s(str, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2,
		&t2, &n2, &v3, &t3, &n3) == 9)
	{
		OBJIndex o1 = OBJfactory(v1, t1, n1);
		OBJIndex o2 = OBJfactory(v2, t2, n2);
		OBJIndex o3 = OBJfactory(v3, t3, n3);
		trianglesSet.emplace(o1, o2, o3);
		return;
	}
	if (sscanf_s(str, "f %d//%d %d//%d %d//%d %d//%d", &v1, &n1, &v2,
		&n2, &v3, &n3, &v4, &n4) == 8)
	{
		OBJIndex o1 = OBJfactory(v1, -1, n1);
		OBJIndex o2 = OBJfactory(v2, -1, n2);
		OBJIndex o3 = OBJfactory(v3, -1, n3);
		OBJIndex o4 = OBJfactory(v4, -1, n4);
		trianglesSet.emplace(o1, o2, o3);
		trianglesSet.emplace(o1, o3, o4);
		return;
	}
	if (sscanf_s(str, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3) == 6)
	{
		OBJIndex o1 = OBJfactory(v1, -1, n1);
		OBJIndex o2 = OBJfactory(v2, -1, n2);
		OBJIndex o3 = OBJfactory(v3, -1, n3);
		trianglesSet.emplace(o1, o2, o3);
		return;
	}
	if (sscanf_s(str, "f %d/%d %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2,
		&v3, &t3, &v4, &t4) == 8)
	{
		OBJIndex o1 = OBJfactory(v1, t1, -1);
		OBJIndex o2 = OBJfactory(v2, t2, -1);
		OBJIndex o3 = OBJfactory(v3, t3, -1);
		OBJIndex o4 = OBJfactory(v4, t4, -1);
		trianglesSet.emplace(o1, o2, o3);
		trianglesSet.emplace(o1, o3, o4);
		return;
	}
	if (sscanf_s(str, "f %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2, &v3, &t3) == 6)
	{
		OBJIndex o1 = OBJfactory(v1, t1, -1);
		OBJIndex o2 = OBJfactory(v2, t2, -1);
		OBJIndex o3 = OBJfactory(v3, t3, -1);
		trianglesSet.emplace(o1, o2, o3);
		return;
	}
	if (sscanf_s(str, "f %d %d %d %d ", &v1, &v2, &v3, &v4) == 4)
	{
		OBJIndex o1 = OBJfactory(v1, -1, -1);
		OBJIndex o2 = OBJfactory(v2, -1, -1);
		OBJIndex o3 = OBJfactory(v3, -1, -1);
		OBJIndex o4 = OBJfactory(v4, -1, -1);
		trianglesSet.emplace(o1, o2, o3);
		trianglesSet.emplace(o1, o3, o4);
		return;
	}
	if (sscanf_s(str, "f %d %d %d", &v1, &v2, &v3) == 3)
	{
		OBJIndex o1 = OBJfactory(v1, -1, -1);
		OBJIndex o2 = OBJfactory(v2, -1, -1);
		OBJIndex o3 = OBJfactory(v3, -1, -1);
		trianglesSet.emplace(o1, o2, o3);
		return;
	}






}