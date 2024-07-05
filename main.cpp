#include <GL/glut.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// el states
enum GameState {
    MENU,
    GAME,
    GAME_OVER
};

GameState currentGameState = MENU; // var type gamestate

float posX = 0.0;
float posY = -0.5;
float velocityY = 0.0;
bool isJumping = false;
const float gravity = 0.006;
const float jumpStrength = 0.1;
int seconds = 0;
int score = 0;
bool isGameOver = false;

const float groundLevel = -0.5;

struct Obstacle {
    float x, y, width, height;
    bool passed;
};

std::vector<Obstacle> obstacles;
float obstacleSpeed = 0.04f;
int obstacleSpawnTime = 40;
// int obstacleSpawnTime2 = 70;
int frameCounter = 0; // 3ashan el new obstacles

struct Button {
    float x, y, width, height;
    std::string label;
};

Button startButton = { -0.2f, 0.2f, 0.4f, 0.1f, "Start Game" };
Button exitButton = { -0.2f, 0.0f, 0.4f, 0.1f, "Exit" };



void drawCharacter() {
    if (isGameOver) return; // dpn't draw law game is over

    float size = 0.5; // el size beta3 el character
    float elevation = 0.06; // trial and error lehad ma sabat 3al ground

    // Helmet
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0); // White
    glVertex2f(posX - 0.06 * size, posY + 0.35 * size + elevation);
    glVertex2f(posX + 0.06 * size, posY + 0.35 * size + elevation);
    glVertex2f(posX + 0.06 * size, posY + 0.23 * size + elevation);
    glVertex2f(posX - 0.06 * size, posY + 0.23 * size + elevation);
    glEnd();

    // el heta el sooda ely fel helmet
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0); // Black
    glVertex2f(posX - 0.05 * size, posY + 0.33 * size + elevation);
    glVertex2f(posX + 0.05 * size, posY + 0.33 * size + elevation);
    glVertex2f(posX + 0.05 * size, posY + 0.25 * size + elevation);
    glVertex2f(posX - 0.05 * size, posY + 0.25 * size + elevation);
    glEnd();

    // Body
    glBegin(GL_POLYGON);
    glColor3f(0.9, 0.9, 0.9); // Light grey
    glVertex2f(posX - 0.06 * size, posY + 0.23 * size + elevation);
    glVertex2f(posX + 0.06 * size, posY + 0.23 * size + elevation);
    glVertex2f(posX + 0.06 * size, posY + 0.0 * size + elevation);
    glVertex2f(posX - 0.06 * size, posY + 0.0 * size + elevation);
    glEnd();

    // Left arm
    glBegin(GL_POLYGON);
    glColor3f(0.9, 0.9, 0.9); // Light grey
    glVertex2f(posX - 0.06 * size, posY + 0.2 * size + elevation);
    glVertex2f(posX - 0.12 * size, posY + 0.15 * size + elevation);
    glVertex2f(posX - 0.12 * size, posY + 0.05 * size + elevation);
    glVertex2f(posX - 0.06 * size, posY + 0.1 * size + elevation);
    glEnd();

    // Right arm
    glBegin(GL_POLYGON);
    glColor3f(0.9, 0.9, 0.9); // Light grey
    glVertex2f(posX + 0.06 * size, posY + 0.2 * size + elevation);
    glVertex2f(posX + 0.12 * size, posY + 0.15 * size + elevation);
    glVertex2f(posX + 0.12 * size, posY + 0.05 * size + elevation);
    glVertex2f(posX + 0.06 * size, posY + 0.1 * size + elevation);
    glEnd();

    // Left leg
    glBegin(GL_POLYGON);
    glColor3f(0.9, 0.9, 0.9); // Light grey
    glVertex2f(posX - 0.04 * size, posY + 0.1 * size + elevation);
    glVertex2f(posX - 0.02 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX - 0.06 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX - 0.08 * size, posY + 0.1 * size + elevation);
    glEnd();

    // Right leg
    glBegin(GL_POLYGON);
    glColor3f(0.9, 0.9, 0.9); // Light grey
    glVertex2f(posX + 0.04 * size, posY + 0.1 * size + elevation);
    glVertex2f(posX + 0.02 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX + 0.06 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX + 0.08 * size, posY + 0.1 * size + elevation);
    glEnd();

    // Left glove
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5); // Grey
    glVertex2f(posX - 0.12 * size, posY + 0.05 * size + elevation);
    glVertex2f(posX - 0.13 * size, posY + 0.04 * size + elevation);
    glVertex2f(posX - 0.12 * size, posY + 0.02 * size+ elevation);
    glVertex2f(posX - 0.11 * size, posY + 0.03 * size + elevation);
    glEnd();

    // Right glove
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5); // Grey
    glVertex2f(posX + 0.12 * size, posY + 0.05 * size + elevation);
    glVertex2f(posX + 0.13 * size, posY + 0.04 * size + elevation);
    glVertex2f(posX + 0.12 * size, posY + 0.02 * size + elevation);
    glVertex2f(posX + 0.11 * size, posY + 0.03 * size + elevation);
    glEnd();

    // Left boot
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5); // Grey
    glVertex2f(posX - 0.06 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX - 0.02 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX - 0.02 * size, posY - 0.12 * size + elevation);
    glVertex2f(posX - 0.06 * size, posY - 0.12 * size + elevation);
    glEnd();

    // Right boot
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5); // Grey
    glVertex2f(posX + 0.06 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX + 0.02 * size, posY - 0.1 * size + elevation);
    glVertex2f(posX + 0.02 * size, posY - 0.12 * size + elevation);
    glVertex2f(posX + 0.06 * size, posY - 0.12 * size + elevation);
    glEnd();

    // Backpack
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5); // Grey
    glVertex2f(posX - 0.05 * size, posY + 0.23 * size + elevation);
    glVertex2f(posX + 0.05 * size, posY + 0.23 * size + elevation);
    glVertex2f(posX + 0.05 * size, posY + 0.15 * size + elevation);
    glVertex2f(posX - 0.05 * size, posY + 0.15 * size + elevation);
    glEnd();
}


void drawObstacle(const Obstacle& obs) {
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5); // Grey color lel obstacles ka2enha asteroids
    glVertex2f(obs.x, obs.y);
    glVertex2f(obs.x + obs.width, obs.y);
    glVertex2f(obs.x + obs.width, obs.y + obs.height);
    glVertex2f(obs.x, obs.y + obs.height);
    glEnd();
}

// from geeksfprgeeks
void drawText(float x, float y, std::string text, float r, float g, float b) {
    glColor3f(r, g, b); // text color
    glRasterPos2f(x, y);
    for (size_t i = 0; i < text.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
}

void drawTimer() {
    std::stringstream ss; // string feeh characters w numbers
    ss << "Timer: " << seconds << "s";
    drawText(-0.9, 0.9, ss.str(), 1.0, 1.0, 1.0);
}

void drawScore() {
    std::stringstream ss;
    ss << "Score: " << score;
    drawText(-0.9, 0.8, ss.str(), 1.0, 1.0, 1.0);
}
void drawScore2() // score lel game over
{
    std::stringstream ss;
    ss << "Score: " << score;
    drawText(-0.2, -0.2, ss.str(), 1.0, 1.0, 1.0);
}

void drawSky() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Background
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0); // White color for stars
    glPointSize(1.0); // Size beta3 kol star is 1 pixel
    glBegin(GL_POINTS);
    for (int i = 0; i < 1000; i++) {
        float x = (float)(rand() % 4000 - 2000) / 1000.0; // Random x-coordinate between -2.0 and 2.0
        float y = (float)(rand() % 4000 - 2000) / 1000.0; // Random y-coordinate between -2.0 and 2.0
        glVertex2f(x, y);
    }
    glEnd();
}

void drawGround() {
    glBegin(GL_LINES);
    glColor3f(0.5, 0.5, 0.5); // Grey color for the ground line
    glVertex2f(-1.0, groundLevel);
    glVertex2f(1.0, groundLevel);
    glEnd();
}

void drawMenu() {
    drawSky(); // Draw the starry sky background
    drawText(-0.2f, 0.4f, "Main Menu", 1.0f, 1.0f, 1.0f);

    //Start
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(startButton.x, startButton.y);
    glVertex2f(startButton.x + startButton.width, startButton.y);
    glVertex2f(startButton.x + startButton.width, startButton.y - startButton.height);
    glVertex2f(startButton.x, startButton.y - startButton.height);
    glEnd();
    drawText(startButton.x + 0.05f, startButton.y - 0.07f, startButton.label, 1.0f, 1.0f, 1.0f);

    // Exit
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(exitButton.x, exitButton.y);
    glVertex2f(exitButton.x + exitButton.width, exitButton.y);
    glVertex2f(exitButton.x + exitButton.width, exitButton.y - exitButton.height);
    glVertex2f(exitButton.x, exitButton.y - exitButton.height);
    glEnd();
    drawText(exitButton.x + 0.1f, exitButton.y - 0.07f, exitButton.label, 1.0f, 1.0f, 1.0f);
}


void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    switch (currentGameState) {
        case MENU:
            drawMenu();
            break;
        case GAME:
            drawSky();
            drawGround();  // Draw the ground line
            drawCharacter();
            drawTimer();            drawScore();
            // Draw obstacles
            for (size_t i = 0; i < obstacles.size(); ++i) {
                drawObstacle(obstacles[i]);
            }
            break;
       case GAME_OVER:
            drawSky();
            drawText(-0.2, 0.0, "Game Over", 1.0, 1.0, 1.0);
             drawScore2();

            break;
    }

    glFlush();
}

void timer(int) {
    if (currentGameState == GAME && !isGameOver) {
        seconds++;
    }
    glutPostRedisplay();
    glutTimerFunc(1000, timer, 0); // bey update el timer kol sanya
}


void update(int) {
    if (isGameOver) {
        // Stop eltimer when game over
        glutTimerFunc(0, timer, 0);
        currentGameState = GAME_OVER;
        glutPostRedisplay();
        return;
    }

    // Apply gravity
    if (posY > groundLevel || isJumping) {
        velocityY -= gravity; // shadet el gravity
        posY += velocityY;

        if (posY <= groundLevel) {
            posY = groundLevel;
            velocityY = 0.0;
            isJumping = false;
        }
    }

    // Move obstacles law el state game
    if (currentGameState == GAME) {
        for (size_t i = 0; i < obstacles.size(); ++i) {
            obstacles[i].x -= obstacleSpeed;
        }

        // Check for passed obstacles w increment el score
        for (size_t i = 0; i < obstacles.size(); ++i) {
            if (!obstacles[i].passed && posX > obstacles[i].x + obstacles[i].width) {
                obstacles[i].passed = true;
                score++;
            }
        }

        // Remove off screen obstacles
      //  if (!obstacles.empty() && obstacles.front().x + obstacles.front().width < -1.0) // law el obstacle x coordinate -1 yeb2a howa 3ada 5alas
        //    {
          //  obstacles.erase(obstacles.begin()); // fa ne3melo erase
        //}

        // Spawn obstacles kol shewaya
        frameCounter++;
        if (frameCounter >= obstacleSpawnTime) // law el frame counter akbar yeb2a it is time to spawn a new obstacle
            {
            frameCounter = 0;
            Obstacle newObstacle = {1.0, groundLevel, 0.1, 0.2, false};
            obstacles.push_back(newObstacle);
        }

        // Check el collisions
        for (size_t i = 0; i < obstacles.size(); ++i) {
            if (posX < obstacles[i].x + obstacles[i].width && posX + 0.1 > obstacles[i].x &&
                posY < obstacles[i].y + obstacles[i].height && posY + 0.4 > obstacles[i].y) {
                isGameOver = true;
                std::cout << "Game Over!" << std::endl;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // bet8ayar el frames 60 fps
}




void keyboard(unsigned char key, int x, int y) {
    switch (currentGameState) {

        case GAME:
            if (isGameOver) return;

            switch (key) {
                    case ' ':
                    if (!isJumping) {
                        isJumping = true;
                        velocityY = jumpStrength;
                    }
            }
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mouseX = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
        float mouseY = 1.0f - (float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

        if (currentGameState == MENU) {
            if (mouseX > startButton.x && mouseX < startButton.x + startButton.width &&
                mouseY > startButton.y - startButton.height && mouseY < startButton.y) {
                currentGameState = GAME;
            }
            if (mouseX > exitButton.x && mouseX < exitButton.x + exitButton.width &&
                mouseY > exitButton.y - exitButton.height && mouseY < exitButton.y) {
                exit(0);
            }
        }
    }
}


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Space Escape");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    srand(time(0)); // Seed for random number generation
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
     glutMouseFunc(mouse);
    glutTimerFunc(0, update, 0);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
