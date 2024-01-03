#include <iostream>
#include <vector> // Include the header for the vector container
#include <stack>// Include the header for the stack container
#include <array>
#include <GL/glut.h>// Include the GLUT library for OpenGL
#include <windows.h>// Include the Windows header for Windows-specific functionality
#include <sstream>// Include the header for string stream operations


int numDisks = 3;  // Define the number of disks in the Tower of Hanoi

// Create a vector of vectors to represent the three rods in the Tower of Hanoi.
// Each rod is represented by a vector containing pairs of int and array of floats.
// The int represents the disk size, and the array of floats represents the color.
std::vector<std::vector<std::pair<int, std::array<float, 3>>>> rods(3); // Pair of position and color

bool isGameCompleted() {
    return rods[2].size() == numDisks; // Assuming the destination rod is rods[2]
}
std::stack<std::pair<int, int>> moveHistory;// Create a stack to keep track of the move history, where each move is represented as a pair of integers.
bool isMoveInProgress = false; // Flag to indicate if a move is in progress
int moveCount = 0;             // Counter for the number of moves

// Function to draw a disk with a specified color
void drawDisk(float x, float y, float radius, float red, float green, float blue) {
    glColor3f(red, green, blue); // Set the color of the disk using RGB values.

    glBegin(GL_POLYGON);  // Begin drawing a filled polygon for the disk.
    // Iterate over angles to create a circular shape for the disk.
    for (int i = 0; i < 360; i += 5) {
        float angle = i * 3.14159265 / 180.0; // Convert the angle from degrees to radians.
        // Calculate the position of each vertex of the disk.
        float x_i = x + radius * cos(angle);
        float y_i = y + radius * sin(angle);

        glVertex2f(x_i, y_i);   // Specify the vertex position.
    }
    glEnd();   // End drawing the polygon.
}


// Function to draw the Tower of Hanoi with specified colors for rods and plates
void drawTower() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer to prepare for drawing.
    glColor3f(1.0, 1.0, 1.0);  // Set the color to white for text and other elements.

    // write  "Tower of Hanoi" at the top of screen
    glColor3f(1.0, 1.0, 1.0); //setting color
    glRasterPos2f(0.3, 0.95);   //set the current raster position in the window coordinate system. 0.3 is the x-coordinate & 0.95 is the y-coordinate 
    std::string towerOfHanoi = "Tower of Hanoi";
    for (char c : towerOfHanoi)// loop that iterates over each character in the towerOfHanoi string and draws each character on the screen using the glutBitmapCharacter function.
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw Rabia Mukhtar -221834  Jasia Mubashir - 221831
    glRasterPos2f(0.1, 0.02);
    std::string madeBy = "Made By:  221834 Rabia Mukhtar & 221831 Jasia Mubashir ";
    for (char c : madeBy) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c); // renders a bitmap character on the screen.
    }

    // Draw the rods
    glColor3f(0.0, 1.0, 0.0); // Green tower, you can change the color here
    glBegin(GL_LINES);
    for (int i = 0; i < 3; ++i) // t draws three vertical lines to represent the three towers in the Tower of Hanoi
    {  //tower0,1,2
        glVertex2f(0.1 + i * 0.3, 0.1);  //define the startingand ending points of vertical lines representing the towers.
        glVertex2f(0.1 + i * 0.3, 0.8);
    }
    glEnd();

    // Draw the disks with different colors
    float diskWidth = 0.04;
    // Iterate over rods and disks to draw them.
    for (int i = 0; i < 3; ++i) {
        for (size_t j = 0; j < rods[i].size(); ++j) {
            // Calculate position and size of each disk.
            float x = 0.2 + i * 0.3;
            float y = 0.2 + j * 0.05;
            float radius = 0.02 + rods[i][j].first * 0.02;
            float color[3];
            // Copy color information from the data structure.
            std::copy(std::begin(rods[i][j].second), std::end(rods[i][j].second), color);
            // Call the drawDisk function to draw the disk.
            drawDisk(x, y, radius, color[0], color[1], color[2]);
        }
    }

    // Display move count
    std::ostringstream oss;
    oss << "Moves: " << moveCount;
    std::string moveString = oss.str();

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(0.1, 0.9);
    // Draw move count character by character.
    for (char c : moveString) //The purpose of this loop is to draw each character of the moveString on the screen using the glutBitmapCharacter function
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);// Specifies the font and size and The character to be rendered on the screen.
    }

    glFlush();
}

// Function to move a disk from one rod to another
void moveDisk(int from, int to) {
    // Check if the source rod is not empty and the destination rod is either empty or has a larger disk on top.
    if (!rods[from].empty() && (rods[to].empty() || rods[from].back().first < rods[to].back().first)) {
        moveHistory.push({ from, to }); // Record the move in the history stack.
        auto disk = rods[from].back();  // Get the disk from the top of the source rod
        rods[to].push_back(disk);  // Place the disk on the top of the destination rod.
        rods[from].pop_back();  // Remove the disk from the source rod.
        moveCount++;  // Increment the move count.

        // Check if the maximum move limit is reached.
        if (moveCount >= 100) {
            std::cout << "Game over! You've reached the maximum number of moves." << std::endl;
            exit(0); // Close the game
        }
    }
}


void undoMove() {
    if (!moveHistory.empty()) {
        std::pair<int, int> lastMove = moveHistory.top();  // Get the last recorded move.
        moveHistory.pop();   // Remove the last move from the history stack.
        auto disk = rods[lastMove.second].back();    // Get the disk from the top of the destination rod.
        rods[lastMove.first].push_back(disk);   // Place the disk back on the source rod
        rods[lastMove.second].pop_back();  // Remove the disk from the destination rod.
        moveCount--;    // Decrement the move count.
        glutPostRedisplay(); // Redraw the Tower of Hanoi after undoing the move.
    }
}

// Function to handle the Tower of Hanoi animation
void animateMoves() {
    // Check if there is a recorded move in the history stack.
    if (!moveHistory.empty()) {
        std::pair<int, int> lastMove = moveHistory.top(); // Get the last recorded move.
        moveHistory.pop();   // Remove the last move from the history stack.
        moveDisk(lastMove.second, lastMove.first); // Perform the move in reverse direction.
        drawTower(); // Draw the updated Tower of Hanoi

        // Check if the game is completed by examining the destination rod.
        if (rods[2].size() == numDisks) {
            drawTower(); // Redraw the Tower of Hanoi with the final state of disks

            while (!moveHistory.empty()) {
                std::pair<int, int> remainingMove = moveHistory.top(); // Get the remaining moves
                moveHistory.pop(); // Remove the remaining move from the history stack
                moveDisk(remainingMove.second, remainingMove.first); // Perform the remaining moves
                drawTower(); // Draw the updated Tower of Hanoi
            }

            std::ostringstream oss;  //represents an output string stream.  used for strings output
            oss << "Congratulations! You've completed the Tower of Hanoi in " << moveCount << " moves.";
            std::string completionMessage = oss.str();

            glColor3f(1.0, 1.0, 1.0);
            glRasterPos2f(0.1, 0.85);
            for (char c : completionMessage) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
            }

            glFlush();
            Sleep(3000); // Sleep for 3 seconds before exiting
            exit(0);     // Close the game
        }
    }
}

// New function to handle moves based on source and destination selection
void makeMove(int sourceRod, int destinationRod) {
    if (!isMoveInProgress) {
        // Your existing moveDisk function logic goes here
        if (!rods[sourceRod].empty() && (rods[destinationRod].empty() || rods[sourceRod].back().first < rods[destinationRod].back().first)) {
            moveHistory.push({ sourceRod, destinationRod });
            auto disk = rods[sourceRod].back();
            rods[destinationRod].push_back(disk);
            rods[sourceRod].pop_back();
            moveCount++;

            // Check for the maximum move limit
            if (moveCount >= 100) {
                std::cout << "Game over! You've reached the maximum number of moves." << std::endl;
                exit(0);
            }
        }

        isMoveInProgress = false;
        glutPostRedisplay();

        //Check if the game is completed

    }
}


// Keyboard callback function for user-friendly controls
void keyboardFunc(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '3') {
        static int sourceRod = -1; // Initially set to an invalid value
        if (sourceRod == -1) {
            sourceRod = key - '1'; // Convert char to integer index
        }
        else {
            int destinationRod = key - '1';
            makeMove(sourceRod, destinationRod);
            sourceRod = -1; // Reset the source rod selection
        }
    }
    else if (key == 'u') {
        undoMove(); // Call the undoMove function when 'u' key is pressed
    }
}



// Idle callback function

//This function is called when the program is idle, and it is responsible for
// initiating the Tower of Hanoi animation if a move is currently in progress. After the
// animation is performed, it resets the move in progress flag and posts a redisplay to update
// the graphics.
void idleFunc() {
    if (isMoveInProgress)   // Check if a move is in progress.
    {
        animateMoves();// If a move is in progress, initiate the animation
        isMoveInProgress = false; // Reset the move in progress flag.
        glutPostRedisplay(); // Post a redisplay to update the graphics.
    }
    if (isGameCompleted()) {
        std::ostringstream oss;
        oss << "Congratulations! You've completed the Tower of Hanoi in " << moveCount << " moves.";
        std::string completionMessage = oss.str();

        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(0.1, 0.85);
        for (char c : completionMessage) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }

        glFlush();
        Sleep(3000);
        exit(0);
    }
}

// OpenGL initialization
//This function initializes the OpenGL environment
void initialize() {
    glClearColor(0.0, 0.0, 0.2, 1.0); // set the color
    glMatrixMode(GL_PROJECTION);  // Set the current matrix mode to projection.
    glLoadIdentity(); // Load the identity matrix to reset any previous transformations.
    gluOrtho2D(0.0, 1.0, 0.0, 1.0); // Set the orthographic projection with the specified limits
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);   // Initialize GLUT with command line arguments.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // Set display mode with single buffer and RGB color.
    glutInitWindowSize(600, 600);  // Set initial window size to 600x600 pixels.
    glutInitWindowPosition(100, 100);  // Set initial window position on the screen.
    glutCreateWindow("Tower of Hanoi");   // Create the main window with the specified title.

    initialize();  // Initialize the OpenGL environment

    // Initialize the Tower of Hanoi with disks on the first rod
    for (int i = numDisks; i > 0; --i) {
        std::array<float, 3> color;
        switch (i) {
        case 1:
            color = { 1.0, 0.0, 0.0 }; // Red
            break;
        case 2:
            color = { 0.0, 1.0, 0.0 }; // Green
            break;
        case 3:
            color = { 0.0, 0.0, 1.0 }; // Blue
            break;
        }
        rods[0].emplace_back(i, color);  //is used to add a new disk to the first rod (rods[0]) in the game.
    }

    glutDisplayFunc(drawTower);  // Set the display callback function.
    glutKeyboardFunc(keyboardFunc);  // Set the keyboard callback function.
    glutIdleFunc(idleFunc);  // Set the idle callback function.

    glutMainLoop(); // Enter the GLUT main event loop.

    return 0;
}
