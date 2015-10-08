////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//


#include "myWaveHeader.h"

namespace octet {
  /// Scene containing a box with octet.

  class wave : public app {

	  //scene for drawing box
	  ref<visual_scene> app_scene;
	  ref<camera_instance> camera;
	  ref<myWaveMesh> lWave;

	  void userInterface(){

		  if (is_key_down('Q')){ lWave->incrementedFreq(); }

		  if (is_key_down('W')){ lWave->decrementedFreq(); }

		  if (is_key_down('E')){ lWave->incrementedAmpli(); }

		  if (is_key_down('R')){ lWave->decrementedAmpli(); }

		  if (is_key_down('T')){ lWave->incrementedDirec(); }

		  if (is_key_down('Y')){ lWave->decrementedDirec(); }

		  if (is_key_down('S')){ lWave->wireFrame(); }

	  }
  public:
	  /// this is called when we construct the class before initilisation
	  wave(int argc, char **argv) : app(argc, argv){
		  }
	  void app_init(){
		  app_scene = new visual_scene();
		  app_scene->create_default_camera_and_lights();
		  camera = app_scene->get_camera_instance(0);
		  camera->get_node()->translate(vec3(0.0f, 0.0f, 4.0f));

		  lWave = new myWaveMesh();
		  lWave->update();
		  lWave->BuildMesh();
		  lWave->addToTheScene(app_scene);

		  scene_node *waveNode = lWave->getWaveNode();
		  waveNode->rotate(-45.0f, vec3(1.0f, 0.0f, 0.0f));

	  }
    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);
	 
      // update matrices. assume 30 fps.
	  lWave->update();
	  lWave->BuildMesh();
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

      // tumble the box  (there is only one mesh instance)
      scene_node *node = app_scene->get_mesh_instance(0)->get_node();
      node->rotate(1, vec3(1, 0, 0));
      node->rotate(1, vec3(0, 1, 0));
	  userInterface();
	  
    }
  };
}
