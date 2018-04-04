#include <GLFW/glfw3.h>

int main(int argc, const char * argv[]) {
    if(!glfwInit()){    //init error
        return -1;
    }

    int width = 1600;    //screen width
    int height = 900;   //screen height

    GLFWwindow* window = glfwCreateWindow(width, height, "Hello OpenGL", NULL, NULL);  //create window

    if(!window){     //create window fail
        glfwTerminate();
        return -1;
    }

    
    unsigned char* pixels = new unsigned char[width * height * 4]; //buffer
    GLuint index;  //temp

    //set pixel
    for (int count_a = 0; count_a < width; count_a++) {
        for (int count_b = 0; count_b < height; count_b++) {
            index = count_b * width + count_a;

            pixels[index * 4 + 0] = 255;    //red pixel
            pixels[index * 4 + 1] = 0;      //green
            pixels[index * 4 + 2] = 0;      //blue
            pixels[index * 4 + 3] = 255;    //alpha
        }
    }

    //set pixel
    for (int count_a = 1000; count_a < width; count_a++) {
        for (int count_b = 600; count_b < height; count_b++) {
            index = count_b * width + count_a;

            pixels[index * 4 + 0] = 0;    //red pixel
            pixels[index * 4 + 1] = 255;      //green
            pixels[index * 4 + 2] = 0;      //blue
            pixels[index * 4 + 3] = 255;    //alpha
        }
    }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {  //close window
        glClearColor(1.0, 1.0, 1.0, 1.0);  //clear screen by white pixel
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2f(-1, -1);   //set origin to screen bottom left
        //glPixelZoom(2, 2); //if i use this function. renderer is right.
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels); //draw pixel

        glfwSwapBuffers(window); //swap buffer
    }
    glfwTerminate();
    return 0;
}