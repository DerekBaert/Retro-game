#pragma once


#include "Ball.h"
#include "Brick.h"
#include "ofMain.h"
#include "ofxCenteredTrueTypeFont.h"
#include "Paddle.h"
#include "Sides.h"


class ofApp : public ofBaseApp
{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void resetGame();
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		Ball ball;
		Paddle paddle;
		std::vector<Brick> bricks{};
		RectangleSize paddleSize;
		bool paused = true;
		bool gameStart = true;
		const int OF_KEY_SPACE = 32;
		int points = 0;
		int lives;
		bool gameOver = false;
		bool gameWon = false;
		bool easy;
		bool redFlag;
		bool orangeFlag;
		int brickCounter;
		float brickStart;
		float fifthOfScreen;		
		float brickWidth;
		float buffer;

		ofxCenteredTrueTypeFont gameFont;
		ofxCenteredTrueTypeFont titleFont;

		
};