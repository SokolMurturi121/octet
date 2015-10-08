#include <fstream>
#include <sstream>
#include <ctime>
#include "sand.h"

namespace octet{
	class wave_simulation : public app {
		
		//defines
		#define TWOPI  6.283185
		#define PI 3.141592
		#define WVHEIGHT 64
		#define WVWIDTH 64
		#define PADDEDHWV WVHEIGHT+2
		#define PADDEDWWV WVWIDTH+2

		//Used for making scene
		ref<visual_scene> app_scene;
		
		//Key Components of Wave itself
		mesh_builder waveMesh;
		scene_node *cam, *waveNode;
		ref<mesh> mWave;
		material *blue, *waveMat;
		material *red, *sandMat;
		image *waveImg;
		image *sandImg;

		//dynarray for txt files and normals
		dynarray<char> read;
		dynarray<char> LengthStr;
		dynarray<char> amplitudeStr;
		dynarray<char> speedStr;

		//array of floats and vec3 for sine wave parameters
		float wavelength[8], amplitude[8], speed[8], frequency[8], phase[8];
		vec3 wavesdirection[8];

		//random number generator
		random rand;


		sand floor;									//sand mesh made in collader
		int numWaves = 7;							//number of sine waves
		float increaseAmp = 0.0f;					//floats for additional amplitude and frequency
		float increaseFreq = 0.0f;
		float slope = 0.5f;							//slope of a wave
		float MESHHEIGHT[WVWIDTH][WVHEIGHT];		//array for vertex
		bool start = true;							//boolean for start run
		bool wireframe = false;						//boolean for wireframe mode
		bool textureSwitch = false;					//boolean for flat mesh or texture
		bool gerstnerWave = true;					//boolean for gerstner calculations
		int frame = 1;								//frame counter


	public:
		wave_simulation(int argc, char **argv) : app(argc, argv) 
		{

		}

		//this function is used load each of the individual wave files so that they can be analiysed
		void loadFileWave(int waveNum)
		{
			std::fstream waveFile;
			std::stringstream waveString;
			std::string wave;
			read.reset();
			waveString.clear();
			wave.clear();
			waveString << "wave" << waveNum << ".txt";
			wave = waveString.str();
			waveFile.open(wave, std::ios::in);

			if (!waveFile.is_open())
			{
				printf("File is not found/opened\n");
			}
			else
			{
				printf("File found/opened\n");
			}
			waveFile.seekg(0, waveFile.end);
			unsigned int length = waveFile.tellg();
			waveFile.seekg(0, waveFile.beg);
			waveNum -= 1;
			if (length == -1)
			{
				length = 0;
			}
			if (length != 0)
			{
				read.resize(length);
				waveFile.read(read.data(), length);
				waveFile.close();
				grabLength(waveNum);
				grabAmplitude(waveNum);
				grabSpeed(waveNum);
				waveDirection(waveNum);
				read.push_back('\0');
			}
		}

		//this function will be used to get the wave length and frequency
		void grabLength(int i)
		{
			LengthStr.reset();
			string str(read.data(), read.size());
			int startLoc = str.find("wavelength:");
			startLoc += 11;
			int endLoc = str.find("amplitude:");
			endLoc -= 2;

			for (int i = startLoc; i < endLoc; ++i)
			{
				LengthStr.push_back(read[i]);
				//read[i] = NULL;
			}

			wavelength[i] = atof(LengthStr.data());
			frequency[i] = TWOPI / wavelength[i];
		}

		//get the direction
		void waveDirection(int i)
		{
			wavesdirection[i] = vec3(rand.get(-1.0f, 1.0f), 0.0f, rand.get(-1.0f, 1.0f));
		}


		//get the amplitude
		void grabAmplitude(int i)
		{
			amplitudeStr.reset();
			string str(read.data(), read.size());
			int startLoc = str.find("amplitude:");
			startLoc += 10;
			int endLoc = str.find("speed:");
			endLoc -= 2;

			for (int i = startLoc; i < endLoc; ++i)
			{
				amplitudeStr.push_back(read[i]);
				//read[i] = NULL;
			}

			amplitude[i] = atof(amplitudeStr.data());
		}

		//get the speed
		void grabSpeed(int i)
		{
			speedStr.reset();
			string str(read.data(), read.size());
			int startLoc = str.find("speed:");
			startLoc += 6;
			int endLoc = str.find("end");
			endLoc -= 2;

			for (int i = startLoc; i < endLoc; ++i)
			{
				speedStr.push_back(read[i]);
				//read[i] = NULL;
			}

			speed[i] = atof(speedStr.data());
			speed[i] = speed[i] * 0.1f;
			phase[i] = speed[i] * TWOPI / wavelength[i];
		}


		//generate the mesh
		void meshGeneration(bool first)
		{
			vec3 vertices[4];
			vec3 normals[4];
			vec2 uvs[4];
			int vertexNum;

			//delete old vertices
			if (!first) 
			{
				waveMesh.deleteVertices();
			}

			//for first run, set default variables for flat plane
			if (first)
			{
				vertexNum = 0;
				for (int i = 0; i < PADDEDWWV; i++)
				{
					for (int j = 0; j < PADDEDHWV; j++)
					{
						MESHHEIGHT[i][j] = 0.0f;
					}
				}
			}

			for (int z = 0; z < WVHEIGHT; z++)
			{
				for (int x = 0; x < WVWIDTH; x++)
				{
					//setup vertices
					vertices[0] = vec3(x*1.0f, MESHHEIGHT[x][z], z*1.0f);
					vertices[1] = vec3(x*1.0 + 1.0f, MESHHEIGHT[x + 1][z], z*1.0f);
					vertices[2] = vec3(x*1.0f + 1.0f, MESHHEIGHT[x + 1][z + 1], z*1.0f + 1.0f);
					vertices[3] = vec3(x*1.0f, MESHHEIGHT[x][z + 1], z*1.0f + 1.0f);

					//calculate the normals
					normals[0] = cross((vertices[1] - vertices[0]), (vertices[3] - vertices[0]));
					normals[1] = cross((vertices[2] - vertices[1]), (vertices[0] - vertices[1]));
					normals[2] = cross((vertices[3] - vertices[2]), (vertices[1] - vertices[2]));
					normals[3] = cross((vertices[0] - vertices[3]), (vertices[2] - vertices[3]));

					uvs[0] = vec2(x*(1.0f / WVHEIGHT), z*(1.0f / WVWIDTH));
					uvs[1] = vec2(x *(1.0f / WVHEIGHT) + (1.0f / WVHEIGHT), z*(1.0f / WVWIDTH));
					uvs[2] = vec2(x *(1.0f / WVHEIGHT) + (1.0f / WVHEIGHT), z *(1.0f / WVWIDTH) + (1.0f / WVWIDTH));
					uvs[3] = vec2(x*(1.0f / WVHEIGHT), z *(1.0f / WVWIDTH) + (1.0f / WVWIDTH));
					
					//add vertex to mesh
					for (size_t i = 0; i < 4; i++)
					{
						waveMesh.add_vertex(vec4(vertices[i].x(), vertices[i].y(), vertices[i].z(), 1.0f), vec4(normals[i].x(), normals[i].y(), normals[i].z(), 1.0f), uvs[i].x(), uvs[i].y());
					}

					//create the triangles
					// 0--1
					// | \|
					// 3--2
					if (first)
					{
						waveMesh.add_index(vertexNum);
						waveMesh.add_index(vertexNum + 1);
						waveMesh.add_index(vertexNum + 2);
						waveMesh.add_index(vertexNum);
						waveMesh.add_index(vertexNum + 2);
						waveMesh.add_index(vertexNum + 3);
						vertexNum += 4;
					}
				}
			}
			//set false once first run is complete
			start = false;
		}

		//calculate new height for the point
		void gerstnerWaves(int x, int z)
		{
			float yCoord = 0.0f;
			
			for (int i = 0; i < numWaves; ++i)
			{	
				//sum sine waves
				float xOff = x - wavesdirection[i].x();
				float zOff = z - wavesdirection[i].y();
				float angular = sqrt(pow(xOff,2)+pow(zOff,2));
				float sine = 0.1f * sin((frequency[i] + increaseFreq) * angular + frame * phase[i]);
				yCoord += sine;

				if (gerstnerWave)
				{
					//gerstner waves
					float steepness = slope / (frequency[i] + increaseFreq) * (amplitude[i] + increaseAmp) * numWaves;
					float angle = (frequency[i] + increaseFreq) * wavesdirection[i].dot(vec3(x, MESHHEIGHT[x][z], z)) + frame * phase[i];
					float yPos = steepness * (amplitude[i] + increaseAmp) * wavesdirection[i].z() * cosf(angle);
					yCoord += yPos;
				}
			}
			MESHHEIGHT[x][z] = yCoord;
		}

		//this is called once OpenGL is initalized
		void app_init()
		{
			app_scene = new visual_scene;
			app_scene->create_default_camera_and_lights();
			waveNode = app_scene->add_scene_node();
			rand.set_seed(time(0));
			waveImg = new image("src/examples/water/water.gif");
			sandImg = new image("src/examples/water/water.gif");
			blue = new material(vec4(0, 0, 1, 1));
			red = new material(vec4(1, 0, 0, 1));
			waveMat = new material(waveImg);
			sandMat = new material(sandImg);
			waveMesh.init();
			meshGeneration(start);
			mWave = new mesh();
			waveMesh.get_mesh(*mWave);
			mWave->set_mode(GL_TRIANGLES);
			app_scene->add_mesh_instance(new mesh_instance(waveNode, mWave, blue));
			cam = app_scene->get_camera_instance(0)->get_node();
			cam->translate(vec3(32, 64, 70));
			cam->rotate(-45, vec3(1, 0, 0));
			app_scene->get_camera_instance(0)->set_far_plane(1000);

			//load all the files
			for (int i = 1; i < numWaves; ++i)
			{
				loadFileWave(i);
			}

			//for rendering the rocks
			floor.render();
			app_scene->add_mesh_instance(new mesh_instance(floor.seaNode, floor.seaMesh, floor.seabed));
		}

		//this is called to draw the world
		void draw_world(int x, int y, int w, int h)
		{
			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			app_scene->begin_render(vx, vy);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			

			//generate new plane with new points
			meshGeneration(start);

			//call to calculate new points
			for (int z = 0; z < WVHEIGHT; z++)
			{
				for (int x = 0; x < WVWIDTH; x++)
				{
					gerstnerWaves(x, z);
				}
			}

			//frame counter increment;
			++frame;

			//key input for wireframe
			if (is_key_down(key_space))//is_key_going_down***
			{
				if (!wireframe)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					wireframe = !wireframe;
				}

				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					wireframe = !wireframe;
				}
			}

			//key input to change between texture and solid
			if (is_key_down('Q'))
			{
				if (textureSwitch)
				{
					blue = new material(vec4(0, 1, 1, 1));
					red = new material(vec4(1, 0, 0, 1));
					app_scene->get_mesh_instance(0)->set_material(blue);
					textureSwitch = !textureSwitch;
				}

				else
				{
					waveMat = new material(waveImg);
					sandMat = new material(sandImg);
					app_scene->get_mesh_instance(0)->set_material(waveMat);
					app_scene->get_mesh_instance(0)->set_material(sandMat);
					textureSwitch = !textureSwitch;
				}	
			}

			//key input to alter amplitude
			if (is_key_down('W'))
			{
				increaseFreq += 0.01f;
			}

			else if (is_key_down('S'))
			{
				increaseFreq -= 0.01f;
			}

			//key input to alter frequency
			if (is_key_down('E'))
			{
				increaseAmp += 0.01f;
			}

			else if (is_key_down('D'))
			{
				increaseAmp -= 0.01f;
			}

			//key input for a different wave
			if (is_key_down('T'))
			{
				for (int i = 0; i < numWaves; ++i)
				{
					loadFileWave(i);
				}
			}

			//key input to enable or disable gerstner waves calculations
			if (is_key_down('G'))//is_key_going_down
			{
				gerstnerWave = !gerstnerWave;
			}

			//key input to raise and lower "Rocks"
			if (is_key_down(key_up))
			{
				floor.increment();
			}

			else if (is_key_down(key_down))
			{
				floor.decrement();
			}

			//get the new mesh
			waveMesh.get_mesh(*mWave);

			// update matrices. assume 30 fps.
			app_scene->update(1.0f / 30);

			// draw the scene
			app_scene->render((float)vx / vy);
		}
	};
}