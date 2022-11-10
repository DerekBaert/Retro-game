#include "ofApp.h"
#include "Paddle.h"

//--------------------------------------------------------------

void ofApp::setup()
{
	paused = false;
	lives = 3;
	brickStart = ofGetHeight() / 10;
	fifthOfScreen = brickStart / 2;

	// Setting up the variables for creating the paddle, and the speed of the paddle as well
	RectangleSize paddleSize{ 20,100 };
	Coordinates paddlePosition{ static_cast<float>((ofGetWidth() / 2) - (paddleSize.width / 2)), static_cast<float>(ofGetHeight() - 50) };
	float paddleSpeed = 10;

	// Setting up the variables for creating the ball
	float ballSize{ 10 };
	Coordinates ballPosition{ paddlePosition.x + (paddleSize.width / 2) - (ballSize / 2),(paddlePosition.y - ballSize)-1 };

	ball = { ballSize,ballPosition };
	paddle = { paddleSize, paddlePosition, paddleSpeed };

	buffer = ofGetWidth() / 400;
	brickWidth = (ofGetWidth()  / 14) - buffer;
	RectangleSize brickSize{ static_cast<float>(ofGetHeight() / 50), brickWidth };

	for(float x = buffer; x < ofGetWidth() + buffer; x += brickWidth + buffer)
	{
		float y = brickStart;
		for(int i = 1; i < 9; i++)
		{			
			Brick brick;

			switch (i)
			{
			case 1:
			case 2:
				brick = {{brickSize}, {x, y}, ofColor(255,0,0), 7};
				break;
			case 3:
			case 4:
				brick = { {brickSize}, {x, y}, ofColor(255,165,0),5 };
				break;
			case 5:
			case 6:
				brick = { {brickSize}, {x, y}, ofColor(0,255,0),3 };
				break;
			case 7:
			case 8:
				brick = { {brickSize}, {x, y}, ofColor(255,255,0),1 };
				break;
			default:
				break;
			}

			bricks.push_back(brick);
			y += fifthOfScreen;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofFill();
	ofBackground(0);
	ofSetColor(255);
	
	ofDrawBitmapString("Points: " + std::to_string(points), 10, fifthOfScreen);
	ofDrawBitmapString("Lives: " + std::to_string(lives), ofGetWidth() - 100, fifthOfScreen);

	for(auto brick : bricks)
	{
		brick.draw();
	}

	paddle.draw();
	ball.draw();

	if (paused)
	{
		if (gameOver)
		{
			ofDrawBitmapString("Game over", ofGetWidth() / 2, ofGetHeight() / 2);
			ofDrawBitmapString("Press Space to continue", ofGetWidth() / 2, ofGetHeight() / 2 + 10);
		}
		else
		{
			ofDrawBitmapString("Pause", ofGetWidth() / 2, ofGetHeight() / 2);
		}		
	}

	
}

//--------------------------------------------------------------
void ofApp::update()
{	
	ball.hitSide();

	if(paddle.hit(ball.getRect()))
	{
		ball.reverseY();
	}

	for(auto &brick :bricks)
	{

		if(!brick.destroyed())
		{
			ofRectangle ballRect = ball.getRect();
			//ofRectangle brickRect = brick.getRect();

			if (brick.topBottomHit(ballRect))
			{
				ball.reverseY();
			}
			if(brick.sideHit(ballRect))
			{
				ball.reverseX();
			}
		}
		//if(!brick.destroyed() && brick.hit(ball.getRect()))
		//{
		//	//Sides brickSides = brick.getSides();
		//	//Sides ballSides = ball.getSides();
		//	ofRectangle ballRect = ball.getRect();
		//	ofRectangle brickRect = brick.getRect();

		//	if(ballRect.getTop() <= brickRect.getBottom() && ballRect.getTop() >= brickRect.getTop())
		//	{
		//		ball.reverseY();
		//	}
		//	if(ballRect.getBottom() <= brickRect.getTop() && ballRect.getBottom() >= brickRect.getBottom())
		//	{
		//		ball.reverseY();
		//	}

		//	if ((ballRect.getRight() >= brickRect.getLeft() && ballRect.getRight() <= brickRect.getRight()) 
		//		&& (ballRect.getBottom() < brickRect.getTop() + brickRect.height / 2)
		//		|| ballRect.getTop() > brickRect.getBottom() - brickRect.height / 2)
		//	{
		//		ball.reverseX();
		//	}

		//	if ((ballRect.getLeft() >= brickRect.getRight() && ballRect.getLeft() <= brickRect.getLeft())
		//		&& (ballRect.getBottom() < brickRect.getTop() + brickRect.height / 2)
		//		|| ballRect.getTop() > brickRect.getBottom() - brickRect.height / 2)
		//	{
		//		ball.reverseX();
		//	}

		//	points += brick.getPoints();
		//	break;
		//}
	}

	if(ball.hitTop())
	{
		paddle.shrink();
		ofDrawBitmapString("Hit top", 50, 75);
	}

	if(ball.hitBottom())
	{
		if(lives > 0)
		{
			ball.reset();
			lives--;
		}
		else
		{
			gameOver = true;
			paused = true;
		}
		
	}

	if(!paused)
	{
		ball.move();
	}	
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if(key == OF_KEY_LEFT && !paused)
	{
		paddle.move(-1);
	}
	if (key == OF_KEY_RIGHT && !paused)
	{
		paddle.move(1);
	}
	if(key == OF_KEY_SPACE)
	{
		if(gameOver)
		{
			resetGame();
		}
		paused = !paused;
	}
}

void ofApp::resetGame()
{
	for(auto &brick : bricks)
	{
		brick.reset();
	}
	points = 0;
	lives = 3;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
