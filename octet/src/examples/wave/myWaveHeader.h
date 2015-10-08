#include "c:\UniProjects\octet\octet\src\octet.h"
#include <vector>
#include <ctime>
#include <vector>

//#ifndef waveMesh
#define waveMesh

//#define 1Pi 3.141592654f
//#define 2Pi 6.283185307f

	namespace octet{

		class myWaveMesh : public mesh{
		private:

			// Structures

#pragma region Structures
			//vertexs which will be used within OpenGl
			struct Verts{
				vec3p myPositions;
				vec3p myNormal;
				uint32_t myColour;

			};
			struct sineWave{
				float ampli;	// the amplification of the wave stored as a vec3
				vec3 direction; // direction in which the wave travels
				float freq;		//frequencey of waves
				float ohm;		// omega

				sineWave() = default;

				//rFrequency: rate or inverse frequency, waveSpeed speed
				sineWave(float amps, vec3 direc, float rFrequency, float waveSpeed){
					ampli = amps;
					direction = direc;
					freq = 6.283185307f * rFrequency;
					ohm = waveSpeed * freq;

				}
			};
#pragma endregion

			// Variables
#pragma region VARIABLES
			mesh *WaveMesh;
			material *waveMaterial;
			//param_shader *waveShader;
			scene_node *waveNode;
			dynarray<vec3p> points;
			dynarray<vec3p> normals;
			dynarray<sineWave> sineWaves;
			int numberWaves = 10;
			static unsigned long long waveTime;
			float offset = 1.0f;
			vec3 startPos;
			size_t squareSize = 32;
			bool isWireframe = false;
			float tSteepness = 0.5f; // 0: sine wave, 1: maximum Value
			random gen; // random number generator
#pragma endregion
			// private functions
#pragma region privateFunctions
			// creates three floats into RGBA colour


			//FIX!!!!!!!!!!!!!!!!/////////////////////////

			static uint32_t makeColor(float red, float green, float blue){
				//return 0xff000000 + ((int)(red*225.0f) << 0) + ((int)(green*255.0f) << 8) + ((int)blue*255.0f) << 16);
				return(0.0f, 0.0f, 1.0f);
			}
			static uint32_t makeColor(vec3 color){
				return 0xff000000 + ((int)(color.x()*255.0f) << 0) + ((int)(color.y()*255.0f) << 8) + ((int)(color.z()*255.0f) << 16);
			}



			// superceeded takes a vertex and returns the height according to time
			// currently uses the default sine wave
			float getWavePos(int x, int y, sineWave wave){
				float vector_product = wave.direction.dot(vec3((float)x, (float)y, 0.9f));
				float height = wave.ampli * sin(vector_product * wave.freq + waveTime * wave.ohm);
				return height;
			}
			// creates a sin wave
			void createSineWaves(){
				float frequency = 0.02f;
				float ampl = 1.0f;
				float phase = 3.0f;
				for (size_t i = 0; i < numberWaves; ++i){
					sineWaves.push_back(sineWave(ampl * std::pow(0.5,(i + 1)),
					vec3(gen.get(-1.0f, 1.0f), gen.get(-1.0f, 1.0f), 0.0f), frequency, phase));
				}
			}
			//procedurally generated colour
			// doesnt take into account vertices
			vec3 randomColour(){
				return vec3(0.0f, 0.4f, 0.8f);
			}
			// calculate mesh verts using the wave function
			//
			//
			//
			vec3 gerstnerPoints(int x, int y){
				vec3 vert = vec3(0);

				//for eah wave
				for (size_t i = 0; i < sineWaves.size(); ++i){
					// calc each of the points gerstner's waves function
					sineWave wave = sineWaves[i];
					float steepness = tSteepness / (wave.ohm * sineWaves.size());
					float radians = wave.freq * wave.direction.dot(vec3(x, y, 0.0f)) + waveTime *wave.ohm;
					float xPos = steepness * wave.direction.x() * cosf(radians);
					float yPos = steepness * wave.direction.y() * cosf(radians);
					float zPos = wave.ampli * sinf(radians);
					vert += (xPos, yPos, zPos);
				}
				return vert;
			}
			//Calc. normals
			vec3 gerstnerNormal(int x, int y, vec3 point){
				vec3 normal = vec3(0.0f, 0.0f, 1.0f);

				for each (sineWave wave in sineWaves){
					float height = wave.ohm * wave.ampli;
					float steepness = tSteepness / (wave.ohm * sineWaves.size());
					float radians = wave.freq * wave.direction.dot(point) + waveTime * wave.ohm;
					float xPos = -height * wave.direction.x() * cosf(radians);
					float yPos = -height * wave.direction.y() * cosf(radians);
					float zPos = -steepness * height * sinf(radians);
					normal += vec3(xPos, yPos, zPos);
				}
				return normal;
			}
#pragma endregion

		public:
			myWaveMesh(){
				init();
			};

			~myWaveMesh(){
				delete WaveMesh;
				delete waveMaterial;
				//delete waveShader;
				delete waveNode;
				waveNode = NULL;
				//waveShader = NULL;
				WaveMesh = NULL;
				waveMaterial = NULL;
				points.reset();

			}
			void init(){
				WaveMesh = new mesh();
				waveNode = new scene_node();
				//waveShader = new param_shader("shaders/default.vs", "shaders/simple_colour.fs");
				waveMaterial = new material(vec4(1, 1, 0, 1), 0);

				startPos = vec3(-offset * 0.5f * squareSize, offset * 0.5f * squareSize, -1.0f);
				points.resize(squareSize * squareSize);
				normals.resize(squareSize * squareSize);

				size_t numVerts = squareSize * squareSize;
				size_t numIndis = (squareSize - 1)  * (squareSize - 1) * 6;
				WaveMesh->allocate(sizeof(Verts) * numVerts, sizeof(uint32_t) * numIndis);
				WaveMesh->set_params(sizeof(Verts), numIndis, numVerts, GL_TRIANGLES, GL_UNSIGNED_INT);

				WaveMesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
				WaveMesh->add_attribute(attribute_normal, 3, GL_FLOAT, 0);
				WaveMesh->add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);

				gen.set_seed(time(0));

				createSineWaves();
			}
			void BuildMesh(){
				gl_resource::wolock vl(WaveMesh->get_vertices());
				Verts *vertx = (Verts *)vl.u8();
				gl_resource::wolock il(WaveMesh->get_indices());
				uint32_t *index = il.u32();

				for (size_t i = 0; i != squareSize; ++i){
					for (size_t j = 0; j != squareSize; ++j){
						vertx->myPositions = points[j + i * squareSize];
						vertx->myNormal = normals[j + i * squareSize];
						vertx->myColour = makeColor(randomColour());
					}
				}

				uint32_t apv = 0;
				for (size_t i = 0; i != squareSize * (squareSize - 1) + 1; ++i){
					if (i % squareSize != squareSize - 1){
						index[0] = i;
						index[1] = i + squareSize + 1;
						index[2] = i + 1;
						index[3] = i;
						index[4] = i + squareSize;
						index[5] = i + squareSize + 1;
						index += 6;
					}
				}
			}

			void addToTheScene(visual_scene * app_scene){
				app_scene->add_mesh_instance(new mesh_instance(waveNode, WaveMesh, waveMaterial));
			}
			void update(){
				for (size_t i = 0; i != squareSize; ++i){
					for (size_t j = 0; j != squareSize; ++i){
						vec3 waveVector = gerstnerPoints(j, i);
						points[j + i * squareSize] = vec3p(startPos + vec3(offset * j, -offset * i, 0.0f) + waveVector);
						normals[j + i * squareSize] = vec3p(gerstnerNormal(j, i, points[j + i * squareSize]));
					}
				}
				++waveTime;
			}

#pragma regin set
			scene_node *getWaveNode(){
				return waveNode;
			}
			void inline incrementedAmpli(){
				sineWaves.begin()->ampli += 0.1f;
			}

			void inline decrementedAmpli(){
				sineWaves.begin()->ampli -= 0.1f;
			}
			void inline incrementedFreq(){
				sineWaves.begin()->ohm += sineWaves.begin()->ohm * sineWaves.begin()->freq;
			}
			void inline decrementedFreq(){
				sineWaves.begin()->ohm -= sineWaves.begin()->ohm * sineWaves.begin()->freq;
			}
			void inline incrementedDirec(){
				sineWaves.begin()->direction = sineWaves.begin()->direction *mat4t().rotateZ(14.0f);
			}
			void inline decrementedDirec(){
				sineWaves.begin()->direction = sineWaves.begin()->direction *mat4t().rotateZ(14.0f);
			}

			void inline wireFrame(){
				size_t numVertices = squareSize * squareSize;
				size_t numIndices = (squareSize - 1) * (squareSize - 1) * 6;
				if (!isWireframe){
					WaveMesh->set_params(sizeof(Verts), numIndices, numVertices, GL_LINES, GL_UNSIGNED_INT);
				}
				else{
					WaveMesh->set_params(sizeof(Verts), numIndices, numVertices, GL_TRIANGLES, GL_UNSIGNED_INT);
				}
				isWireframe = !isWireframe;
			}
#pragma endregion
		};

		unsigned long long myWaveMesh::waveTime = 0;

	}