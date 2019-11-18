#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;

class DrawingObject
{
public:
    float x, y, size;
    DrawingObject(float xc, float yc, float sizeVal)
    {
        x=xc;
        y=yc;
        size=sizeVal;
    }
    float getInitX(){
        return x;
    }
};
class mnll{
public:
  DrawingObject *node;
  bool isHouse;
  mnll *next;
  mnll(){
    isHouse= false;
      next=NULL;
  }
};

mnll *head =NULL, *rear=NULL;



class house : public DrawingObject
{
public:
    house(float xc, float yc, float sizeVal) : DrawingObject(xc,yc,sizeVal)
    {
        memset(vrtc, 0x0, sizeof(vrtc)*15);
        vrtc[0]=vrtc[9]=x;
        vrtc[1]=vrtc[4]=y;
        vrtc[10]=vrtc[7]=y+size/2;
        vrtc[3]=vrtc[6]=x+size;
        vrtc[12]=x+(size/2);
        vrtc[13]=vrtc[9]+5;
    }
    void posUpdate()
    {
        int i;
        for(i=0;i<15;i=i+2)
        {
            vrtc[i]--;
        }
    }
    void drawHouse()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(1.2);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vrtc);
        glDrawArrays(GL_POLYGON, 0, 4);
        glDrawArrays(GL_TRIANGLES, 2, 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
  /*  ~house(){
        delete[] vrtc;
    }*/
protected:
    
private:
    float* vrtc = new float[15];
    
};

class tree : public DrawingObject
{
public:
    tree(float xc, float yc, float sizeVal): DrawingObject(xc,yc,sizeVal) {
        y+=40;
        memset(vrtc, 0x0, sizeof(vrtc)*22);
        memset(trunk, 0x0, sizeof(vrtc)*4);
        vrtc[0]=x;
        vrtc[1]=trunk[3]=vrtc[3]=y;
        vrtc[2]=x+size;
        vrtc[5]=vrtc[7]=vrtc[19]=vrtc[21]=y+30;
        vrtc[9]=vrtc[11]=vrtc[15]=vrtc[17]=vrtc[5]+30;
        vrtc[2]=x+size;
        vrtc[4]=x+(size*3/4);
        vrtc[6]=vrtc[4]+15;
        vrtc[20]=x+(size/4);
        vrtc[18]=vrtc[20]-15;
        float base2=vrtc[6]-vrtc[18];
        vrtc[16]=vrtc[18]+(base2/4);
        vrtc[14]=vrtc[16]-15;
        vrtc[8]=vrtc[18]+(base2*3/4);
        vrtc[10]=vrtc[8]+15;
        vrtc[12]=x+(size/2);
        vrtc[13]=vrtc[9]+30;
        trunk[0]=trunk[2]=vrtc[12];
        trunk[1]=y-40;
    }
    void posUpdate(){
        int i;
        for(i=0;i<22;i=i+2)
        {
            vrtc[i]--;
        }
        trunk[0]--;
        trunk[2]--;
    }
    
    void drawTree()
    {
        glLineWidth(1.2);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vrtc);
        glDrawArrays(GL_POLYGON, 0, 11);
        glDisableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vrtc);
        glDrawArrays(GL_POLYGON, 0, 11);
        glDisableClientState(GL_VERTEX_ARRAY);
        glBegin(GL_LINES);
         glLineWidth(2);
            glVertex2f(trunk[0], trunk[1]);
            glVertex2f(trunk[2],trunk[3]);
        glEnd();


    }
   /* ~tree(){
        delete[] vrtc;
        delete[] trunk;
    }*/
    
protected:
    
private:
    float* vrtc = new float[22];
    float* trunk = new float[4];
    
};

void generateObject(float x){
    int n;
    mnll *tptr=new mnll;
    
    
    n=rand();
    if(n%2==0)
    {
        house h1(x,300,100);
        tptr->isHouse=true;
        tptr->node=&h1;
    }
    else{
        tree t1(x,300,100);
        tptr->node=&t1;
    }
    if(rear==NULL)
    {
        rear=tptr;
        head=rear;
    }
    else{
        rear->next=tptr;
        rear=tptr;
    }
    
    
    
}

int main()
{
    glfwInit();
    float x=WIDTH;
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "walking men", nullptr, nullptr);
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
   // house h1 = house(400,300,100);
    //tree t1 = tree(300,300,75);
    if(nullptr == window)
    {
        std::cout << "failed to create window \n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_FALSE;
    if(GLEW_OK != glewInit())
    {
        std::cout<<"failed to init glew"<<std::endl;
        return -1;
    }
    glViewport(0, 0, screenWidth, screenHeight);
    glOrtho( 0, screenWidth, 0, screenHeight, 0, 1 );

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window))
    {
        mnll *tptr=head;
        glfwPollEvents();
        glColor3f(0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        generateObject(x);
       // h1.drawHouse();
       // t1.drawTree();
        glBegin(GL_LINES);
         glLineWidth(2);
            glVertex2f(0,300);
            glVertex2f(800,300);
            glVertex2f(0,220);
            glVertex2f(800,220);
            
        glEnd();
        while(tptr!=NULL)
        {
            if(tptr->node->getInitX()<WIDTH)
            {
                if(tptr->isHouse)
                {
                    house *h1=(house*)tptr->node;
                    h1->drawHouse();
                    h1->posUpdate();
                }
                else{
                    tree *t1=(tree*)tptr->node;
                    t1->drawTree();
                    t1->posUpdate();
                }
            }
            tptr=tptr->next;
        }
        usleep(10000);
        glfwSwapBuffers(window);
        
        x+=100;
    }
    glfwTerminate();
    return 0;
}

