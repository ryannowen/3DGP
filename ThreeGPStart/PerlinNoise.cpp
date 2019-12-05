#include "PerlinNoise.h"

#include <math.h>
#include <time.h>
#include <cstdlib>

float PerlinNoise::Noise(int argX, int argY, int argSeed)
{
	int n = argX + argY * argSeed;  // 57 is the seed –can be tweaked
	n = (n >> 13) ^ n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0f - ((float)nn / 1073741824.0f);
}

float PerlinNoise::Lerp(float argA, float argB, float argX)
{
	float ft = argX * 3.1415927f;
	float f = (1.0f - cos(ft)) * 0.5f;
	return argA * (1.0f - f) + argB * f;
}

float PerlinNoise::Perlin(float argXPos, float argZPos)
{
	srand(time(0));
	int seed{ rand() % INT_MAX };

	float s = Noise((int)argXPos, (int)argZPos, seed);
	float t = Noise((int)argXPos + 1, (int)argZPos, seed);
	float u = Noise((int)argXPos, (int)argZPos + 1, seed);
	float v = Noise((int)argXPos + 1, (int)argZPos + 1, seed);
	float c1 = Lerp(s, t, argXPos);
	float c2 = Lerp(u, v, argXPos);
	return Lerp(c1, c2, argZPos);
}
