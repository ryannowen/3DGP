#pragma once
class PerlinNoise
{
private:
	static float Noise(int argX, int argY, int argSeed);
	static float Lerp(float argA, float argB, float argX);
public:
	static float Perlin(float argXPos, float argZPos);
};

