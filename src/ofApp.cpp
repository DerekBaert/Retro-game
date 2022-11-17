#include "ofApp.h"
#include "Paddle.h"

//--------------------------------------------------------------

// Paddle bounce mirrors bricks, still gets stuck however.

// Font source: https://www.fontspace.com/pixeloid-font-f69232
// Centered True Type Font: https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
// Hit, game over and winSound sounds: https://mixkit.co/free-sound-effects/game/ (no option to link individually)
// Background music: https://soundimage.org/chiptunes/

void ofApp::setup()
{
	//  Setting up buttons and font elements for title screen	
	easyStart.addListener(this, &ofApp::easyMode);
	normalStart.addListener(this, &ofApp::normalMode);
	easyStart.setup("Easy");
	normalStart.setup("Normal");
	easyStart.loadFont("pixel2.ttf", 20);
	normalStart.loadFont("pixel2.ttf", 20);
	easyStart.setPosition(ofGetWidth() / 2 - easyStart.getWidth(), ofGetHeight() * 0.75);
	normalStart.setPosition(ofGetWidth() / 2 + 10, ofGetHeight() * 0.75);

	titleFont.load("pixel2.ttf", 75);
	titleFont.setLetterSpacing(1.5);
	gameFont.load("pixel2.ttf", 20);

	// Creating game manager
	manager = GameManager();
	
	// Calculating starting point for brick grid, and what 1/5 of the screen is for later calculations.
	brickStart = ofGetHeight() / 10;
	fifthOfScreen = brickStart / 2;

	// Sound effect players
	hitSound.load("bounce.wav");
	winSound.load("complete.wav");
	gameOverSound.load("game_over.wav");
	backgroundMusic.load("Arcade-Puzzler.mp3");
	titleMusic.load("Arcade-Heroes.mp3");

	// Sets and starts title music if game has not started
	if(!manager.isGameStarted())
	{
		titleMusic.setLoop(true);
		titleMusic.play();
	}

	// Setting up the variables for creating the paddle, and the speed of the paddle as well
	paddleSize = { 15,100 };
	Coordinates paddlePosition{ static_cast<float>((ofGetWidth() / 2) - (paddleSize.width / 2)), static_cast<float>(ofGetHeight() - 40) };
	float paddleSpeed = 10;
	paddle = { paddleSize, paddlePosition, paddleSpeed };

	// Setting up the variables for creating the ball
	float ballSize{ 10 };
	Coordinates ballPosition{ paddlePosition.x + (paddleSize.width / 2) - (ballSize / 2),(paddlePosition.y - ballSize)-1 };
	ball = { ballSize,ballPosition};	

	// Calculating the buffer (space between each brick) and calculating the width of the brick including buffer
	buffer = ofGetWidth() / 400;
	brickWidth = (ofGetWidth()  / 14) - buffer;
	RectangleSize brickSize{ static_cast<float>(ofGetHeight() / 50), brickWidth };

	// Loop through and add bricks to vector, setting color based on the iteration of the inner loop to figure out which row it will be in
	for(float x = buffer; x < ofGetWidth(); x += brickWidth + buffer)
	{
		float y = brickStart;
		for(int i = 1; i < 9; i++)
		{			
			Brick brick;

			// Checking what color of brick to add based on the iteration
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

	if(manager.isGameStarted())
	{
		gameFont.drawString("Points: " + std::to_string(manager.getScore()), 10, fifthOfScreen);
		gameFont.drawString("Lives: " + std::to_string(manager.getLives()), ofGetWidth() - 150, fifthOfScreen);

		for (auto brick : bricks)
		{
			brick.draw();
		}

		paddle.draw();
		ball.draw();

		if (manager.isGamePaused())
		{
			if (manager.isGameOver())
			{				
				gameFont.drawStringCentered("Game over", ofGetWidth() / 2, ofGetHeight() / 2);
				gameFont.drawStringCentered("Press Space to restart.", ofGetWidth() / 2, ofGetHeight() / 2 + gameFont.getLineHeight());
			}
			else if (manager.isGameWon())
			{				
				gameFont.drawStringCentered("You Won!", ofGetWidth() / 2, ofGetHeight() / 2);
				gameFont.drawStringCentered("Press Space to play again.", ofGetWidth() / 2, ofGetHeight() / 2 + gameFont.getLineHeight());
			}
			else
			{
				gameFont.drawStringCentered("Paused", ofGetWidth() / 2, ofGetHeight() / 2);
			}
		}
	}
	else
	{
		titleFont.drawStringCentered("Brick Breaker", ofGetWidth() / 2, brickStart * 2);
		gameFont.drawStringCentered("Controls:", ofGetWidth() / 2, ofGetHeight() * 0.65);
		gameFont.drawStringCentered("Move Paddle: <- -> keys, or move mouse", ofGetWidth() / 2, ofGetHeight() * 0.65 + gameFont.getLineHeight());
		gameFont.drawStringCentered("Press Space bar to Pause game", ofGetWidth() / 2, ofGetHeight() * 0.65 + gameFont.getLineHeight()* 2);
		/*gameFont.drawStringCentered("Press Enter to start Normal mode.", ofGetWidth() / 2, ofGetHeight() * 0.65 + gameFont.getLineHeight() * 3.5);
		gameFont.drawStringCentered("Press 'E' to start Easy mode.", ofGetWidth() / 2, ofGetHeight() * 0.65 + gameFont.getLineHeight() * 4.5);*/
		//gui.draw();
		easyStart.draw();
		normalStart.draw();
		
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	if(manager.isGameStarted() && !manager.isGameOver() && !manager.isGameWon())
	{
		if(!backgroundMusic.isLoaded())
		{
			backgroundMusic.load("Arcade-Puzzler.mp3");
		}
		if (titleMusic.isPlaying())
		{
			titleMusic.stop();
		}
		//ofDrawBitmapString(backgroundMusic.isLoaded() ? "True" : "False", 200, 200);

		if (!backgroundMusic.isPlaying())
		{
			backgroundMusic.setLoop(true);
			backgroundMusic.play();
		}
	}

	ball.hitSide();

	ofRectangle ballRect = ball.getRect();

	if(paddle.hit(ball.getRect()))
	{
		hitSound.play();
		if (paddle.sideHit(ballRect))
		{
			ball.reverseX();
		}

		// If the brick registers the top or bottom as being hitSound, the ball reverse it's trajectory on the Y axis.
		if (paddle.topHit(ballRect))
		{
			ball.reverseY();
		}
	}
	
	for(auto &brick :bricks)
	{
		// Checking if brick is not destroyed already and the ball is intersecting with it
		if(!brick.destroyed() && brick.getRect().intersects(ballRect))
		{
			// If the brick registers the side as being hitSound, the ball reverse it's trajectory on the X axis.
			if (brick.sideHit(ballRect))
			{
				ball.reverseX();
			}

			// If the brick registers the top or bottom as being hitSound, the ball reverse it's trajectory on the Y axis.
			if (brick.topBottomHit(ballRect))
			{
				ball.reverseY();
			}

			// After hitSound is determined, add the brick's point value to the player's score.
			manager.addPoints(brick.getPoints());

			if(brick.getPoints() == 7 && !manager.wasRedHit())
			{
				ball.increaseSpeed();
				manager.redHit();
			}

			if(brick.getPoints() == 5 && !manager.wasOrangeHit())
			{
				ball.increaseSpeed();
				manager.orangeHit();
			}

			if(manager.brickCount() == 4 || manager.brickCount() == 12)
			{
				ball.increaseSpeed();
			}
			hitSound.play();
			manager.incrementBrickCounter();
			break;		
		}
	}

	// Checking if top of the canvas is hitSound
	if(ball.hitTop() && !manager.isEasyMode())
	{
		paddle.shrink();
	}

	// Checking if the bottom of the canvas is hitSound
	if(ball.hitBottom() && !manager.isGameOver())
	{
		// If lives are above 0, reset the ball's location to the start and decrement the life counter
		if(manager.getLives() > 0)
		{
			ball.reset();
			manager.loseALife();
		}
		// If the lives are 0 or less, set paused and game over to true.
		else
		{
			backgroundMusic.stop();
			gameOverSound.play();
			manager.pauseButton();
			manager.loseGame();			
		}		
	}

	// Ball only moves if game is not paused
	if(!manager.isGamePaused())
	{
		ball.move();
	}

	// Checking if all bricks are gone, and if so make the game as complete
	if(manager.brickCount() >= static_cast<int>(bricks.size()) && !manager.isGameWon())
	{
		backgroundMusic.stop();
		manager.winGame();
		manager.pauseButton();
		winSound.play();
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if(key == OF_KEY_LEFT && !manager.isGamePaused() && !manager.isGameOver())
	{
		paddle.move(-1);
	}
	if (key == OF_KEY_RIGHT && !manager.isGamePaused() && !manager.isGameOver())
	{
		paddle.move(1);
	}


	if(key == OF_KEY_SPACE && manager.isGameStarted())
	{
		if(manager.isGameOver() || manager.isGameWon())
		{
			resetGame();

		}

		manager.pauseButton();
	}	
}

void ofApp::easyMode()
{
	if (!manager.isGameStarted())
	{
		manager.easyModeOn();
		manager.pauseButton();
		manager.gameStarted();
	}
}

void ofApp::normalMode()
{
	if (!manager.isGameStarted())
	{
		manager.pauseButton();
		manager.gameStarted();
	}
}

void ofApp::resetGame()
{
	if(manager.isEasyMode())
	{
		manager.reset();
		manager.easyModeOn();
	}
	else
	{
		manager.reset();
	}
	
	for(auto &brick : bricks)
	{
		brick.reset();
	}
	paddle.reset();
	ball.newGame();
	backgroundMusic.play();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
	if(mouseX > 0 && mouseX + paddle.getWidth() < ofGetWidth() && !manager.isGamePaused())
	{
		paddle.mouseMove(mouseX);
	}	
}