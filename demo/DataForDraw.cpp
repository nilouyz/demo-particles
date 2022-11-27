#include "DataForDraw.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

float DataForDraw::PI = 3.141592653f;

DataForDraw::DataForDraw(Color color, Pos pos, float height, float radius, int sp, float rate)
{
    this->color = color;
    this->pos = pos;
    this->rate = rate;
    this->height = height;
    this->radius = radius;
    this->sp = sp;
    live = true;
}


DataForDraw::~DataForDraw(){
    glDeleteBuffers(1, &VBO);
}

void DataForDraw::initVertexData()
{
    this->vcount = 1;
    vdata.resize(this->vcount * 8);
    // s: 0.0|  height/vh|    10.0|       10.6 |              20.0|         20.6|                    |   
    //       |        fly|  split | first-child|first-chd disable | second-child|second-child disable|
    // x y | r g b | s | vx vy 
    vdata[0]= 0.0;
    vdata[1]= -1.0;
    vdata[2]= color.r;
    vdata[3]= color.g;
    vdata[4]= color.b;
    vdata[5]= 0.0;
}

void DataForDraw::updateVetexData()
{
    float vh = 4.0f;
    float g = -10.0f;
    float dt = 0.005f;
    vector<float> updatedata;
    for(int i=0; i<vdata.size()/8; i++)
    {
        // get state
        float d = vdata[i*8+5];
        if(d < this->height/vh)
        {
            //fly state
            vh = vh + g*dt;
            vdata[i*8+1] = vdata[i*8+1] + vh*dt;
            vdata[i*8+5] += dt;
            updatedata.push_back(vdata[i*8]);     
            updatedata.push_back(vdata[i*8+1]);     
            updatedata.push_back(vdata[i*8+2]);     
            updatedata.push_back(vdata[i*8+3]);     
            updatedata.push_back(vdata[i*8+4]);     
            updatedata.push_back(vdata[i*8+5]);     
            updatedata.push_back(vdata[i*8+6]);     
            updatedata.push_back(vdata[i*8+7]);
            this->vcount = 1;
        } else if(d<10) {
            // split state
            vcount=0;
            for(int j=0;j<sp;j++)
            {
                float angle = 2.0*PI/sp *j;
                float va = radius*0.6 + radius*1.6*rand()/float(RAND_MAX);
                float vx = va*cosf(angle);
                float vy = va*sinf(angle);
                updatedata.push_back(vdata[i*8]);     
                updatedata.push_back(vdata[i*8+1]);     
                updatedata.push_back(color.r);     
                updatedata.push_back(color.g);    
                updatedata.push_back(color.b);     
                updatedata.push_back(10.1f);    
                updatedata.push_back(vx);
                updatedata.push_back(vy);
                this->vcount++;
                // printf("split %f %f %f\n", angle, vx, vy);
            }
            printf("after split has:%d\n", this->vcount);
        } else if (d<10.6f){
            // child fly
            float seed = rand()/(float)(RAND_MAX);
            if(seed>rate || d < 10.4) {
                float currx=vdata[i*8+6]*dt+vdata[i*8];
                float curry=vdata[i*8+7]*dt+vdata[i*8+1];
                updatedata.push_back(currx);     
                updatedata.push_back(curry);     
                updatedata.push_back(color.r);     
                updatedata.push_back(color.g);    
                updatedata.push_back(color.b);     
                updatedata.push_back(vdata[i*8+5]+dt);    
                updatedata.push_back(vdata[i*8+6]);
                updatedata.push_back(vdata[i*8+7]+g*dt*0.2);        
            } else {
                // split next child
                this->vcount--;
                for(int k=0; k<18; k++) {
                    float currx=vdata[i*8+6]*dt+vdata[i*8];
                    float curry=vdata[i*8+7]*dt+vdata[i*8+1];
                    updatedata.push_back(currx);     
                    updatedata.push_back(curry);     
                    updatedata.push_back(color.r);     
                    updatedata.push_back(color.g);    
                    updatedata.push_back(color.b);     
                    updatedata.push_back(20.1f);    

                    float angle = 2.0*PI/18.0 *k;
                    float vchild = radius * rand()/(float)(RAND_MAX);
                    updatedata.push_back(vdata[i*8+6]+vchild * cosf(angle));
                    updatedata.push_back(vdata[i*8+7]+vchild * sinf(angle));    
                    this->vcount ++;
                }
            }
        } else if (d < 20) {
            this->vcount --;
            if(this->vcount == 0) {
                live = false;
            }
        } else if (d < 20.5f) {
                float currx=vdata[i*8+6]*dt+vdata[i*8];
                float curry=vdata[i*8+7]*dt+vdata[i*8+1];
                updatedata.push_back(currx);     
                updatedata.push_back(curry);     
                updatedata.push_back(color.r);     
                updatedata.push_back(color.g);    
                updatedata.push_back(color.b);     
                updatedata.push_back(vdata[i*8+5]+dt);    
                updatedata.push_back(vdata[i*8+6]);
                updatedata.push_back(vdata[i*8+7]+g*dt);   
        } else {
            this->vcount --;
            if(this->vcount == 0) {
                live = false;
            }
        }
    }
    vdata.resize(updatedata.size());
    for(unsigned int i=0;i<vdata.size();i++)
    {
        vdata[i]=updatedata[i];
    }    
}

void DataForDraw::bindForDraw()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vdata.size()*sizeof(float), vdata.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}




