#include "DxLib.h"

class Camera{
public:
	VECTOR MoveVector;
	float  SinParam;
	float  CosParam;
	float  HAngle;
	float  VAngle;

	Camera();
	void update(VECTOR playerPos);
};