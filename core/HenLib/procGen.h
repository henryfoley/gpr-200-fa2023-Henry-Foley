#pragma once

#include "../ew/mesh.h"

namespace HenLib {
	ew::MeshData createPlane(int subdivision, float width, float height);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createSphere(float radius, int numSegments);
}