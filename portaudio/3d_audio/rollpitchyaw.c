void toSpherical(double x, double y, double z)
{	
	double radius = sqrt(pow(x, 2)+pow(y, 2)+pow(z, 2));
	double elev = acos(z/radius);
	double azim = atan(y/x);
}

void toCartesian(double radius, double elev, double azim)
{
	double x = radius*sin(elev)*cos(azim);
	double y = radius*sin(elev)*sin(azim);
	double z = radius*cos(elev);
}

void getNewCoordinates(double oldX, double oldY, double oldZ, double roll, double pitch, double yaw)
{
	double r[3][3] = {{(cos(yaw)*cos(pitch)),(cos(yaw)*sin(pitch)*sin(roll)-sin(yaw)*cos(roll)),(cos(yaw)*sin(pitch)*cos(roll)+sin(yaw)*sin(roll))},
	{(sin(yaw)*cos(pitch)),(sin(yaw)*sin(pitch)*sin(roll)+cos(yaw)*cos(roll)),(sin(yaw)*sin(pitch)*cos(roll)-cos(yaw)*sin(roll))},
	{(-cos(pitch)),(cos(pitch)*sin(roll)),(cos(yaw)*cos(roll))}};
	
	double newX = (r[1][1]*oldX + r[1][2]*oldY + r[1][3]*oldZ);
	double newY = (r[2][1]*oldX + r[2][2]*oldY + r[2][3]*oldZ);
	double newZ = (r[3][1]*oldX + r[3][2]*oldY + r[3][3]*oldZ);
}