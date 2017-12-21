#version 430
//RHC

in vec3 rayDirection;
uniform vec3 eye;
out vec4 FragColor;

const int numSpheres = 3;
const int numPlanes = 1;
const vec3 bgColor = normalize(vec3(217, 170, 223));

struct Ray{
	vec3 direction;
	vec3 origin;
};

struct Plane{
	vec3 point;
	vec3 normal;
	vec3 color;
	int beta;
	int objID;
};

struct Sphere{
	vec3 center;
	float radius;
	vec3 color;
	int beta;
	int objID;
};

struct HitObj{
	float distance;
	vec3 color;
	vec3 litColor;
	vec3 newDir;
	vec3 newOrigin;
	vec3 normal;
	int beta;
	int objID;
};

struct DirectionalLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 attenuation;
};

DirectionalLight dirLight;
PointLight pointLight;
Sphere spheres[numSpheres];
Plane planes[numPlanes];


float planeCollision(in Plane plane, in Ray ray){
	vec3 normal = plane.normal;
	vec3 point = plane.point;
	vec3 origin = ray.origin;
	vec3 direction = ray.direction;
	float numerator = dot((point - origin), normal);
	float denominator = dot(direction, normal);
	float d;
	if(denominator != 0){
		d = numerator/denominator;
	}
	else{
		d = -1;
	}
	if(d < 0){
		d = -1;
	}
	return d;
}

float sphereCollision(in Sphere sphere, in Ray ray){
	vec3 center = sphere.center;
	float radius = sphere.radius;
	vec3 origin = ray.origin;
	vec3 direction = ray.direction;
	float discriminant = pow(dot(direction, (origin - center)),2) - pow(length(origin - center),2) + pow(radius, 2);
	float d1;
	float d2;
	if (discriminant >= 0){
		d1 = -1 * dot(direction, (origin - center)) - sqrt(discriminant);
		d2 = -1 * dot(direction, (origin - center)) + sqrt(discriminant);
		if (d1 < 0){
			return d2;
		}
		if (d2 < 0){
			return d1;
		}
		return min(d1,d2);
	}
	return -1;
}

float directionalShadow(in HitObj obj, in vec3 lightDirection){
	Ray ray;
	ray.direction = lightDirection;
	ray.origin = obj.newOrigin;
	//iterate spheres
	for(int i = 0; i < numSpheres; i++){
		float dist = sphereCollision(spheres[i], ray);
		if(dist >= 0 && obj.objID != spheres[i].objID){
			return 0;
		}
	}
	//iterate planes
	for(int i = 0; i < numPlanes; i++){
		float dist = planeCollision(planes[i], ray);
		if(dist >= 0 && obj.objID != planes[i].objID){
			return 0;
		}
	}
	return 1;
}

float pointShadow(in HitObj obj){
	Ray ray;
	ray.direction = normalize(-obj.newOrigin + pointLight.position);
	ray.origin = pointLight.position;
	//iterate spheres
	for(int i = 0; i < numSpheres; i++){
		float dist = sphereCollision(spheres[i], ray);
		if(dist >= 0 && obj.objID != spheres[i].objID){
			return 0;
		}
	}
	//iterate planes
	for(int i = 0; i < numPlanes; i++){
		float dist = planeCollision(planes[i], ray);
		if(dist >= 0 && obj.objID != planes[i].objID){
			return 0;
		}
	}
	return 1;
}

vec3 directionalLighting(in Ray ray, in HitObj obj){
	//directional light
	vec3 lightDir = dirLight.direction;
	vec3 diffuse = dirLight.diffuse * max(0, dot(obj.normal, lightDir));
	vec3 halfDir = normalize(lightDir + -ray.direction);
	float specAngle = max(0, dot(halfDir, obj.normal));
	float specVal = pow(specAngle, obj.beta);
	vec3 spec = dirLight.specular * specVal; 
	float shadow = directionalShadow(obj, lightDir);
	vec3 finalDirLight = vec3(dirLight.ambient + shadow *(spec + diffuse));
	
	//point light
	lightDir = pointLight.position - obj.newOrigin;
	float distance = length(lightDir);
	lightDir = normalize(lightDir);
	float attenuation = 1 / (pointLight.attenuation.x * pow(distance,2) + pointLight.attenuation.y * distance + pointLight.attenuation.z);
	diffuse = pointLight.diffuse * max(0 , dot(obj.normal, lightDir));
	halfDir = normalize(lightDir + -ray.direction);
	specAngle = max(0, dot(halfDir, obj.normal));
	specVal = pow(specAngle, obj.beta);
	spec = pointLight.specular * specVal;
	shadow = pointShadow(obj);
	vec3 finalPointLight = vec3((pointLight.ambient + attenuation * shadow *(spec + diffuse)));
	//return vec3((pointLight.ambient + (spec + diffuse)));
	//return shadow.xxx;
	//return obj.color * finalPointLight;
	return obj.color * (finalDirLight  +  finalPointLight);
}

vec3 rayCast(inout Ray ray){
	bool hit = false;
	HitObj minObj;
	minObj.distance = 1.0/0.0;
	minObj.color = bgColor;
	minObj.litColor = bgColor;
	minObj.newDir = vec3(0.0, 0.0, 0.0);
	minObj.newOrigin = vec3(0.0, 0.0, 0.0);
	minObj.normal = vec3(0.0, 0.0, 0.0);
	minObj.beta = 0;
	minObj.objID = 0;
	//iterate spheres
	for(int i = 0; i < numSpheres; i++){
		float dist = sphereCollision(spheres[i], ray);
		if(dist < minObj.distance && dist >= 0){
			minObj.distance = dist;
			minObj.color = spheres[i].color;
			minObj.normal = normalize((dist * ray.direction + ray.origin) - spheres[i].center);
			minObj.newOrigin = (dist * ray.direction + ray.origin) + minObj.normal * 0.01;
			minObj.newDir = normalize(reflect(ray.direction, minObj.normal));
			minObj.objID = spheres[i].objID;
			minObj.beta = spheres[i].beta;
			hit = true;
		}
	}
	//iterate planes
	 for(int i = 0; i < numPlanes; i++){
		float dist = planeCollision(planes[i], ray);
		if(dist < minObj.distance && dist >= 0){
			minObj.distance = dist;
			minObj.color = planes[i].color;
			minObj.normal = planes[i].normal;
			minObj.newOrigin = (dist * ray.direction + ray.origin) + minObj.normal * 0.01;
			minObj.newDir = normalize(reflect(ray.direction, minObj.normal));
			minObj.objID = planes[i].objID;
			minObj.beta = spheres[i].beta;
			hit = true;
		}
	}
	//find closest
	//return color and new ray
		//phong lighing :)
	if (hit){
		//return minObj.color;
		minObj.litColor = directionalLighting(ray, minObj);
		ray.origin = minObj.newOrigin;
		ray.direction = minObj.newDir;
		if(minObj.beta <= 1){
			ray.origin = vec3(0.0, -2.0, 0.0);
			ray.direction = vec3(0.0, -1.0, 0.0);
		}
		return minObj.litColor;
	}
	ray.origin = minObj.newOrigin;
	ray.direction = minObj.newDir;
	return bgColor;
}

void main(){
	FragColor = vec4(bgColor, 1.0);//fuchsia
	
	dirLight.ambient = vec3(0.1, 0.1, 0.1);
	dirLight.diffuse = vec3(0.5, 0.5, 0.5);
	dirLight.specular = vec3(0.5, 0.5, 0.5);
	dirLight.direction = normalize(vec3(0.0, 1.0, 1.0));
	
	pointLight.ambient = vec3(0.1, 0.1, 0.1);
	pointLight.diffuse = vec3(0.5, 0.5, 0.5);
	pointLight.specular = vec3(0.5, 0.5, 0.5);
	pointLight.attenuation = vec3(0.0, 0.0, 1.0);
	pointLight.position = vec3(2.0, 2.5, -7.0);

	
	Ray ray;
	ray.direction = normalize(rayDirection);
	ray.origin = eye;
	//ray.origin = vec3(0.0, 0.0, 0.0);

	Plane ground;
	ground.point = vec3(0.0, -1.0, 0.0);
	ground.normal = normalize(vec3(0.0, 1.0, 0.0));
	ground.color = normalize(vec3(213, 206, 139));//yellow
	ground.beta = 200;
	ground.objID = 0;
	planes[0] = ground;

	Sphere sphere1;
	sphere1.center = vec3(0.0, 0.1, -10.0);
	sphere1.radius = 1.0;
	sphere1.color = normalize(vec3(116, 182, 200));//cyan
	sphere1.beta = 256;
	sphere1.objID = 1;
	spheres[0] = sphere1;
	
	Sphere sphere2;
	sphere2.center = vec3(2.0, 0.5, -8.0);
	sphere2.radius = 1.0;
	sphere2.color = normalize(vec3(88, 68, 187));//purple
	sphere2.beta = 100;
	sphere2.objID = 2;
	spheres[1] = sphere2;
	
	Sphere sphere3;
	sphere3.center = vec3(1.0, 2.0, -9.0);
	sphere3.radius = 1.0;
	sphere3.color = normalize(vec3(234, 166, 83));//orange
	sphere3.objID = 3;
	sphere3.beta = 1;
	spheres[2] = sphere3;
	
	//FragColor = vec4(ray.direction, 1.0);
	vec3 ray2Color = vec3(0.0, 0.0, 0.0);
	vec3 ray3Color = vec3(0.0, 0.0, 0.0);
	vec3 ray4Color = vec3(0.0, 0.0, 0.0);
	
	vec3 ray1Color = rayCast(ray);
	if (!all(equal(ray1Color,bgColor))){
		ray2Color = rayCast(ray);
		if (!all(equal(ray2Color,bgColor))){
			ray3Color = rayCast(ray);
			if (!all(equal(ray3Color,bgColor))){
				ray4Color = rayCast(ray);
			}
		}
	}
	
	vec3 finalRayColor = ray1Color + .5 *(ray2Color + .5 * (ray3Color + .5 * ray4Color));
	FragColor = vec4(finalRayColor, 1.0);
}

