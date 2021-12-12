#include <iostream>
#include <string>
#include <SDL.h>
#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <fstream>
#include <stdio.h>

using namespace std;

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

string LoadShader(string fileName);

int main(int argc = 0, char* argv[] = nullptr)
{
    //  argc is an int counting how many arguments are given when running the program in command line
    //  argv is an array of strings (or array of arrays of char) that represent each of the informations I want to pass on to my program for its execution
    //  if I have a program that requires that I give 2 arguments to multiply one number by another I would call it like this: ./myMultiplier.exe 4 6

    //	Handle args
    if (argc > 0) {
        for (int i = 0; i < argc; i++) {
            cout << argv[i] << endl;
        }
    }

    #pragma region Setting up OpenGL + Glew
    //  Setting up OpenGL + glew ======================
    SDL_Window* window = nullptr;
    SDL_GLContext context;
    int flags = SDL_WINDOW_OPENGL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "DSL failed to initialize" << endl;
        return 1;
    }

    //	Initialize window and openGL
    window = SDL_CreateWindow("Computer Graphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
    //  Create an OpenGL compatible context to let glew draw on it
    context = SDL_GL_CreateContext(window);

    //  Setting up OpenGL with glew
    glewExperimental = GL_TRUE;
    if (glewInit() == GLEW_OK) {
        cout << "Glew initialized successfully" << endl;
    }
    //  ===============================================  
    #pragma endregion

    //  Get infos 
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGl version supported: " << version << endl;

    //  Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    //  Set viewport and clear color
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(1 - 0.245f, 1 - 0.242f, 1 - 0.241f, 1.0f); // Changing background color

    #pragma region Load points & vertices

    GLfloat points[] = { // List of points
        // First triangle:
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        // Second triangle:
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    // GLfloat:
    // OpenGL can work with a lot of different languages and has to do a lot of abstraction to maintain consistency and be sure of the informations it is receiving.
    // things like GLuint, GLenum, GLboolean, GLsizei, ...

    GLfloat vertices[] = {
        // positions            // colors
        -1.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f,

        -0.75f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.0f, 1.0f, 0.0f,
        0.0f, 0.25f, 0.0f,       0.0f, 0.0f, 1.0f
    };
    #pragma endregion

    #pragma region VBO / VAO

    GLuint vbo = 0; // Vertex Buffer Object
        // it is a way to store data before sending it to the GPU
        // Create an ID to be given at object generation
    glGenBuffers(1, &vbo); // Pass how many buffers should be created and the reference of the ID to get the value set
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint vao = 0; // Vertex Attribute Object
        // it remembers all the vertex buffers we want to use and their memory layout
        // Create an ID to be given at object generation
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // vao to use next
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //                       ^ to cut the string at the right place
    //                         for each vertex, we have 3 "cases", so we need to cut the string every 3 "cases"
    // glVertexAttribPointer(
    //      position of the attribute -> here, position is the only attribute so its in position 0,
    //      the number of informations to compose this attribute -> here, 3 coordinates,
    //      the type of OpenGL data it will use,
    //      should I normalize the data ? -> if inputed an int, it would convert it to a float 0 to 1,
    //      the size (in bits) of the data in one vertex,
    //      the offset (with a weird void* cast)
    //      );

    GLuint vao_02 = 0;
    glGenVertexArrays(1, &vao_02);
    glBindVertexArray(vao_02);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Extras =====================================================

    GLuint vbo_02 = 0;
    glGenBuffers(1, &vbo_02);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_02);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vao_03 = 0;
    glGenVertexArrays(1, &vao_03);
    glBindVertexArray(vao_03);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_02);

    // ============================================================

    #pragma endregion 
    #pragma region Shaders

    #pragma region Not in file
    // Vertex Shaders -> Color of the vertex
    // handles the computation regarding geometric transformation, vertices attributesand global lighting
    //const char* vertex_shader = // shader the points' positions of the mesh
    //    "#version 330\n"
    //    "in vec3 vp;"
    //    "void main () {"
    //    "  gl_Position = vec4 (vp.x, vp.y, vp.z, 1.0);" // Transform the triangle
    //    "}\0";                                //  ^ = w -> 1.0 means that it is homogeneous

    ////  Fragment Shaders (or pixels-shaders) -> Color of the pixel
    ////  handles computation of the pixels inside the primitive
    //const char* fragment_shader_01 = // computes the pixels
    //    "#version 330\n"
    //    "out vec4 color;"
    //    "void main () {"
    //    "  color = vec4 (0.215, 0.077, 0.036, 1.0);" // Triangle color [RGBA]
    //    "}\0"; // \0 allows to unspecialize, meaning that the characters string ended

    //const char* fragment_shader_02 = 
    //    "#version 330\n"
    //    "out vec4 color;"
    //    "void main () {"
    //    "  color = vec4 (0.155, 0.057, 0.028, 1.0);" 
    //    "}\0";  
    #pragma endregion
    // Final version - in file:
    string vshader_01 = LoadShader("vertShad_01.txt");
    const char* vert_shader = vshader_01.c_str();
    string fshader_01 = LoadShader("fragShad_01.txt");
    const char* frag_shader = fshader_01.c_str();
    string fshader_02 = LoadShader("fragShad_02.txt");
    const char* frag_shader_02 = fshader_02.c_str();

    string fshader_03 = LoadShader("fragShad_03.txt");
    const char* frag_shader_03 = fshader_03.c_str();

    #pragma endregion 
    #pragma region Compile & stock shaders in [PROGRAM]
    // Compile the shaders =======================================
    GLuint vs = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vs, 1, &vert_shader, NULL); // Now that we have a vertex shader, let’s put the code text inside
    glCompileShader(vs); // And compile
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag_shader, NULL);
    glCompileShader(fs);
    GLuint fs_02 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_02, 1, &frag_shader_02, NULL);
    glCompileShader(fs_02);

    GLuint fs_03 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_03, 1, &frag_shader_03, NULL);
    glCompileShader(fs_03);
    // ===========================================================

    // Put the compiled shaders into a single [PROGRAM] v
    GLuint shaderProgram = glCreateProgram();
    // Now attach shaders to use the program
    glAttachShader(shaderProgram, vs); 
    glAttachShader(shaderProgram, fs);
    // And link it
    glLinkProgram(shaderProgram);

    GLuint shaderProgram_02 = glCreateProgram();
    glAttachShader(shaderProgram_02, fs_02);
    glAttachShader(shaderProgram_02, vs);
    glLinkProgram(shaderProgram_02);

    // Extras =====================================================

    GLuint shaderProgram_03 = glCreateProgram();
    glAttachShader(shaderProgram_03, fs_03);
    glAttachShader(shaderProgram_03, vs);
    glLinkProgram(shaderProgram_03);

    // ============================================================
    #pragma endregion
    #pragma region Shaders modifiers

    // Shaders modifiers ==========================================

    float timeValue;
    float redColor;

    //  Translation & Rotation & Scaling ======
    float spaceInBetween = 2.0f;
    float rotGlobalPower = 2.0f;
    float scaleGlobalPower = 4.0f;
    float trembling = 10.0f;
    float localRotation = 90.0f;
    float xScaling = 0.5f;
    float yScaling = 0.5f;

    //  Values to enter
    cout << "\nSpace in between: "; cin >> spaceInBetween;
    cout << "\nRotation power (global): "; cin >> rotGlobalPower;
    cout << "\nScale power (global): "; cin >> scaleGlobalPower;
    cout << "\nTrembling: "; cin >> trembling;
    cout << "\nRotation power (local): "; cin >> localRotation;
    cout << "\nScaling (x - axis): "; cin >> xScaling;
    cout << "\nScaling (y - axis): "; cin >> yScaling;

    //float posModifier = (sin(timeValue) / 2.0f) + 0.5f;
    float posModifier;
    float rotModifier;
    float scaleModifier;

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);  //  Identity matrix

    //  Getting shaders attributes ============
    unsigned int vertexColorLocation = glGetUniformLocation(shaderProgram_03, "ourColor");
    unsigned int vertexPosModifier = glGetUniformLocation(shaderProgram_03, "posModifier");
    unsigned int transformLoc = glGetUniformLocation(shaderProgram_03, "transform");

    // ============================================================  

    #pragma endregion

    // Game loop ========================================
    
    bool isRunning = true;
    
    while (isRunning) {

        // Inputs ===========================================
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            default:
                break;
            }
        }
        // Update ===========================================

        timeValue = (float)SDL_GetTicks() / 1000;
        redColor = (sin(timeValue) / 2.0f) + 0.5f;

        //posModifier = (sin(timeValue) / 2.0f) + 0.5f;
        posModifier = (sin(timeValue) / spaceInBetween) + 0.5f;
        rotModifier = (sin(timeValue) / rotGlobalPower) + 0.5f;
        scaleModifier = (sin(timeValue) / scaleGlobalPower) + 0.5f;

        // Tests - glm 

        trans = glm::mat4(1.0f);  //  Identity matrix

        //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
        //vec = trans * vec;
        trans = glm::rotate(trans, glm::radians(((sin(rotModifier * trembling) / 2.0f) + 0.5f) * localRotation), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(xScaling, yScaling, 0.5));

        // Draw =============================================
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        // Draw here ========================================
        //glUseProgram(shaderProgram); // Use the [PROGRAM] ^
        //glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3); // (GL_SHAPE, firstPoint, number of points [to draw])
        //    //                        ^ 
        //    // GL_TRIANGLES: draw a [filled] triangle 
        //    // + GL_TRIANGLE_STRIP | GL_TRIANGLE_FAN
        //    // GL_LINES_LOOP: draws the entire loop
        //    // GL_LINE_STRIP: draws the outline of the filled area (the last point will not connect to the first)
        //    // GL_LINES: draws lines
        //    // GL_POINTS: draws points

        //glUseProgram(shaderProgram_02);
        //glBindVertexArray(vao_02);
        //glDrawArrays(GL_TRIANGLES, 3, 3);

        vertexColorLocation = glGetUniformLocation(shaderProgram_03, "ourColor");
        vertexPosModifier = glGetUniformLocation(shaderProgram_03, "posModifier");
        transformLoc = glGetUniformLocation(shaderProgram_03, "transform");

        glUseProgram(shaderProgram_03);
        glUniform4f(vertexColorLocation, redColor, 0.4f, 0.4f, 1.0f);
        glUniform3f(vertexPosModifier, posModifier, -1.0f, 0.0f);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(vao_03);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUniform3f(vertexPosModifier, 0.0f, posModifier -1, 0.0f);
                     
        glUseProgram(shaderProgram_03);
        glUniform4f(vertexColorLocation, redColor, 0.2f, 0.2f, 1.0f);
        glBindVertexArray(vao_03);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        redColor = (cos(timeValue) / 2.0f) + 0.5f;
        glUseProgram(shaderProgram_03);
        glUniform4f(vertexColorLocation, redColor, 0.2f, 0.2f, 1.0f);
        glBindVertexArray(vao_03);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        SDL_GL_SwapWindow(window); // Swapbuffer
    }

    // Quit =============================================
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);

    return 0;
}

string LoadShader(string fileName) {
    string line;
    string fileContent;

    ifstream myFile;
    myFile.open(fileName);

    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            fileContent += line + '\n';
        }
        myFile.close();
    }
    else {
        cerr << "Error - failed to open " << fileName << endl;
    }

    //cout << fileContent;
    // cout << "c style : \n";
    // const char* file_shader = fileContent.c_str();
    return fileContent;
}