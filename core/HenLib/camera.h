#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/mat4.h"
#include "transformation.h"

namespace HenLib {
	struct Camera {
		ew::Vec3 position;			//Camera Body Position
		ew::Vec3 target;			//Position to Look At
		float fov;					//Vertical Field of View in degrees
		float aspectRatio;			//Screen Width / Screen Height
		float nearPlane;			//Near Plane Distance (+Z)
		float farPlane;				//Far Plane Distance (+Z)
		bool orthographic;			//Perspective or Orthographic?
		float orthoSize;			//Height of Orthographic Frustum
		ew::Mat4 ViewMatrix() const	//World->View
		{
			return HenLib::LookAt(position, target, ew::Vec3(0, 1, 0));
		}
		ew::Mat4 ProjectionMatrix() const	//View->Clip
		{
			if(orthographic) {
				return HenLib::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else {
				return HenLib::Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
		} 
	};
}