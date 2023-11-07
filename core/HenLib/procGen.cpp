#include "procGen.h"
#include <stdlib.h>

namespace HenLib {
	static void createPlaneIndices(int subdivision, float width, float height, ew::MeshData* mesh) {
		
	}

	/// Creates a Plane of uniform size
	ew::MeshData createPlane(int subdivision, float width, float height) {
		ew::MeshData mesh;
		int numberOfVerts = (subdivision + 1) * (subdivision + 1);
		int numberOfIndices = 6 * (subdivision) * (subdivision);
		mesh.vertices.reserve(numberOfVerts);
		mesh.indices.reserve(numberOfIndices);

		//Vertices
		for (int row = 0; row <= subdivision; row++) {
			for (int col = 0; col <= subdivision; col++) {
				ew::Vertex vertex;
				//Position
				vertex.pos.x = float(width) * (float(col) / float(subdivision));
				vertex.pos.y = 0.0f;
				vertex.pos.z = float(-height) * (float(row) / float(subdivision));
				//UVs
				vertex.uv = ew::Vec2(col, row)/float(subdivision);
				//Normals
				vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
				mesh.vertices.push_back(vertex);
			}
		}
		//Indices
		int columns = subdivision + 1;
		for (int row = 0; row < subdivision; row++) {
			for (int col = 0; col < subdivision; col++) {
				unsigned int startVertex = row * columns +col;
				// Bottom Right Triangle
				mesh.indices.push_back(startVertex);
				mesh.indices.push_back(startVertex + 1);
				mesh.indices.push_back(startVertex + columns +1);
				// Top Left Triangle
				mesh.indices.push_back(startVertex + columns + 1);
				mesh.indices.push_back(startVertex + columns);
				mesh.indices.push_back(startVertex);
			}
		}

		return mesh;
	}

	/// Creates a Cylinder
	MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		int numberOfVerts = (numSegments + 1) * 3;
		int numberOfIndices = (2 * numSegments) + ((numSegments * 3) * 2);
		mesh.vertices.reserve(numberOfVerts);
		mesh.indices.reserve(numberOfIndices);
		
		//Top & Bottom Cap Data
		float topY = height / 2.0;
		float bottomY = -topY;
		float thetaStep = ew::TAU / float(numSegments);

		//Top Vertex
		ew::Vertex topVertex;
		topVertex.pos = ew::Vec3(0.0f, topY, 0.0f);
		mesh.vertices.push_back(topVertex);
		//Top Ring Vertices
		for (int i = 0; i <= numSegments; i++) {
			ew::Vertex vertex;
			float theta = i * thetaStep;
			vertex.pos.x = cos(theta) * radius;
			vertex.pos.y = topY;
			vertex.pos.z = sin(theta) * radius;
			mesh.vertices.push_back(vertex);
		}

		//Bottom Vertex
		ew::Vertex bottomVertex;
		bottomVertex.pos = ew::Vec3(0.0f, bottomY, 0.0f);
		mesh.vertices.push_back(bottomVertex);

		//Bottom Ring Vertices
		for (int i = 0; i <= numSegments; i++) {
			ew::Vertex vertex;
			float theta = i * thetaStep;
			vertex.pos.x = cos(theta) * radius;
			vertex.pos.y = bottomY;
			vertex.pos.z = sin(theta) * radius;
			mesh.vertices.push_back(vertex);
		}
		
		


		return mesh;
	}

	/// Creates a Sphere
	/*MeshData createSphere(float radius, int numSegments) {

	}*/
}