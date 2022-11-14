﻿#pragma once
#include "Coordinates.h"
#include "ofRectangle.h"
#include "RectangleSize.h"
#include "Sides.h"

class Ball
{
	float mSize;
	Coordinates mPosition;
	Coordinates mSpeed;
	Coordinates mOffset;
	ofRectangle mRectangle;
	Coordinates mIncrease;
public:
	Ball();
	Ball(float size, Coordinates position);

	void move();
	void increaseSpeed();
	void reverseX();
	void reverseY();
	void draw();
	void hitSide();
	void reset();

	bool hitTop();
	bool hitBottom();

	Coordinates getPosition();
	ofRectangle getRect();
	Sides getSides();


};
