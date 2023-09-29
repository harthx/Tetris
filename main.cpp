/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You don't need to change anything in the driver code/main function except the part(s) where instructed.
 * You are expected to define your functionalities in the "functionality.h" header and call them here in the instructed part(s).
 * The game is started with a small box, you need to create other possible in "pieces.h" file in form of array.
    E.g., Syntax for 2 pieces, each having four coordinates -> int Pieces[2][4];
 * Load the images into the textures objects from img directory.
 * Don't make any changes to the "utils.h" header file except for changing the coordinate of screen or global variables of the game.
 * SUGGESTION: If you want to make changes in "utils.h", do it once you complete rest of the functionalities.
 * TIP: You can study SFML for drawing scenes, sprites and window.
 * */

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"
#include <fstream>
using namespace sf;
using namespace std;
int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(430, 460), title);
    Texture obj1, obj2, obj3, shadowTexture, bombTexture, explosion1, explosion2, explosion3, explosion4, explosion5, explosion6, explosion7;
    Font textFont;
    Text text;
    RectangleShape textBox;
    Music theme;
    textFont.loadFromFile("font/Tetris.ttf");
    obj1.loadFromFile("img/tiles.png");
    obj2.loadFromFile("img/background.jpg");
    obj3.loadFromFile("img/frame.png");
    shadowTexture.loadFromFile("img/shadows.png");
    bombTexture.loadFromFile("img/bombs.png");
    explosion1.loadFromFile("img/explosion1.png");
    explosion2.loadFromFile("img/explosion2.png");
    explosion3.loadFromFile("img/explosion3.png");
    explosion4.loadFromFile("img/explosion4.png");
    explosion5.loadFromFile("img/explosion5.png");
    explosion6.loadFromFile("img/explosion6.png");
    explosion7.loadFromFile("img/explosion7.png");
    if (!theme.openFromFile("sounds/maintheme.ogg"))
    {
        std::cout << "error";
    }

    Sound clearOne;
    SoundBuffer clearOneBuffer;
    clearOneBuffer.loadFromFile("sounds/clearOne.ogg");
    clearOne.setBuffer(clearOneBuffer);

    Sound clearFour;
    SoundBuffer clearFourBuffer;
    clearFourBuffer.loadFromFile("sounds/clearFour.ogg");
    clearFour.setBuffer(clearFourBuffer);

    Sound hardDrop;
    SoundBuffer hardDropBuffer;
    hardDropBuffer.loadFromFile("sounds/instantDrop.ogg");
    hardDrop.setBuffer(hardDropBuffer);

    Sound moveSound;
    SoundBuffer moveSoundBuffer;
    moveSoundBuffer.loadFromFile("sounds/movePiece.ogg");
    moveSound.setBuffer(moveSoundBuffer);

    Sound rotateSound;
    SoundBuffer rotateSoundBuffer;
    rotateSoundBuffer.loadFromFile("sounds/rotatePiece.ogg");
    rotateSound.setBuffer(rotateSoundBuffer);

    Sound levelUp;
    SoundBuffer levelUpBuffer;
    levelUpBuffer.loadFromFile("sounds/levelUp.ogg");
    levelUp.setBuffer(levelUpBuffer);

    Sound landPiece;
    SoundBuffer landPieceBuffer;
    landPieceBuffer.loadFromFile("sounds/landPiece.ogg");
    landPiece.setBuffer(landPieceBuffer);

    Sound explosion;
    SoundBuffer explosionBuffer;
    explosionBuffer.loadFromFile("sounds/explosion.wav");
    explosion.setBuffer(explosionBuffer);

    theme.play();
    theme.setLoop(true);

    Sprite sprite(obj1), testbg(obj2), frame(obj3), ghost(shadowTexture), bomb(bombTexture);
    Sprite explosionOne(explosion1), explosionTwo(explosion2), explosionThree(explosion3), explosionFour(explosion4), explosionFive(explosion5), explosionSix(explosion6);
    Sprite explosionSeven(explosion7);
    int delta_x = 0, colorNum = 1, bombColor = rand() % 7;         // bombColor controls the color of the bomb
    float timer = 0, delay = 0.99, bombTimer = 0, bombDelay = 0.2; // bombTimer is in spawning the bomb, bombDelay is the rate at which it falls.
    float levelTimer = 0;                                          // levelTimer is used to the rate at which the leve increments
    bool rotate = 0;
    int rotateCount = 0; // rotateCount is used to measure whether we should do a rotation in the 1st, 2nd, 3rd or 4th quadrant.
    /*
        bombPosition is used to control the "spawning" of the bomb. When the bombTimer is less than the spawn interval of the bomb, bombPosition
        is 500, i.e out of the window, so the bomb does not appear. When the bomb should spawn, bombPosition's value is changed so that
        the bomb appears where it should. In this way, it appears that the bomb spawns and despawns on the window.
    */
    int bombPosition = 500;
    float bombDrop = 0; // bombDrop controls the vertical droprate of the bomb.
    bool instantFall = false;
    int userScore = 0;
    int randColor = 1 + rand() % 7; // both this and randNum are used to store the color/piece that the nextPiece will be, and are used
    // to assign values to the current piece.
    int randNum = rand() % 7;
    int currentNum = 3;
    int numLines = 0;
    int bombPos[1][2] = {0};        // This is used to control the bomb's position
    bombPos[0][0] = rand() % 6 + 2; // By default, the bomb's horizontal position will be from 2-8.
    int saveBombPos[1][2] = {0};
    int nextPiece[4][2] = {0};
    int shadow[4][2] = {0};
    int tempGrid[M][N] = {0};   // Used in the destruction of lines
    int checkPiece[4][2] = {0}; // Used to check which piece is currently active on the board
    int level = 1;
    int nextPieceOffset = 275; // After the level exceeds a certain point, we remove the nextPiece from the screen by increasing this.
    bool landing = false;
    bool didLevelUp = false;
    bool exploded = false;
    bool isExploding = false;
    bool firstExplode = 0, secondExplode = 0, thirdExplode = 0, fourthExplode = 0, fifthExplode = 0, sixthExplode = 0, seventhExplode = 0;
    bool resetExplosionTime = 0;
    float explosionTimerOne = 0, explosionTimerTwo = 0, explosionTimerThree = 0, explosionTimerFour = 0, explosionTimerFive = 0, explosionTimerSix = 0, explosionTimerSeven = 0;
    bool playingGame = false;
    bool mainMenu = true;
    bool gameBegun = false;
    bool levelMenu = false;
    bool soundsMenu = false;
    bool highuserScoresMenu = false;
    bool increaseLevel = false;
    bool decreaseLevel = false;
    int levelStore = level;
    int readuserScore = 0;
    bool isGameOver = false;
    bool gameOverScreen = false;
    bool pauseMenu = false;
    bool inGameLevelMenu = false;
    bool inGameHighuserScores = false;
    float gameOverTimer = 0;
    float timeOne = 0;

    Clock clock;
    fstream myFile;
    while (window.isOpen())
    {
        //---Event Listening Part---//
        Event e;
        while (window.pollEvent(e))
        {                                // Event is occurring - until the game is in running state
            if (e.type == Event::Closed) // If cross/close is clicked/pressed
                window.close();          // Opened window disposes
            if (e.type == Event::KeyPressed)
            {                                           // If any other key (not cross) is pressed
                if (e.key.code == Keyboard::Up)         // Check if the other key pressed is UP key
                    rotate = true;                      // Rotation gets on
                else if (e.key.code == Keyboard::Left)  // Check if the other key pressed is LEFT key
                    delta_x = -1;                       // Change in X-Axis - Negative
                else if (e.key.code == Keyboard::Right) // Check if the other key pressed is RIGHT key
                    delta_x = 1;                        // Change in X-Axis - Positive
            }
            if (e.type == Event::KeyReleased)
            {
                if (e.key.code == Keyboard::Space) // If the user releases space, the block instantly falls down.
                    instantFall = true;
                else if (e.key.code == Keyboard::D)
                    increaseLevel = true;
                else if (e.key.code == Keyboard::A)
                    decreaseLevel = true;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) // Just another way to detect key presses without event listener
            delay = 0.05;                           // If DOWN key is pressed, delay in falling piece decreases from 0 to 0.05

        if (mainMenu)
        {
            text.setCharacterSize(30);
            text.setFillColor(Color::White);
            text.setFont(textFont);

            text.setString("1. START");
            text.setPosition(120, 50);
            window.clear(Color::Black);
            window.draw(testbg);
            window.draw(text);

            text.setString("2. LEVEL");
            text.setPosition(120, 150);
            window.draw(text);

            text.setString("3. HIGH userScoreS");
            text.setPosition(120, 250);
            window.draw(text);

            text.setString("4. QUIT");
            text.setPosition(120, 350);
            window.draw(text);

            if (e.type == Event::KeyReleased)
            {
                if (e.key.code == Keyboard::Num1)
                {
                    mainMenu = false;
                    gameBegun = 1;
                    playingGame = true;
                }

                else if (e.key.code == Keyboard::Num2)
                {
                    mainMenu = false;
                    levelMenu = true;
                }

                else if (e.key.code == Keyboard::Num3)
                {
                    mainMenu = false;
                    highuserScoresMenu = true;
                }

                else if (e.key.code == Keyboard::Num4)
                {
                    window.close();
                }
            }
        }

        if (levelMenu)
        {
            window.clear();
            window.draw(testbg);

            text.setString("CURRENT LEVEL:");
            text.setPosition(60, 50);
            window.draw(text);

            text.setString(std::to_string(level));
            text.setPosition(350, 50);
            window.draw(text);

            text.setString("'D' TO INCREASE");
            text.setPosition(60, 120);
            window.draw(text);

            text.setString("'A' TO DECREASE");
            text.setPosition(60, 190);
            window.draw(text);

            if (increaseLevel)
            {
                level += 1;
                if (level > 15)
                    level = 15;
                increaseLevel = 0;
            }
            if (decreaseLevel)
            {
                level -= 1;
                if (level < 1)
                    level = 1;
                decreaseLevel = 0;
            }

            text.setString("ESC TO GO BACK");
            text.setPosition(60, 260);
            window.draw(text);

            if (e.type == Event::KeyReleased)
                if (e.key.code == Keyboard::Escape)
                {
                    mainMenu = true;
                    levelMenu = false;
                }
        }

        if (highuserScoresMenu)
        {
            window.clear();
            window.draw(testbg);

            text.setString("HIGH userScoreS:");
            text.setPosition(60, 50);
            window.draw(text);
        }

        if (gameOverScreen)
        {
            timeOne = clock.getElapsedTime().asSeconds();
            gameOverTimer += timeOne;
            window.clear();
            window.draw(testbg);

            text.setString("GAME          OVER");
            text.setPosition(100, 150);
            window.draw(text);
            if (gameOverTimer > 5)
            {
                window.close();
            }
        }

        if (pauseMenu)
        {
            text.setCharacterSize(30);
            text.setFillColor(Color::White);
            text.setFont(textFont);

            text.setString("1. RESUME");
            text.setPosition(120, 50);
            window.clear(Color::Black);
            window.draw(testbg);
            window.draw(text);

            text.setString("2. RESTART");
            text.setPosition(120, 135);
            window.draw(text);

            text.setString("3. LEVEL");
            text.setPosition(120, 220);
            window.draw(text);

            text.setString("4. HIGH userScore");
            text.setPosition(120, 305);
            window.draw(text);

            text.setString("5. QUIT");
            text.setPosition(120, 390);
            window.draw(text);

            if (e.type == Event::KeyReleased)
            {
                if (e.key.code == Keyboard::Num1)
                {
                    pauseMenu = false;
                    playingGame = true;
                }

                else if (e.key.code == Keyboard::Num2)
                {
                    pauseMenu = false;
                    playingGame = true;
                    gameBegun = true;
                    point_1[0][0] = 0;
                    point_1[0][1] = 0;
                    point_1[1][0] = 1;
                    point_1[1][1] = 0;
                    point_1[2][0] = 0;
                    point_1[2][1] = 1;
                    point_1[3][0] = 1;
                    point_1[3][1] = 1;
                    destroyGrid();
                    bombPosition = 500;
                    colorNum = 1 + rand() % 7;
                    bombColor = rand() % 7;
                    level = 1;
                    userScore = 0;
                    randColor = 1 + rand() % 7;
                }

                else if (e.key.code == Keyboard::Num3)
                {
                    pauseMenu = false;
                    inGameLevelMenu = true;
                }

                else if (e.key.code == Keyboard::Num4)
                {
                    highuserScoresMenu = true;
                    pauseMenu = false;
                }

                else if (e.key.code == Keyboard::Num5)
                {
                    window.close();
                }
            }
        }

        if (inGameLevelMenu)
        {
            window.clear();
            window.draw(testbg);

            text.setString("CURRENT LEVEL:");
            text.setPosition(60, 50);
            window.draw(text);

            text.setString(std::to_string(level));
            text.setPosition(350, 50);
            window.draw(text);

            text.setString("'D' TO INCREASE");
            text.setPosition(60, 120);
            window.draw(text);

            text.setString("'A' TO DECREASE");
            text.setPosition(60, 190);
            window.draw(text);

            if (increaseLevel)
            {
                level += 1;
                if (level > 15)
                    level = 15;
                increaseLevel = 0;
            }
            if (decreaseLevel)
            {
                level -= 1;
                if (level < 1)
                    level = 1;
                decreaseLevel = 0;
            }

            text.setString("ESC TO GO BACK");
            text.setPosition(60, 260);
            window.draw(text);

            if (e.type == Event::KeyReleased)
                if (e.key.code == Keyboard::Escape)
                {
                    pauseMenu = true;
                    inGameLevelMenu = false;
                }
        }
        ///////////////////////////////////////////////
        ///*** START CALLING YOUR FUNCTIONS HERE ***///
        if (playingGame)
        {
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            bombTimer += time;
            bombDrop += time;
            levelTimer += time;
            if (gameBegun)
            {
                timer = 0;
                bombTimer = 0;
                bombDrop = 0;
                levelTimer = 0;
                clock.restart();
                gameBegun = 0;
            }

            if (e.type == Event::KeyReleased)
                if (e.key.code == Keyboard::H)
                {
                    playingGame = false;
                    pauseMenu = true;
                }

            // We use this function to get the nextPiece to display on the screen.
            setNextPiece(randNum, nextPiece, level);

            // This function enables the user to press space to make the block fall instantly.
            fallAtOnce(instantFall, delay, colorNum);

            if (instantFall)
                hardDrop.play();
            instantFall = false;

            fallingPiece(timer, delay, colorNum, randNum, randColor, currentNum, checkPiece, landing); // Example: fallingPiece() function is called here
            if (landing)
            {
                landPiece.play();
                landing = false;
            }
            // horizontalMove() controls the horizontal movement of the blocks, ensuring that it is legal.
            horizontalMove(delta_x);
            if (delta_x != 0)
                moveSound.play();
            delta_x = 0;

            // This function rotates the piece. The relevant data (i.e the rotation status in rotate, and the rotation counter) is sent.
            if (rotatePiece(rotate, rotateCount, currentNum, checkPiece))
            {
                // If the rotation happens, function returns 1 and rotation counter is updated.
                rotateCount++;
                rotateSound.play();
            }
            // When we get to the next block (when !anomaly evaluates true), the rotation counter is set to zero.
            if (!anomaly())
                rotateCount = 0;
            // After each call, rotate is set to false since we only rotate once for each input.
            rotate = false;

            // We cycle through every possible row index to check if any rows are filled, and destroy them if necessary.
            for (int rowIndex = 0; rowIndex < M; rowIndex++)
                destroyLine(rowIndex, userScore, numLines, tempGrid);
            // The user's userScore is updated with how many lines are destroyed.
            updateScore(numLines, userScore);
            if (numLines == 1 || numLines == 2 || numLines == 3)
                clearOne.play();
            else if (numLines == 4)
                clearFour.play();
            numLines = 0;

            // blockShadow makes the shadow block the same as the piece currently in the grid.
            blockShadow(colorNum, shadow);
            // This function controls the movement of the shadow, i.e where it can legally be/not be.
            createShadow(shadow);

            // This function controls the bomb's movement, color, delay rate, and position.
            (fallingBomb(bombTimer, bombDelay, bombColor, bombDrop, bombPosition, colorNum, bombPos, exploded, isExploding, saveBombPos));
            if (exploded)
            {
                explosion.play();
                exploded = false;
            }

            // This function changes the level for each 15 seconds the user plays the game.
            changeLevel(levelTimer, level, didLevelUp);
            if (didLevelUp)
            {
                levelUp.play();
                didLevelUp = false;
            }
            // This function changes the delay based on the level the user is currently on.
            levelDelay(delay, level);

            /*
                Even though the delay is already being updated in levelDelay, we also update it here as a failsafe against the fallAtOnce
                function. In it, the delay is set to zero, which would interfere with our level delay function. So in the case that the delay would become zero,
                we assign it to be 0.15 when the level is >= 29.
            */
            if (level < 9)
                delay = 1 - (level * 0.1);
            else
                delay = 0.2;

            gameOver(isGameOver);

            gridShrink(level);
            ///*** YOUR CALLING STATEMENTS END HERE ***///
            //////////////////////////////////////////////
            if (isGameOver)
            {
                playingGame = false;
                gameOverScreen = true;
            }

            window.clear(Color::Black);
            window.draw(testbg);

            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    if (gameGrid[i][j] == 0)
                        continue;

                    ghost.setTextureRect(IntRect(gameGrid[i][j] * 18, 0, 18, 18));
                    ghost.setPosition(j * 18, i * 18);
                    ghost.move(28, 31);
                    window.draw(ghost);
                    sprite.setTextureRect(IntRect(gameGrid[i][j] * 18, 0, 18, 18));
                    sprite.setPosition(j * 18, i * 18);
                    sprite.move(28, 31); // offset
                    window.draw(sprite);
                }
            }

            // Setting the text settings and font to display on the window
            text.setCharacterSize(30);
            text.setFillColor(Color::White);
            text.setFont(textFont);

            text.setString("NEXT");
            text.setPosition(265, 50);
            window.draw(text);

            // Displaying the level the user is on
            text.setString("LEVEL");
            text.setPosition(260, 170);
            window.draw(text);

            text.setString(std::to_string(level));
            text.setPosition(295, 230);
            window.draw(text);

            // Displaying current userScore of the user
            text.setString("userScore");
            text.setPosition(255, 290);
            window.draw(text);

            text.setString(std::to_string(userScore));
            text.setPosition(290, 330);
            window.draw(text);

            // The bomb will only be rendered once, so we place it outside of the i < 4 loops here.
            // It's size and offset will be the same as a normal block, however. I made the sprite to be similar to the blocks,
            // so the width is the same (18 pixels) but the height is 19 pixels, 1 higher.
            bomb.setTextureRect(IntRect((bombColor)*18, 0, 18, 19));
            bomb.setPosition(bombPosition * 18, bombPos[0][1] * 18);
            bomb.move(28, 31);
            window.draw(bomb);

            if (isExploding)
            {
                while (explosionTimerOne <= 0.1 && !firstExplode)
                {
                    explosionOne.setPosition(saveBombPos[0][0] * 18 - 18, saveBombPos[0][1] * 18);
                    explosionOne.move(28, 31);
                    window.draw(explosionOne);
                    explosionTimerOne += time;
                    explosionTimerTwo = 0;
                }
                firstExplode = 1;
                while (explosionTimerTwo <= 0.1 && firstExplode)
                {
                    explosionTwo.setPosition(saveBombPos[0][0] * 18 - 18, saveBombPos[0][1] * 18);
                    explosionTwo.move(28, 31);
                    window.draw(explosionTwo);
                    explosionTimerTwo += time;
                    explosionTimerThree = 0;
                    firstExplode = 0;
                }
                secondExplode = 1;
                while (explosionTimerThree <= 0.1 && secondExplode)
                {
                    explosionThree.setPosition(saveBombPos[0][0] * 18 - 18, saveBombPos[0][1] * 18);
                    explosionThree.move(28, 31);
                    window.draw(explosionThree);
                    explosionTimerThree += time;

                    explosionTimerFour = 0;
                    secondExplode = 0;
                }
                thirdExplode = 1;
                while (explosionTimerFour <= 0.1 && thirdExplode)
                {
                    explosionFour.setPosition(saveBombPos[0][0] * 18 - 18, saveBombPos[0][1] * 18);
                    explosionFour.move(28, 31);
                    window.draw(explosionFour);
                    explosionTimerFour += time;

                    explosionTimerFive = 0;
                    thirdExplode = 0;
                }
                fourthExplode = 1;
                while (explosionTimerFive <= 0.1 && fourthExplode)
                {
                    explosionFive.setPosition(saveBombPos[0][0] * 18 - 18, saveBombPos[0][1] * 18);
                    explosionFive.move(28, 31);
                    window.draw(explosionFive);
                    explosionTimerFive += time;

                    explosionTimerSix = 0;
                    fourthExplode = 0;
                }
                fifthExplode = 1;
                while (explosionTimerSix <= 0.1 && fifthExplode)
                {
                    explosionSix.setPosition(saveBombPos[0][0] * 18 - 18, saveBombPos[0][1] * 18);
                    explosionSix.move(28, 31);
                    window.draw(explosionSix);
                    explosionTimerSix += time;

                    explosionTimerSeven = 0;
                    fifthExplode = 0;
                }
                sixthExplode = 1;
                while (explosionTimerSeven <= 0.1 && sixthExplode)
                {
                    explosionSeven.setPosition(saveBombPos[0][0] * 18 - 18, saveBombPos[0][1] * 18);
                    explosionSeven.move(28, 31);
                    window.draw(explosionSeven);
                    explosionTimerSeven += time;
                    sixthExplode = 0;
                    seventhExplode = 1;
                }
            }
            if (explosionTimerSeven >= 0.1 && seventhExplode)
            {
                isExploding = 0;
                explosionTimerOne = 0;
                explosionTimerTwo = 0;
                explosionTimerThree = 0;
                explosionTimerFour = 0;
                explosionTimerFive = 0;
                explosionTimerSix = 0;
                explosionTimerSeven = 0;
            }

            // If we reach a level >= 35, the next piece stops displaying by increasing this offset.
            if (level >= 10)
                nextPieceOffset = 500;
            for (int i = 0; i < 4; i++)
            {
                sprite.setTextureRect(IntRect(randColor * 18, 0, 18, 18));
                sprite.setPosition(nextPiece[i][0] * 18 + nextPieceOffset, nextPiece[i][1] * 18 + 110);
                window.draw(sprite);
                ghost.setTextureRect(IntRect((colorNum - 1) * 18, 0, 18, 18));
                ghost.setPosition(point_1[i][0] * 18, shadow[i][1] * 18);
                ghost.move(28, 31);
                window.draw(ghost);
                sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
                sprite.setPosition(point_1[i][0] * 18, point_1[i][1] * 18);
                sprite.move(28, 31);
                window.draw(sprite);
            }
            window.draw(frame);
        }
        //---The Window that now Contains the Frame is Displayed---//
        window.display();
    }
    return 0;
}