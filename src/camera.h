#include "DxLib.h"

class Camera{
public:
	VECTOR MoveVector;
	VECTOR Position;
	float Mx;
	float Mz;
	float  SinParam;
	float  CosParam;
	float  HAngle;
	float  VAngle;
	float r;
	float v;


	Camera(VECTOR playerPos);
	void update(VECTOR playerPos);
	void update(VECTOR playerPos, float hAngle);
	void update(float hAngle);

};