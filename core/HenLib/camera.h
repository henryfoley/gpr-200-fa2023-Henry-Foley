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

	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from previous frame
		float yaw = 0, pitch = 0; //Degrees
		float mouseSensitivity = 0.1f; //How fast to turn with mouse
		bool firstMouse = true; //Flag to store initial mouse position
		float moveSpeed = 5.0f; //How fast to move with arrow keys (M/S)
	};
}