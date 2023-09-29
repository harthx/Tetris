/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */

//---Piece Starts to Fall When Game Starts---//

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
void fallingPiece(float &timer, float &delay, int &colorNum, int &n, int &randColor, int &currentNum, int checkPiece[4][2], bool &landing)
{
    if (timer > delay)
    {
        for (int i = 0; i < 4; i++)
        {
            point_2[i][0] = point_1[i][0];
            point_2[i][1] = point_1[i][1];
            point_1[i][1] += 1; // How much units downward
        }
        if (!anomaly())
        {
            for (int i = 0; i < 4; i++)
            {
                gameGrid[point_2[i][1]][point_2[i][0]] = colorNum;
                landing = true;
            }
            //--- Un-Comment this Part When You Make BLOCKS array---//
            for (int i = 0; i < 4; i++)
            {
                /*
                    When we use the default calculations, i.e "point_1[i][0] = BLOCKS[n][i] % 2; ..." and so on, we realize that
                    1st column shows x-axis offset, and 2nd column shows y-axis offset, where moving down is positive in the y-axis. I reversed the order of
                    divison and modulus so that the number of blocks in a piece show horizontally. i.e, instead of a block showing vertically ilke |, it instead
                    shows horizontally like ___. This makes it easier to display the required pieces in the PDF.
                */
                point_1[i][0] = BLOCKS[n][i] / 2;
                point_1[i][1] = BLOCKS[n][i] % 2;

                // checkPiece will have the initial piece stored in it. It is used just to check which
                // block is currently on the screen.
                checkPiece[i][0] = BLOCKS[n][i] / 2;
                checkPiece[i][1] = BLOCKS[n][i] % 2;
            }
            /*
                colorNum and currentNum are assigned the values of randColor and n, after which randColor and n are changed. This is so
                the next piece can be rendered accurately in the game, and that the piece within the grid always becomes the shown
                next piece.
            */
            colorNum = randColor;
            currentNum = n;
            n = rand() % 7;
            randColor = 1 + rand() % 7;
            timer = 0;
        }
        timer = 0;
    }
}

/////////////////////////////////////////////
///*** START CODING YOUR FUNCTIONS HERE ***///

// This controls the horizontal movement of the block.
void horizontalMove(int &delta_x)
{
    int temp;
    bool canMove = true;
    for (int i = 0; i < 4; i++)
    {
        temp = point_1[i][0] + delta_x;
        // If any of the blocks would exit the grid if they were moved right/left, this evaluates true.
        if (!(temp < N && temp >= 0))
            canMove = false;
        // If any of the blocks would go to an already occupied position if offset, this evaluates true.
        if (gameGrid[point_1[i][1]][point_1[i][0] + delta_x] != 0)
            canMove = false;
    }
    // The block will only be moved left/right if that movement is legal within the provided grid and the required block is not occupied.
    if (canMove)
    {
        for (int i = 0; i < 4; i++)
        {
            point_1[i][0] += delta_x;
        }
    }
}

// Function is declared as boolean since we will send the rotation status to main.
bool rotatePiece(bool rotate, int rotateCount, int currentNum, int checkPiece[4][2])
{
    // If the piece is a square (which is when currentNum will be equal to 3), we do not do any rotation.
    if (currentNum == 3)
        return 0;
    // No rotation necessary if the user does not input rotate command.
    if (!rotate)
        return 0;

    // storeRot is used to store the rotation of the current block.
    int storeRot[4][2];
    int tempArr[4][2];
    for (int i = 0; i < 4; i++)
    {
        // We place the values of point_1 (the current position of the block) into tempArr
        tempArr[i][0] = point_1[i][0];
        tempArr[i][1] = point_1[i][1];
    }
    int iteration = 0;
    while (iteration < 2)
    {
        /*
            All the rotations made here are according to the mathematical principle of 90 degree rotations. When a point is rotated
            90 degrees, it goes from (x, y) to (y, -x). This process has been applied for each rotation in this function.
        */
        if (rotateCount % 4 == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                /*
                    If the rotation is from 1st quad to 4th quad, it's 90 degrees, i.e x = y, and y = -x. Since the first column
                    of the block array stores the x-axis data and second column stores y axis data, we place the corresponding values.
                    NOTE: Here, we place the required block (storeRot[0])'s x-position equal to the y-position of that block's position (checkPiece[1])
                    (and vice versa w/ negative-x for y), and subtract it with the current position of the block as the offset value (tempArr).
                    However, subtracting it once gives us a negative value, which would go out of the grid. This is why I placed the entire
                    rotation function in a loop that runs two times, since subtracting the block's rotated position with the negative x-axis and y-axis
                    numbers gives us the correct offset to place it in the grid.
                */
                storeRot[i][0] = checkPiece[i][1] - tempArr[1][0];
                storeRot[i][1] = -(checkPiece[i][0]) - tempArr[1][1];
            }
        }
        if (rotateCount % 4 == 1)
        {
            for (int i = 0; i < 4; i++)
            {
                // Here, rotation will be of 180 degrees, so both x and y are negative.
                storeRot[i][0] = -(checkPiece[i][0]) - tempArr[1][0];
                storeRot[i][1] = -(checkPiece[i][1]) - tempArr[1][1];
            }
        }
        if (rotateCount % 4 == 2)
        {
            for (int i = 0; i < 4; i++)
            {
                // Here, rotation will be 270 degrees (i.e 3rd quad), so only x is negative.
                storeRot[i][0] = -(checkPiece[i][1]) - tempArr[1][0];
                storeRot[i][1] = checkPiece[i][0] - tempArr[1][1];
            }
        }
        if (rotateCount % 4 == 3)
        {
            for (int i = 0; i < 4; i++)
            {
                // At this point, we arrive back to the 1st quadrant, so x and y both become positive.
                storeRot[i][0] = checkPiece[i][0] - tempArr[1][0];
                storeRot[i][1] = checkPiece[i][1] - tempArr[1][1];
            }
        }
        for (int i = 0; i < 4; i++)
        {
            // Updated the temporary value of the block's position since we will subtract it from the
            // current value of storeRot to get the correct position of the block.
            tempArr[i][0] = storeRot[i][0];
            tempArr[i][1] = storeRot[i][1];
        }
        iteration++;
    }

    bool canRotate = true;

    for (int i = 0; i < 4; i++)
    {
        /*
            If the x-position of the rotated block goes out of the grid (in either negative-x or greater than N), the block can't rotate.
            Similarly, if the y-position of the rotated block goes out of the grid (either above or below M), the block can't rotate.
        */
        if ((tempArr[i][0] < 0 || tempArr[i][0] >= N) || (tempArr[i][1] < 0 || tempArr[i][1] >= M))
            canRotate = false;
        if (gameGrid[tempArr[i][1]][tempArr[i][0]] != 0)
            canRotate = false;
    }

    if (!canRotate)
        return 0;

    // If all checks pass, the block's rotation is legal, and it's position (in point_1) is updated.
    for (int i = 0; i < 4; i++)
    {
        point_1[i][0] = tempArr[i][0];
        point_1[i][1] = tempArr[i][1];
    }
    return 1;
}

// This function is used to check for any filled lines and destroy them if necessary.
void destroyLine(int rowIndex, int &score, int &numLines, int tempGrid[M][N])
{
    bool isFull = true;
    for (int i = 0; i < N; i++)
        // If at any point a square isn't colored in a row, it means that the row is not filled.
        if (gameGrid[rowIndex][i] == 0 || gameGrid[rowIndex][i] == 8)
            isFull = false;
    if (isFull)
    {
        /*
            If only one row is destroyed, tempGrid stays zero. In general, tempGrid will copy everything below the
            row currently being destroyed.
        */
        for (int i = M - 1; i > rowIndex; i--)
            for (int j = 0; j < N; j++)
                tempGrid[i][j] = gameGrid[i][j];
        //    tempGrid copies everything above the row we need to destroy.
        for (int i = rowIndex; i > 0; i--)
            for (int j = 0; j < N; j++)
                tempGrid[i][j] = gameGrid[i - 1][j];
        // gameGrid becomes tempGrid for every value above the row being destroyed, i.e the rows are shifted down
        for (int i = rowIndex; i > 0; i--)
            for (int j = 0; j < N; j++)
                gameGrid[i][j] = tempGrid[i][j];
        numLines++;
    }
}

// This function is used to update the score for how many lines are destroyed.
void updateScore(int &numLines, int &score)
{
    // If only one line is destroyed, the score increases by 10.
    if (numLines == 1)
        score += 10;
    // If two lines are destroyed, score increases by 30.
    else if (numLines == 2)
        score += 30;
    // If three lines are destroyed, score increases by 60.
    else if (numLines == 3)
        score += 60;
    // If four lines are destroyed, score increases by 100.
    else if (numLines == 4)
        score += 100;
    // Setting the destroyed number of lines to be zero after the score has been updated.
}

bool canShadowExist(int shadow[4][2])
{
    /*
        We check if the shadow can exist at any given point. This is similar to the anomaly function, just for the shadow's
        co-ordinates. If the shadow can exist, the function returns 1, else it returns false.
    */
    for (int i = 0; i < 4; i++)
    {
        if (shadow[i][1] >= M || shadow[i][1] < 0 || shadow[i][0] >= N)
            return 0;
        else if (gameGrid[shadow[i][1]][shadow[i][0]])
            return 0;
    }
    return 1;
}

// In this function, we make the shadow piece equal to the current piece in the grid.
void blockShadow(int colorNum, int shadow[4][2])
{
    for (int i = 0; i < 4; i++)
    {
        shadow[i][0] = point_1[i][0];
        shadow[i][1] = point_1[i][1];
    }
}

void createShadow(int shadow[4][2])
{
    for (int i = 0; i < 4; i++)
    {
        // While it is legal for the shadow to exist in a point, it's vertical position is incremented by 1.
        while (canShadowExist(shadow))
            for (int i = 0; i < 4; i++)
                shadow[i][1] += 1;
        // Once the shadow piece cannot exist, we know that we are 1 vertical position higher than needed. So,
        // we decrement the shadow's vertical position once so its in the correct place.
        for (int i = 0; i < 4; i++)
            shadow[i][1] -= 1;
    }
}

// This controls the instant desecent of the block if the user presses the space key.
void fallAtOnce(bool &instantFall, float &delay, int colorNum)
{
    if (instantFall)
    {
        // As long as the block can legally move, we decrement its position.
        while (anomaly())
        {
            for (int i = 0; i < 4; i++)
                point_1[i][1] += 1;
        }
        // Once anomaly() evaluates false (i.e the block's existence in a position is illegal), we decrement
        // its vertical position.
        for (int i = 0; i < 4; i++)
            point_1[i][1] -= 1;
        // We make instantFall equal to zero since the required block has already fell to the ground.
        instantFall = false;
        // The delay is lowered after the block's instant descent so that it sticks to the ground faster.
        delay = 0;
    }
}

// This is also similar to the anomaly function, we just check if the bomb can exist if it is in a given position.
bool canBombFall(int bombPos[1][2])
{
    if (bombPos[0][1] >= M - 1)
        return 0;
    else if (gameGrid[bombPos[0][1]][bombPos[0][0]])
        return 0;
    return 1;
}

// This is used to destroy the grid if the bomb collides with a block of the same color.
void destroyGrid()
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // We simply cycle through every element in the grid and make it zero.
            gameGrid[i][j] = 0;
        }
    }
}

// If the bomb collides with a block that isn't the same color, it destroys blocks in a 2-block radius.
void destroySquare(int bombPos[1][2])
{
    for (int i = bombPos[0][0] - 1; i <= bombPos[0][0] + 1; i++)
        for (int j = bombPos[0][1] - 1; j <= bombPos[0][1] + 1; j++)
            gameGrid[j][i] = 0;
}

void fallingBomb(float &bombTimer, float bombDelay, int &bombColor, float &bombDrop, int &bombPosition, int colorNum, int bombPos[1][2], bool &exploded, bool &isExploding, int saveBombPos[1][2])
{
    // The bomb appears once every 60 seconds.
    if (bombTimer >= 2)
    {
        // The bomb's position is randomized and it drops (its vertical position increments) with a constant delay.
        bombPosition = bombPos[0][0];
        saveBombPos[0][0] = bombPosition;
        if (bombDrop >= bombDelay)
        {
            bombPos[0][1] += 1;
            bombDrop = 0;
        }
    }
    // If the bomb cannot fall any further,
    if (!canBombFall(bombPos))
    {
        // and if the bomb's color is the same as the grid piece's color,
        if (gameGrid[bombPos[0][1]][bombPos[0][0]] == bombColor + 1)
        {
            // the entire grid is destroyed.
            destroyGrid();
            exploded = true;
            isExploding = true;
            saveBombPos[0][1] = bombPos[0][1] - 1;
        }
        // Otherwise, if the bomb's color is different from the piece it's colliding with,
        else if (gameGrid[bombPos[0][1]][bombPos[0][0]] != 0 && gameGrid[bombPos[0][1]][bombPos[0][0]] != bombColor + 1)
        {
            // then only a few blocks are destroyed.
            destroySquare(bombPos);
            exploded = true;
            isExploding = true;
            saveBombPos[0][1] = bombPos[0][1] - 1;
        }
        // If the bomb doesn't collide with a block, nothing happens and it disappears.

        // After the bomb collides/falls to the bottom of the grid, it's color is randomized.
        bombColor = rand() % 7;
        // Its x-coordinate is set to any random value from 2-8 in the grid
        bombPos[0][0] = bombColor + 2;
        // It's vertical position is set to zero, the ceiling of the grid
        bombPos[0][1] = 0;
        // And its timer is reset. I.e, the bomb is going to spawn once again after a set amount of time (20 seconds).
        bombTimer = 0;
        /* The bombpositon is also set to 500, this is a value outside of the window's range. If we want the bomb to spawn
           (when the bomb's timer is >= 20 seconds), the bombPosition is set to be the bomb's x-coordinate, and the
           sprite is properly rendered in the window.
        */
        bombPosition = 500;
    }
}

// This function is used to set the next piece that is displayed in the game.
void setNextPiece(int n, int nextPiece[4][2], int level)
{
    for (int i = 0; i < 4; i++)
    {
        // nextPiece array is the same as the future piece array. It is just one cycle further than the current piece
        // inside the grid, since the value of 'n' is only updated after the piece's lifetime is finished (when anomaly evaluates false)
        // and we assign a value to nextPiece *after* the grid piece.
        nextPiece[i][0] = BLOCKS[n][i] / 2;
        nextPiece[i][1] = BLOCKS[n][i] % 2;
    }
    // We just reduce the horizontal offset of nextPiece by one if it is the S, Z, or L piece to make it look better.
    if (n == 4 || n == 2 || n == 5)
    {
        for (int i = 0; i < 4; i++)
            nextPiece[i][0] -= 1;
    }
}

// This function changes the level we are on.
void changeLevel(float &levelTimer, int &level, bool &didLevelUp)
{
    // Every 30 seconds, the level increments by 1.
    if (levelTimer >= 10 && level != 15)
    {
        level += 1;
        levelTimer = 0;
        didLevelUp = true;
    }
}

void levelDelay(float &delay, int level)
{
    /*
        For each increment in the level, the delay is reduced by 0.03 seconds. It gets reduced up to minimum of 0.2 seconds (the minimum delay),
        after which point the reduction stops. This point will be reached at level 8.
    */
    if (level >= 8)
        return;
    else if ((1 - (level * 0.1) >= 0.15))
        delay = 1 - (level * 0.1);
}

void gameOver(bool &isGameOver)
{
    for (int i = 0; i < N; i++)
    {
        if (gameGrid[0][i] != 0)
            isGameOver = true;
    }
}

void gridShrink(int level)
{
    if (level == 12)
        for (int i = 0; i < N; i++)
        {
            gameGrid[M - 1][i] = 8;
            gameGrid[M - 2][i] = 8;
        }

    if (level == 13)
        for (int i = 0; i < N; i++)
        {
            gameGrid[M - 1][i] = 8;
            gameGrid[M - 2][i] = 8;
            gameGrid[M - 3][i] = 8;
            gameGrid[M - 4][i] = 8;
        }
    if (level == 14)
        for (int i = 0; i < N; i++)
        {
            gameGrid[M - 1][i] = 8;
            gameGrid[M - 2][i] = 8;
            gameGrid[M - 3][i] = 8;
            gameGrid[M - 4][i] = 8;
            gameGrid[M - 5][i] = 8;
            gameGrid[M - 6][i] = 8;
        }
    if (level == 15)
        for (int i = 0; i < N; i++)
        {
            gameGrid[M - 1][i] = 8;
            gameGrid[M - 2][i] = 8;
            gameGrid[M - 3][i] = 8;
            gameGrid[M - 4][i] = 8;
            gameGrid[M - 5][i] = 8;
            gameGrid[M - 6][i] = 8;
            gameGrid[M - 7][i] = 8;
            gameGrid[M - 8][i] = 8;
        }
}
///*** YOUR FUNCTIONS END HERE ***///
////////////////////////////////////