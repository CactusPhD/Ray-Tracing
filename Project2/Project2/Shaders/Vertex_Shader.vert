#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 rays;
uniform mat4 view;
uniform float xRot;
uniform float yRot;
out vec3 quadPosition;
out vec3 rayDirection;

mat4 rotationMatrix(vec3 axis, float angle){
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
  
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main(){
	gl_Position = vec4(position, 1.0);
	mat4 xMat = rotationMatrix(vec3(1.0, 0.0, 0.0), xRot);
	mat4 yMat = rotationMatrix(vec3(0.0, 1.0, 0.0), yRot);
	rayDirection = (yMat * xMat * vec4(rays, 0.0)).xyz;

}