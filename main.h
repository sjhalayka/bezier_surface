#ifndef main_H
#define main_H

#include "uv_camera.h"
#include "primitives.h"
//#include "mesh.h"

#include <cstdlib>
#include <GL/glut.h>       //GLUT Library

#include <opencv2/opencv.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world340.lib")

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;
using std::istringstream;

#include <fstream>
using std::ofstream;
using std::ifstream;

#include <set>
using std::set;

#include <map>
using std::map;

#include <utility>
using std::pair;


int num_wide = 0;
int num_tall = 0;
int num_dims = 0; // xyz

vector<vector<vector<float> > > cpoints;


GLUquadricObj* glu_obj = gluNewQuadric();

vertex_3 background_colour(0.0f, 0.0f, 0.0f);
vertex_3 control_list_colour(1.0f, 1.0f, 1.0f);

float outline_width = 1.0;
static const float outline_colour[] = { 0.0, 0.0, 0.0 };


bool draw_axis = true;
bool draw_control_list = true;

uv_camera main_camera;

GLint win_id = 0;
GLint win_x = 800, win_y = 600;
float camera_w = 10;

float camera_fov = 45;
float camera_x_transform = 0;
float camera_y_transform = 0;
float u_spacer = 0.01;
float v_spacer = 0.5*u_spacer;
float w_spacer = 0.1;
float camera_near = 0.0001;
float camera_far = 100;

bool lmb_down = false;
bool mmb_down = false;
bool rmb_down = false;
int mouse_x = 0;
int mouse_y = 0;


vector<triangle> triangles;
vector<vertex_3> face_normals;
vector<vertex_3> vertices;
vector<vertex_3> vertex_normals;



void idle_func(void)
{
	glutPostRedisplay();
}

void init_opengl(const int &width, const int &height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(win_x, win_y);
	win_id = glutCreateWindow("orbit");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(0.5, 0.5, 0.5, 1);
	glClearDepth(1.0f);

	main_camera.Set(0, 0, camera_w, camera_fov, win_x, win_y, camera_near, camera_far);
}

void reshape_func(int width, int height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutSetWindow(win_id);
	glutReshapeWindow(win_x, win_y);
	glViewport(0, 0, win_x, win_y);

	main_camera.Set(main_camera.u, main_camera.v, main_camera.w, main_camera.fov, win_x, win_y, camera_near, camera_far);
}

// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
void render_string(int x, const int y, void *font, const string &text)
{
	for (size_t i = 0; i < text.length(); i++)
	{
		glRasterPos2i(x, y);
		glutBitmapCharacter(font, text[i]);
		x += glutBitmapWidth(font, text[i]) + 1;
	}
}
// End text drawing code.

void draw_objects(void)
{
	glEnable(GL_LIGHTING);

	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	GLfloat light0_pos[] = { 0.0f, 0.0f, 10.0f, 1.0f };
	//	GLfloat light1_pos[] = { -2.0f, -2.0f, -2.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glEnable(GL_LIGHT0);

	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	//glEnable(GL_LIGHT1);

	glPushMatrix();





	glDisable(GL_DEPTH_TEST);

	glBegin(GL_TRIANGLES);

	for (size_t i = 0; i < triangles.size(); i++)
	{
		glNormal3f(face_normals[i].x, face_normals[i].y, face_normals[i].z);

		float v0_colour[] = { triangles[i].colour[0].x, triangles[i].colour[0].y, triangles[i].colour[0].z, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v0_colour);

		glVertex3f(triangles[i].vertex[0].x, triangles[i].vertex[0].y, triangles[i].vertex[0].z);

		float v1_colour[] = { triangles[i].colour[1].x, triangles[i].colour[1].y, triangles[i].colour[1].z, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v1_colour);

		glVertex3f(triangles[i].vertex[1].x, triangles[i].vertex[1].y, triangles[i].vertex[1].z);

		float v2_colour[] = { triangles[i].colour[2].x, triangles[i].colour[2].y, triangles[i].colour[2].z, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v2_colour);

		glVertex3f(triangles[i].vertex[2].x, triangles[i].vertex[2].y, triangles[i].vertex[2].z);
	}

	glEnd();

	glEnable(GL_DEPTH_TEST);

	for (size_t i = 0; i < triangles.size(); i++)
	{
		glBegin(GL_LINE_STRIP);

		glNormal3f(face_normals[i].x, face_normals[i].y, face_normals[i].z);

		float v0_colour[] = { 0, 0, 0, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v0_colour);

		glVertex3f(triangles[i].vertex[0].x, triangles[i].vertex[0].y, triangles[i].vertex[0].z);

		//float v1_colour[] = { triangles[i].colour[1].x, triangles[i].colour[1].y, triangles[i].colour[1].z, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v0_colour);

		glVertex3f(triangles[i].vertex[1].x, triangles[i].vertex[1].y, triangles[i].vertex[1].z);

		//float v2_colour[] = { triangles[i].colour[2].x, triangles[i].colour[2].y, triangles[i].colour[2].z, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v0_colour);

		glVertex3f(triangles[i].vertex[2].x, triangles[i].vertex[2].y, triangles[i].vertex[2].z);

		glEnd();
	}


	//glPointSize(4.0);

	//float point_colour[] = { 1.0, 0.5, 0.0, 1.0f };
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, point_colour);

	//glBegin(GL_POINTS);

	//for (int j = 0; j < num_tall; j++)
	//{
	   // for (int i = 0; i < num_wide; i++)
	   // {
		  //  vertex_3 v;
		  //  v.x = cpoints[i][j][0];
		  //  v.y = cpoints[i][j][1];
		  //  v.z = cpoints[i][j][2];

		  //  glVertex3f(v.x, v.y, v.z);
	   // }
	//}

	//glEnd();


	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(-1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -1);

	glEnd();

	glPopMatrix();
}

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the model's components using OpenGL/GLUT primitives.
	draw_objects();

	if (true == draw_control_list)
	{
		// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
		// http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, win_x, 0, win_y);
		glScalef(1, -1, 1); // Neat. :)
		glTranslatef(0, -win_y, 0); // Neat. :)
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor3f(control_list_colour.x, control_list_colour.y, control_list_colour.z);

		size_t break_size = 22;
		size_t start = 20;
		ostringstream oss;

		render_string(10, start, GLUT_BITMAP_HELVETICA_18, string("Mouse controls:"));
		render_string(10, start + 1 * break_size, GLUT_BITMAP_HELVETICA_18, string("  LMB + drag: Rotate camera"));
		render_string(10, start + 2 * break_size, GLUT_BITMAP_HELVETICA_18, string("  RMB + drag: Zoom camera"));

		render_string(10, start + 4 * break_size, GLUT_BITMAP_HELVETICA_18, string("Keyboard controls:"));
		render_string(10, start + 5 * break_size, GLUT_BITMAP_HELVETICA_18, string("  w: Draw axis"));
		render_string(10, start + 6 * break_size, GLUT_BITMAP_HELVETICA_18, string("  e: Draw text"));
		render_string(10, start + 7 * break_size, GLUT_BITMAP_HELVETICA_18, string("  u: Rotate camera +u"));
		render_string(10, start + 8 * break_size, GLUT_BITMAP_HELVETICA_18, string("  i: Rotate camera -u"));
		render_string(10, start + 9 * break_size, GLUT_BITMAP_HELVETICA_18, string("  o: Rotate camera +v"));
		render_string(10, start + 10 * break_size, GLUT_BITMAP_HELVETICA_18, string("  p: Rotate camera -v"));



		vertex_3 eye = main_camera.eye;
		vertex_3 eye_norm = eye;
		eye_norm.normalize();

		oss.clear();
		oss.str("");
		oss << "Camera position: " << eye.x << ' ' << eye.y << ' ' << eye.z;
		render_string(10, win_y - 2 * break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		oss.clear();
		oss.str("");
		oss << "Camera position (normalized): " << eye_norm.x << ' ' << eye_norm.y << ' ' << eye_norm.z;
		render_string(10, win_y - break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// End text drawing code.
	}

	glFlush();
	glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'w':
	{
		draw_axis = !draw_axis;
		break;
	}
	case 'e':
	{
		draw_control_list = !draw_control_list;
		break;
	}
	case 'u':
	{
		main_camera.u -= u_spacer;
		main_camera.Set();
		break;
	}
	case 'i':
	{
		main_camera.u += u_spacer;
		main_camera.Set();
		break;
	}
	case 'o':
	{
		main_camera.v -= v_spacer;
		main_camera.Set();
		break;
	}
	case 'p':
	{
		main_camera.v += v_spacer;
		main_camera.Set();
		break;
	}

	default:
		break;
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			lmb_down = true;
		else
			lmb_down = false;
	}
	else if (GLUT_MIDDLE_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			mmb_down = true;
		else
			mmb_down = false;
	}
	else if (GLUT_RIGHT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			rmb_down = true;
		else
			rmb_down = false;
	}
}

void motion_func(int x, int y)
{
	int prev_mouse_x = mouse_x;
	int prev_mouse_y = mouse_y;

	mouse_x = x;
	mouse_y = y;

	int mouse_delta_x = mouse_x - prev_mouse_x;
	int mouse_delta_y = prev_mouse_y - mouse_y;

	if (true == lmb_down && (0 != mouse_delta_x || 0 != mouse_delta_y))
	{
		main_camera.u -= static_cast<float>(mouse_delta_y)*u_spacer;
		main_camera.v += static_cast<float>(mouse_delta_x)*v_spacer;
	}
	else if (true == rmb_down && (0 != mouse_delta_y))
	{
		main_camera.w -= static_cast<float>(mouse_delta_y)*w_spacer;

		if (main_camera.w < 1.1f)
			main_camera.w = 1.1f;

	}

	main_camera.Set(); // Calculate new camera vectors.
}

void passive_motion_func(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}


long long unsigned int fact(long long unsigned int n)
{
	long long unsigned int ret = 1;

	for (long long unsigned int k = n; k > 0; k--)
		ret *= k;

	return ret;
}

long double binomial(long long unsigned int n, long long unsigned int k)
{

	//     n!
	// -----------
	// k! (n - k)!

	return static_cast<long double>(fact(n)) / static_cast<long double>(fact(k)*fact(n - k));
}

void get_vertices_and_normals_from_triangles(vector<triangle> &t, vector<vertex_3> &fn, vector<vertex_3> &v, vector<vertex_3> &vn)
{
	fn.clear();
	v.clear();
	vn.clear();

	if (0 == t.size())
		return;

	cout << "Triangles: " << t.size() << endl;

	cout << "Welding vertices" << endl;

	// Insert unique vertices into set.
	set<vertex_3> vertex_set;

	for (vector<triangle>::const_iterator i = t.begin(); i != t.end(); i++)
	{
		vertex_set.insert(i->vertex[0]);
		vertex_set.insert(i->vertex[1]);
		vertex_set.insert(i->vertex[2]);
	}

	cout << "Vertices: " << vertex_set.size() << endl;

	cout << "Generating vertex indices" << endl;

	// Add indices to the vertices.
	for (set<vertex_3>::const_iterator i = vertex_set.begin(); i != vertex_set.end(); i++)
	{
		size_t index = v.size();
		v.push_back(*i);
		v[index].index = index;
	}

	vertex_set.clear();

	// Re-insert modifies vertices into set.
	for (vector<vertex_3>::const_iterator i = v.begin(); i != v.end(); i++)
		vertex_set.insert(*i);

	cout << "Assigning vertex indices to triangles" << endl;

	// Find the three vertices for each triangle, by index.
	set<vertex_3>::iterator find_iter;

	for (vector<triangle>::iterator i = t.begin(); i != t.end(); i++)
	{
		find_iter = vertex_set.find(i->vertex[0]);
		i->vertex[0].index = find_iter->index;

		find_iter = vertex_set.find(i->vertex[1]);
		i->vertex[1].index = find_iter->index;

		find_iter = vertex_set.find(i->vertex[2]);
		i->vertex[2].index = find_iter->index;
	}

	vertex_set.clear();

	cout << "Calculating normals" << endl;
	fn.resize(t.size());
	vn.resize(v.size());

	for (size_t i = 0; i < t.size(); i++)
	{
		vertex_3 v0 = t[i].vertex[1] - t[i].vertex[0];
		vertex_3 v1 = t[i].vertex[2] - t[i].vertex[0];
		fn[i] = v0.cross(v1);
		fn[i].normalize();

		vn[t[i].vertex[0].index] = vn[t[i].vertex[0].index] + fn[i];
		vn[t[i].vertex[1].index] = vn[t[i].vertex[1].index] + fn[i];
		vn[t[i].vertex[2].index] = vn[t[i].vertex[2].index] + fn[i];
	}

	for (size_t i = 0; i < vn.size(); i++)
		vn[i].normalize();
}



// TODO: fix camera bug where portrait mode crashes.
void take_screenshot(size_t num_cams_wide, const char *filename, const bool reverse_rows = false)
{
	// Set up Targa TGA image data.
	unsigned char  idlength = 0;
	unsigned char  colourmaptype = 0;
	unsigned char  datatypecode = 2;
	unsigned short int colourmaporigin = 0;
	unsigned short int colourmaplength = 0;
	unsigned char  colourmapdepth = 0;
	unsigned short int x_origin = 0;
	unsigned short int y_origin = 0;
	unsigned short int px = win_x * num_cams_wide;
	unsigned short int py = win_y * num_cams_wide;
	unsigned char  bitsperpixel = 24;
	unsigned char  imagedescriptor = 0;
	vector<char> idstring;

	size_t num_bytes = 3 * px*py;
	vector<unsigned char> pixel_data(num_bytes);

	// Adjust some parameters for large screen format.
	bool temp_draw_control_list = draw_control_list;
	draw_control_list = false;

	float temp_outline_width = outline_width;
	outline_width = 6;

	vector<unsigned char> fbpixels(3 * win_x*win_y);

	// Loop through subcameras.
	for (size_t cam_num_x = 0; cam_num_x < num_cams_wide; cam_num_x++)
	{
		for (size_t cam_num_y = 0; cam_num_y < num_cams_wide; cam_num_y++)
		{
			// Set up camera, draw, then copy the frame buffer.
			main_camera.Set_Large_Screenshot(num_cams_wide, cam_num_x, cam_num_y);
			display_func();
			glReadPixels(0, 0, win_x, win_y, GL_RGB, GL_UNSIGNED_BYTE, &fbpixels[0]);

			// Copy pixels to large image.
			for (GLint i = 0; i < win_x; i++)
			{
				for (GLint j = 0; j < win_y; j++)
				{
					size_t fb_index = 3 * (j*win_x + i);

					size_t screenshot_x = cam_num_x * win_x + i;
					size_t screenshot_y = cam_num_y * win_y + j;
					size_t screenshot_index = 3 * (screenshot_y*(win_x*num_cams_wide) + screenshot_x);

					pixel_data[screenshot_index] = fbpixels[fb_index + 2];
					pixel_data[screenshot_index + 1] = fbpixels[fb_index + 1];
					pixel_data[screenshot_index + 2] = fbpixels[fb_index];
				}
			}
		}
	}

	// Restore the parameters.
	draw_control_list = temp_draw_control_list;
	outline_width = temp_outline_width;
	main_camera.Set();

	// Write Targa TGA file to disk.
	ofstream out(filename, std::ios::binary);

	if (!out.is_open())
	{
		cout << "Failed to open TGA file for writing: " << filename << endl;
		return;
	}

	out.write(reinterpret_cast<char *>(&idlength), 1);
	out.write(reinterpret_cast<char *>(&colourmaptype), 1);
	out.write(reinterpret_cast<char *>(&datatypecode), 1);
	out.write(reinterpret_cast<char *>(&colourmaporigin), 2);
	out.write(reinterpret_cast<char *>(&colourmaplength), 2);
	out.write(reinterpret_cast<char *>(&colourmapdepth), 1);
	out.write(reinterpret_cast<char *>(&x_origin), 2);
	out.write(reinterpret_cast<char *>(&y_origin), 2);
	out.write(reinterpret_cast<char *>(&px), 2);
	out.write(reinterpret_cast<char *>(&py), 2);
	out.write(reinterpret_cast<char *>(&bitsperpixel), 1);
	out.write(reinterpret_cast<char *>(&imagedescriptor), 1);

	out.write(reinterpret_cast<char *>(&pixel_data[0]), num_bytes);
}



#endif
