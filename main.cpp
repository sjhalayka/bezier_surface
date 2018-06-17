#include "main.h"

vertex_3 bezier(const double u, const double v, vector<vector<vector<float> > > &cpoints, size_t num_wide, size_t num_tall)
{
	vertex_3 ret;

	const size_t Ni = num_wide - 1;
	const size_t Nj = num_tall - 1;

	for (size_t i = 0; i <= Ni; i++)
	{
		for (size_t j = 0; j <= Nj; j++)
		{
			double binpow_u = binomial(Ni, i) * pow(u, i) * pow(1 - u, Ni - i);
			double binpow_v = binomial(Nj, j) * pow(v, j) * pow(1 - v, Nj - j);

			ret.x += cpoints[i][j][0] * binpow_u * binpow_v;
			ret.y += cpoints[i][j][1] * binpow_u * binpow_v;
			ret.z += cpoints[i][j][2] * binpow_u * binpow_v;
		}
	}

	return ret;
}

int main(int argc, char **argv)
{
	Mat frame = imread("picture.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (frame.empty())
	{
		cout << "Could not read input file" << endl;
		return 1;
	}

	cout << "Allocating RAM" << endl;

	num_wide = frame.cols;
	num_tall = frame.rows;
	num_dims = 3; // xyz

	cpoints.resize(num_wide);

	for (size_t i = 0; i < num_wide; i++)
	{
		cpoints[i].resize(num_tall);

		for (size_t j = 0; j < num_tall; j++)
			cpoints[i][j].resize(num_dims);
	}

	for (int j = 0; j < num_tall; j++)
	{
		for (int i = 0; i < num_wide; i++)
		{
			cpoints[i][j][0] = double(i) / double(num_tall);
			cpoints[i][j][1] = double(j) / double(num_tall);
			cpoints[i][j][2] = double(frame.at<unsigned char>(j, i)) / 255.0;
		}
	}

	cout << "Tesselating surface" << endl;

	double spacer = 0.1;

	for (double u = 0; u < 1 - 0.001; u += spacer)
	{
		for (double v = 0; v < 1 - 0.001; v += spacer)
		{
			cout << u << " " << v << endl;

			vertex_3 v0 = bezier(u, v, cpoints, num_wide, num_tall);
			vertex_3 v1 = bezier(u + spacer, v, cpoints, num_wide, num_tall);
			vertex_3 v2 = bezier(u + spacer, v + spacer, cpoints, num_wide, num_tall);
			vertex_3 v3 = bezier(u, v + spacer, cpoints, num_wide, num_tall);

			float intensity0 = v0.z;
			float intensity1 = v1.z;
			float intensity2 = v2.z;
			float intensity3 = v3.z;
			float intensity4 = float(intensity0) + float(intensity1) + float(intensity2) + float(intensity3);
			intensity4 /= 4.0;

			vertex_3 v4 = v0 + v1 + v2 + v3;
			v4.x /= 4.0;
			v4.y /= 4.0;
			v4.z /= 4.0;

			triangle t0;
			t0.vertex[0] = v0;
			t0.vertex[1] = v4;
			t0.vertex[2] = v3;

			t0.colour[0].x = intensity0;
			t0.colour[0].y = intensity0;
			t0.colour[0].z = intensity0;
			t0.colour[1].x = intensity4;
			t0.colour[1].y = intensity4;
			t0.colour[1].z = intensity4;
			t0.colour[2].x = intensity3;
			t0.colour[2].y = intensity3;
			t0.colour[2].z = intensity3;


			triangle t1;
			t1.vertex[0] = v0;
			t1.vertex[1] = v1;
			t1.vertex[2] = v4;

			t1.colour[0].x = intensity0;
			t1.colour[0].y = intensity0;
			t1.colour[0].z = intensity0;
			t1.colour[1].x = intensity1;
			t1.colour[1].y = intensity1;
			t1.colour[1].z = intensity1;
			t1.colour[2].x = intensity4;
			t1.colour[2].y = intensity4;
			t1.colour[2].z = intensity4;

			triangle t2;
			t2.vertex[0] = v1;
			t2.vertex[1] = v2;
			t2.vertex[2] = v4;

			t2.colour[0].x = intensity1;
			t2.colour[0].y = intensity1;
			t2.colour[0].z = intensity1;
			t2.colour[1].x = intensity2;
			t2.colour[1].y = intensity2;
			t2.colour[1].z = intensity2;
			t2.colour[2].x = intensity4;
			t2.colour[2].y = intensity4;
			t2.colour[2].z = intensity4;

			triangle t3;
			t3.vertex[0] = v3;
			t3.vertex[1] = v4;
			t3.vertex[2] = v2;

			t3.colour[0].x = intensity3;
			t3.colour[0].y = intensity3;
			t3.colour[0].z = intensity3;
			t3.colour[1].x = intensity4;
			t3.colour[1].y = intensity4;
			t3.colour[1].z = intensity4;
			t3.colour[2].x = intensity2;
			t3.colour[2].y = intensity2;
			t3.colour[2].z = intensity2;

			triangles.push_back(t0);
			triangles.push_back(t1);
			triangles.push_back(t2);
			triangles.push_back(t3);
		}
	}

	get_vertices_and_normals_from_triangles(triangles, face_normals, vertices, vertex_normals);


	glutInit(&argc, argv);
	init_opengl(win_x, win_y);
	glutReshapeFunc(reshape_func);
	glutIdleFunc(idle_func);
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutPassiveMotionFunc(passive_motion_func);
	//glutIgnoreKeyRepeat(1);
	glutMainLoop();
	glutDestroyWindow(win_id);

	return 0;
}




