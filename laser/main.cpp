#include <GLFW/glfw3.h>
#include <iostream>
#include "laser.h"
#include "zercalo.h"
#include "target.h"
#include "text.h"
#include <format>
#include "block.h"

#define EW 1000
#define EH 800

Laser laser;
Zercalo zercals;
Target trg;
Block blc;

int currentIdx = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float sensitivity = 0.1f;

    static bool first = true;
    static double lastX = 0.0;
    static double lastY = 0.0;

    if (first) {
        lastX = xpos;
        lastY = ypos;
        first = false;
        return;
    }

    double xoffset = xpos - lastX;
    double yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    laser.angle -= yoffset / 5;

    if (laser.angle < -90.0f) laser.angle = -90.0f;
    if (laser.angle > 90.0f) laser.angle = 90.0f;
}
void processInput(GLFWwindow* window,float dt){
    static bool lastTab = false;
    bool keyTab = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;

    float stepL = 0.5f;
    float stepZ = 1.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (laser.angle + stepL*dt < 90.0f) laser.angle += stepL * dt;
    }      
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (laser.angle - stepL*dt > -90.0f) laser.angle -= stepL * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        zercals.zercala[currentIdx].angle += stepZ * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        zercals.zercala[currentIdx].angle -= stepZ * dt;
    }
    if (keyTab && !lastTab) {
        currentIdx = (currentIdx + 1) % zercals.zercala.size();
        zercals.setSelected(currentIdx);
    }

    lastTab = keyTab;
}
int main(){
    int count = 0;

    if (!glfwInit()){
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(EW, EH, "Laser", NULL, NULL);
    if (!window){
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    BuildFont();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-400, 400, -400, 400, -100, 100); 
    glMatrixMode(GL_MODELVIEW);

    trg.init(zercals.zercala);
    laser.init(-400.0f, 0.0f, 0.0f);
    zercals.init();
    zercals.setSelected(currentIdx);
    blc.init(EH);

    float lastTime = 0.0f;

    glfwSetCursorPosCallback(window, mouse_callback);

    while (!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();   
        float dt = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window,dt);

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        laser.updateDir();                    // обновить направление (если угол меняется)
        laser.update(zercals.zercala, blc.blocks);       // рассчитать отражения
        laser.draw();

        trg.draw();
        zercals.draw();
        blc.draw();

        int size = laser.segments.size() - 1;

        //if (hit) showHitText(dt);

        if (trg.checkCollision(laser.segments[size].a, laser.segments[size].b,trg.position,20.0f)) {
            count += size;

            if (size > laser.record) laser.record = size;

            trg.init(zercals.zercala);
        }

        std::string score = std::format("Score: {}",count);
        std::string rec = std::format("Record: {}", laser.record);
        RenderTextHUD(10, 15,score.c_str(),EW,EH);
        RenderTextHUD(10, 25, rec.c_str(), EW, EH);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
