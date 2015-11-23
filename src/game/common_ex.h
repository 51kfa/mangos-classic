#ifndef _COMMON_EX_
#define _COMMON_EX_

inline bool IsNearWaypoint(float currentPositionX, float currentPositionY, float currentPositionZ, float destinationPostionX, float destinationPostionY, float destinationPostionZ, float distanceX, float distanceY, float distanceZ)
{
	float xDifference = 0;
	float yDifference = 0;
	float zDifference = 0;
	if (distanceX > 0)
	{
		if (currentPositionX > destinationPostionX)
			xDifference = currentPositionX - destinationPostionX;
		else
			xDifference = destinationPostionX - currentPositionX;
	}
	if (distanceY > 0)
	{
		if (currentPositionY > destinationPostionY)
			yDifference = currentPositionY - destinationPostionY;
		else
			yDifference = destinationPostionY - currentPositionY;
	}
	if (distanceZ > 0)
	{
		if (currentPositionZ > destinationPostionZ)
			zDifference = currentPositionZ - destinationPostionZ;
		else
			zDifference = destinationPostionZ - currentPositionZ;
	}
	if (((distanceX > 0 && xDifference < distanceX) && (distanceY > 0 && yDifference < distanceY) && (distanceZ > 0 && zDifference < distanceZ)) ||
			((distanceX == 0) && (distanceY > 0 && yDifference < distanceY) && (distanceZ > 0 && zDifference < distanceZ)) ||
			((distanceX > 0 && xDifference < distanceX) && (distanceY == 0) && (distanceZ > 0 && zDifference < distanceZ)) ||
			((distanceX > 0 && xDifference < distanceX) && (distanceY > 0 && yDifference < distanceY) && (distanceZ == 0)) ||
			((distanceX > 0 && xDifference < distanceX) && (distanceY == 0) && (distanceZ == 0)) ||
			((distanceX == 0) && (distanceY > 0 && yDifference < distanceY) && (distanceZ == 0)) ||
			((distanceX == 0) && (distanceY == 0) && (distanceZ > 0 && zDifference < distanceZ))
	   )
		return true;
	return false;
}
#endif
