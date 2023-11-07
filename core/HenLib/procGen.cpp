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
	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		//todo: These calculations are most likely wrong
		int numberOfVerts = (numSegments + 1) * 3;
		int numberOfIndices = (2 * numSegments) + ((numSegments * 3) * 4);
		mesh.vertices.reserve(numberOfVerts);
		mesh.indices.reserve(numberOfIndices);
		
		//Top & Bottom Cap Data
		float topY = height / 2.0;
		float bottomY = -topY;
		float thetaStep = ew::TAU / float(numSegments);

		//Top Vertex
		ew::Vertex topVertex;
		topVertex.pos = ew::Vec3(0.0f, topY, 0.0f);
		topVertex.uv = ew::Vec2(0.5f, 0.5f);
		topVertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
		mesh.vertices.push_back(topVertex);

		//Top Cap Ring Vertices
		for (int i = 0; i <= numSegments; i++) {
			ew::Vertex vertex;
			float theta = i * thetaStep;
			//Position
			vertex.pos.x = cos(theta) * radius;
			vertex.pos.y = topY;
			vertex.pos.z = sin(theta) * radius;
			//UVs
			vertex.uv = ew::Vec2(vertex.pos.x * 0.5f + 0.5f, vertex.pos.z * 0.5f + 0.5f);
			//Normals
			vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
			mesh.vertices.push_back(vertex);
		}

		//Top Side Ring Vertices
		for (int i = 0; i <= numSegments; i++) {
			ew::Vertex vertex;
			float theta = i * thetaStep;
			//Position
			vertex.pos.x = cos(theta) * radius;
			vertex.pos.y = topY;
			vertex.pos.z = sin(theta) * radius;
			//Normals
			vertex.normal = ew::Normalize(vertex.pos - topVertex.pos);
			mesh.vertices.push_back(vertex);
		}
		//Top Side Ring Index
		int topSideIndex = mesh.vertices.size();

		//Bottom Side Ring Vertices
		for (int i = 0; i <= numSegments; i++) {
			ew::Vertex vertex;
			float theta = i * thetaStep;
			//Position
			vertex.pos.x = cos(theta) * radius;
			vertex.pos.y = bottomY;
			vertex.pos.z = sin(theta) * radius;
			//Normals
			vertex.normal = ew::Normalize(vertex.pos - ew::Vec3(0.0f, bottomY, 0.0f));
			mesh.vertices.push_back(vertex);
		}

		//Bottom Cap Ring Vertices
		for (int i = 0; i <= numSegments; i++) {
			ew::Vertex vertex;
			float theta = i * thetaStep;
			//Position
			vertex.pos.x = cos(theta) * radius;
			vertex.pos.y = bottomY;
			vertex.pos.z = sin(theta) * radius;
			//UVs
			vertex.uv = ew::Vec2(vertex.pos.x * 0.5f + 0.5f, vertex.pos.z * 0.5f + 0.5f);
			//Normals
			vertex.normal = ew::Vec3(0.0f, -1.0f, 0.0f);
			mesh.vertices.push_back(vertex);
		}

		//Bottom Vertex
		ew::Vertex bottomVertex;
		bottomVertex.pos = ew::Vec3(0.0f, bottomY, 0.0f);
		topVertex.uv = ew::Vec2(0.5f, 0.5f);
		bottomVertex.normal = ew::Vec3(0.0f, -1.0f, 0.0f);
		mesh.vertices.push_back(bottomVertex);

		//Top Cap Indices
		unsigned int topCenter = 0;
		unsigned int topStart = topCenter + 1;

		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(topStart + i);
			mesh.indices.push_back(topCenter);
			mesh.indices.push_back(topStart + i + 1);
		}

		//todo: This it not working properly

		//Side Indices
		/*unsigned int sideStart = topSideIndex;
		unsigned int columns = numSegments + 1;
		for (int i = 0; i < columns; i++) {
			unsigned int startVertex = sideStart + 1;
			// Triangle 1
			mesh.indices.push_back(startVertex);
			mesh.indices.push_back(startVertex + 1);
			mesh.indices.push_back(startVertex + columns + 1);
			// Triangle 2
			mesh.indices.push_back(startVertex + columns + 1);
			mesh.indices.push_back(startVertex + columns);
			mesh.indices.push_back(startVertex);
		}*/

		//Bottom Cap Indices
		unsigned int bottomCenter = mesh.indices.size() + 1;
		unsigned int bottomStart = bottomCenter + 1;
		
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(bottomStart + i);
			mesh.indices.push_back(bottomCenter);
			mesh.indices.push_back(bottomStart + i + 1);
		}


		return mesh;
	}

	/// Creates a Sphere
	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;
		int numberOfVerts = numSegments * 4;
		int numberOfIndices = (2 * numSegments) * 4;
		mesh.vertices.reserve(numberOfVerts);
		mesh.indices.reserve(numberOfIndices);

		float thetaStep = ew::TAU / float(numSegments);
		float phiStep = ew::PI / float(numSegments);

		//Vertices
		for (int row = 0; row <= numSegments; row++) {
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++) {
				float theta = col * thetaStep;
				ew::Vertex vertex;
				//Position
				vertex.pos.x = radius * sin(phi) * cos(theta);
				vertex.pos.y = radius * cos(phi);
				vertex.pos.z = radius * sin(theta) * sin(phi);
				//UVs
				//todo: This does not work properly
				float uvPhi = atan2(vertex.pos.y, vertex.pos.x);
				float uvTheta = asin(vertex.pos.z/radius);
				vertex.uv.x = (uvPhi + ew::PI) / ew::TAU;
				vertex.uv.y = (uvTheta + ew::PI) / ew::PI;
				ew::Normalize(vertex.uv);
				//Normals
				vertex.normal = ew::Normalize(vertex.pos);
				mesh.vertices.push_back(vertex);
			}
		}

		//Indices
		unsigned int poleStart = 0;
		unsigned int sideStart = numSegments + 1;
		//Top Cap
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(sideStart + i);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i + 1);
		}
		//Rest of the Sphere
		int columns = numSegments + 1;
		for (int row = 0; row < numSegments-1; row++) {
			for (int col = 0; col < numSegments; col++) {
				unsigned int startVertex = row * columns + col;
				mesh.indices.push_back(startVertex);
				mesh.indices.push_back(startVertex + 1);
				mesh.indices.push_back(startVertex + columns + 1);
				// Triangle 2
				mesh.indices.push_back(startVertex + columns + 1);
				mesh.indices.push_back(startVertex + columns);
				mesh.indices.push_back(startVertex);
			}
		}
		//Bottom Cap
		//todo: Still not working properly
		unsigned int bottomPoleStart = mesh.vertices.size();
		unsigned int bottomSideStart = bottomPoleStart - 1;
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(bottomSideStart + i);
			mesh.indices.push_back(bottomPoleStart + i);
			mesh.indices.push_back(bottomSideStart + i + 1);
		}

		return mesh;
	}
}