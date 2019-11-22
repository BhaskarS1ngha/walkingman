#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include<memory>
#include<thread>
#include<chrono>

#define RAND_MAX  500
using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
GLFWwindow* window;


class DrawingObject
{
public:
	float x, y, size;
	DrawingObject(float xc, float yc)
	{
		x = xc;
		y = yc;
		size = 10;

	}
	DrawingObject(float xc, float yc, float sizeVal)
	{
		x = xc;
		y = yc;
		
		
		size = sizeVal;
	}
	float getInitX() {
		return x;
	}
	float getFinX() {
		return x+size;
	}
	~DrawingObject() {

	}
};

class man : public DrawingObject {
public:
	man(float cx=100, float cy=270, float r=5,int  numSegments=10) :DrawingObject(cx, cy) {
		seg = (numSegments) * 2;
		vrtc_head = new float[seg];
		vrtc_body = new float[4];
		vrtc_leg2 = new float[2];
		vrtc_leg1 = new float[2];
		memset(vrtc_head, 0x0, sizeof(vrtc_head) * seg);
		rad = r;
		
		genCoords(x);
		

	}
	void genCoords(float x) {
		vrtc_body[0] = vrtc_body[2] = vrtc_leg1[0] = vrtc_leg2[0] = x;
		vrtc_body[1] = y - rad;
		vrtc_body[3] = vrtc_body[1] - 30;
		vrtc_leg1[1] = vrtc_leg2[1] = 210;
		vrtc_leg1[0] = x - spread;
		vrtc_leg2[0] = x + spread;
		int n = 10 + rand();
		n %= 5;
		if (n < 2)
		{
			n = -2;
		}
		
		Tvector = n;
		
		int j;
		for (int i = 0, j = 0; i < seg; i += 2, j++)
		{
			float theta = 2.0f * 3.1415926f * float(j) / float(seg/2);//get the current angle

			float tx = (rad * cosf(theta));//calculate the x component
			float ty = (rad * sinf(theta));//calculate the y component
			vrtc_head[i] = tx + x;
			vrtc_head[i + 1] = ty + y;
		}

	}
	void set_speed(float s)
	{
		Tvector = s;
	}
	void drawMan()
	{
		glLineWidth(1.2);
		glBegin(GL_LINE_LOOP);
		glLineWidth(2);
		for(int i=0;i<seg;i+=2)
			glVertex2f(vrtc_head[i],vrtc_head[i+1]);
		glEnd();
		glBegin(GL_LINES);
		glLineWidth(2);
		for (int i = 0; i < 4; i += 2)
			glVertex2f(vrtc_body[i], vrtc_body[i + 1]);
		glVertex2f(vrtc_body[2], vrtc_body[3]);
		glVertex2f(vrtc_leg1[0], vrtc_leg1[1]);
		glVertex2f(vrtc_body[2], vrtc_body[3]);
		glVertex2f(vrtc_leg2[0], vrtc_leg2[1]);
		
		glEnd();
	}
	void posUpdate()
	{
		
		//while (!glfwWindowShouldClose(window))
		//{
			if (x > 900||x<-100)
			{
				resetPos();
			}
			else{
				x += Tvector;
				for (int i = 0; i < seg; i += 2)
				{
					vrtc_head[i] += Tvector;
				}
				for (int i = 0; i < 4; i += 2)
				{
					vrtc_body[i] += Tvector;
				}
				leg_update();
			}
			


		//}
	}
	void resetPos()
	{
		x = -15;
		genCoords(x);
		cout << "New speed: " << Tvector << endl;
	}

private:
	const float spread = 10;
	int seg,dir =2,rad;
	float Tvector,delay;
	float *vrtc_head, *vrtc_body, *vrtc_leg1, *vrtc_leg2;
	
	void leg_update()
	{
		
		vrtc_leg1[0] += Tvector;
		vrtc_leg2[0] += Tvector;
		
		if (vrtc_leg1[0] == vrtc_leg2[0] || vrtc_leg1[0]>x+spread||vrtc_leg1[0]<x-spread)
		{
			dir *= -1;
		}
		vrtc_leg1[0] += dir;
		vrtc_leg2[0] -= dir;
		
	}
};



class house : public DrawingObject
{
public:
	house(float xc, float yc, float sizeVal) : DrawingObject(xc, yc, sizeVal)
	{
		memset(vrtc, 0x0, sizeof(vrtc) * 15);
		vrtc[0] = vrtc[9] = x;
		vrtc[1] = vrtc[4] = y;
		vrtc[10] = vrtc[7] = y + size / 2;
		vrtc[3] = vrtc[6] = x + size;
		vrtc[12] = x + (size / 2);
		vrtc[13] = y+size + 5;
	}
	void posUpdate()
	{
		int i;
		x--;
		for (i = 0; i < 15; i = i + 3)
		{
			vrtc[i]--;
		}
	}
	void drawHouse()
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glLineWidth(1.2);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 7; i+=3)
		{
			glVertex3f(vrtc[i], vrtc[i + 1],vrtc[i+2]);
			glVertex3f(vrtc[i + 3], vrtc[i + 4],vrtc[i+5]);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2f(vrtc[6], vrtc[7]);
		glVertex2f(vrtc[9], vrtc[10]);
		glVertex2f(vrtc[12], vrtc[13]);
		glEnd();

	}
	  ~house(){
		  delete[] vrtc;
	  }
protected:

private:
	float* vrtc = new float[15];

};

class tree : public DrawingObject
{
public:
	tree(float xc, float yc, float sizeVal) : DrawingObject(xc, yc, sizeVal) {
		y += 40;
		memset(vrtc, 0x0, sizeof(vrtc) * 22);
		memset(trunk, 0x0, sizeof(vrtc) * 4);
		vrtc[0] = x;
		vrtc[1] = trunk[3] = vrtc[3] = y;
		vrtc[2] = x + size;
		vrtc[5] = vrtc[7] = vrtc[19] = vrtc[21] = y + 30;
		vrtc[9] = vrtc[11] = vrtc[15] = vrtc[17] = vrtc[5] + 30;
		vrtc[2] = x + size;
		vrtc[4] = x + (size * 3 / 4);
		vrtc[6] = vrtc[4] + 15;
		vrtc[20] = x + (size / 4);
		vrtc[18] = vrtc[20] - 15;
		float base2 = vrtc[6] - vrtc[18];
		vrtc[16] = vrtc[18] + (base2 / 4);
		vrtc[14] = vrtc[16] - 15;
		vrtc[8] = vrtc[18] + (base2 * 3 / 4);
		vrtc[10] = vrtc[8] + 15;
		vrtc[12] = x + (size / 2);
		vrtc[13] = vrtc[9] + 30;
		trunk[0] = trunk[2] = vrtc[12];
		trunk[1] = y - 40;
	}
	void posUpdate() {
		int i;
		x--;
		for (i = 0; i < 22; i = i + 2)
		{
			vrtc[i]--;
		}
		trunk[0]--;
		trunk[2]--;
	}

	void drawTree()
	{
		glColor3f(0.0f, 1.0f,0.0f);
		glLineWidth(1.2);
		glBegin(GL_POLYGON);
		for (int i = 0; i <= 18; i+=2)
		{
			glVertex2f(vrtc[i], vrtc[i + 1]);
			glVertex2f(vrtc[i + 2], vrtc[i + 3]);
		}
		glEnd();
	//	cout << "dr2" << endl;
	
		glBegin(GL_LINES);
		glLineWidth(2);
		glVertex2f(trunk[0], trunk[1]);
		glVertex2f(trunk[2], trunk[3]);
		glEnd();


	}
	 ~tree(){
		 delete[] vrtc;
		 delete[] trunk;
	 }

protected:

private:
	float* vrtc = new float[22];
	float* trunk = new float[4];

};

class mnll {
public:
	
	unique_ptr<house> houseNode;
	unique_ptr<tree> treeNode;
	bool isHouse;
	mnll* next;
	mnll() {
		//node = NULL;
		isHouse = false;
		next = NULL;
	}
};
mnll* head = NULL, * rear = NULL;

void generateObject(float x) {
	int n;
	mnll* tptr = new mnll;


	n = rand();
	if (n % 3 >=2)
	{
		cout << "gen house" << endl;
		unique_ptr<house> h1(new house(x, 300, 100));
		tptr->isHouse = true;
		tptr->houseNode= move(h1);
		
		
	}
	else {
		cout << "gen tree" << endl;
		unique_ptr<tree> t1(new tree(x, 300, 100));
		tptr->treeNode = move(t1);
	}
	//cout << tptr-> node->getFinX();
	if (rear == NULL)
	{
		rear = tptr;
		head = rear;
	}
	else {
		rear->next = tptr;
		rear = tptr;
	}



}

void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

		float x = r * cosf(theta);//calculate the x component
		float y = r * sinf(theta);//calculate the y component

		glVertex2f(x + cx, y + cy);//output vertex

	}
	glEnd();
}


int main()
{
	glfwInit();
	float x = 10,cx=10;
	window = glfwCreateWindow(WIDTH, HEIGHT, "walking men", nullptr, nullptr);
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
//	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	man mn[10];
	mn[1].set_speed(0.0f);
	 //house h1 = house(400,300,100);
	// house* hp = &h1;

	// tree t1 = tree(300,300,75);
	if (nullptr == window)
	{
		std::cout << "failed to create window \n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_FALSE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "failed to init glew" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	glOrtho(0, screenWidth, 0, screenHeight, 0, 1);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (x <= 1060)
	{
		generateObject(x);
		x += 150;
	}
	x = 1060;
	while (!glfwWindowShouldClose(window))
	{
		
		
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT);
		mnll* tptr = head;
		

		while (tptr != NULL)
		{
			

				if (tptr->isHouse)
				{

					tptr->houseNode->drawHouse();
					tptr->houseNode->posUpdate();
				}
				else {
					
					tptr->treeNode->drawTree();
					tptr->treeNode->posUpdate();
				}
			
			tptr = tptr->next;
			//cout << "ok" << endl;
		
		}
		glColor3f(0.0f, 0.0f, 0.0f);
		
		
		for (int i = 0; i < 10; i++)
		{
			mn[i].drawMan();
			mn[i].posUpdate();
		}
		
		glBegin(GL_LINES);
		glLineWidth(2);
		glVertex2f(0, 280);
		glVertex2f(800, 280);
		glVertex2f(0, 200);
		glVertex2f(800, 200);

		glEnd();
		
		glfwSwapBuffers(window);
		
		
		if (head->isHouse)
		{
			if (head->houseNode->getFinX() < 0)
			{
			//	cout << "head overflow";
				tptr = head;
				head = head->next;
				delete tptr;
				generateObject(x);
			}
		}
		else
		{
			if (head->treeNode->getFinX() < 0)
			{
				//cout << "head overflow";
				tptr = head;
				head = head->next;
				delete tptr;
				generateObject(x);
			}

		}
		
		this_thread::sleep_for(chrono::milliseconds(41));
	}
	glfwTerminate();
	return 0;
}

