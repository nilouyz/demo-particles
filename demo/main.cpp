#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "DataForDraw.h"
#include "Shader.h"
#include <deque>
#include <set>
using namespace std;


unsigned int getFreeVBO(std::set<unsigned int> pool)
{
    if(pool.size()==0) {
        return -1;
    } else {
        unsigned int VBO = *pool.begin();
        pool.erase(VBO);
        return VBO;
    }
}

float getRand()
{
    return rand()/(float)RAND_MAX;
}

Color getRandColor()
{
    return Color(0.2+getRand(),0.2+getRand(),0.2+getRand());
}

DataForDraw * buildFireWorks(std::set<unsigned int> pool)
{
    unsigned int VBO = getFreeVBO(pool);
    if(VBO==-1)
    {
        return nullptr;
    }
    Color c= getRandColor();
    Pos p(2.0*getRand()-1.0, -1.0, 0.0);
    DataForDraw *obj = new DataForDraw(c,p,0.5+1.5*getRand(), 0.4+0.6*getRand(), 320, 0.1*getRand());
    obj->VBO = VBO;
    obj->initVertexData();
    return obj;
}

int main(int argc, char const *argv[])
{
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(800,800, "win", 0, 0);

    glfwMakeContextCurrent(window);
    glewInit();
    glViewport(0,0,800,800);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glfwSwapInterval(1);

    std::set<unsigned int> VBOPool;
    unsigned int VBO[3];
    glGenBuffers(3,VBO);
    VBOPool.insert(VBO[0]);
    VBOPool.insert(VBO[1]);
    VBOPool.insert(VBO[2]);    

    
    std::deque<DataForDraw *> fireWorksPool; 
    DataForDraw *obj = buildFireWorks(VBOPool);
    fireWorksPool.push_back(obj);
    

    Shader ourShader("../demo/a.vs","../demo/a.fs");
    ourShader.use();

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f,0.0f,0.0f,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        for(auto it = fireWorksPool.begin(); it!=fireWorksPool.end();) {
            DataForDraw *item = *it;
            if(item->live)
            {
                item->updateVetexData();
                item->bindForDraw();
                glDrawArrays(GL_POINTS, 0, item->vcount);
                it++;
            } else {
                delete item;
                fireWorksPool.erase(it);
            }
        }

        if(fireWorksPool.empty())
        {
            DataForDraw *newItem = buildFireWorks(VBOPool);
            if(newItem)
            {
                fireWorksPool.push_back(newItem);
            }
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }
    glDeleteBuffers(3, VBO);
    
    return 0;
}
