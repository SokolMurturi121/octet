////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Scene using bullet for physics effects.
	
/// Box class, this class will be used to create all parts of game using the box mesh from physics example///
	
	class Box {

	protected:
		mat4t modelToWorld;
		vec3 size;
		material *mat;
		btRigidBody* rigbody;
		mesh_box* mesh;
		scene_node *node;
		btDefaultMotionState *motionState;

	public:

		Box(){

		}

		~Box(){

		}

		void initBox(mat4t_in model2World, vec3_in size_box, material *mat_box, bool is_dynamic = true){

			modelToWorld = model2World;
			mat = mat_box;
			size = size_box;

///scale and rotation vector from model to world///

			btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld)); ///Translation vector from M2W///
			btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
			btCollisionShape *shape = new btBoxShape(get_btVector3(size));
			btTransform transform(matrix, pos);
			motionState = new btDefaultMotionState(transform); 
			btScalar mass = is_dynamic ? 1.0f : 0.0f; ///Mass of the object///
			btVector3 inertiaTensor;
			shape->calculateLocalInertia(mass, inertiaTensor);
			rigbody = new btRigidBody(mass, motionState, shape, inertiaTensor); ///Constructs the rigidbody///

			mesh = new mesh_box(size);
			node = new scene_node(modelToWorld, atom_);
		}
		void addToScene(btDiscreteDynamicsWorld&world, dynarray<btRigidBody*> &rigid_bodies, dynarray<scene_node*>&nodes, ref<visual_scene>&app_scene){
			
			world.addRigidBody(rigbody);
			rigid_bodies.push_back(rigbody);
			nodes.push_back(node);
			app_scene->add_child(node);
			app_scene->add_mesh_instance(new mesh_instance(node, mesh, mat));
			
		}
	};


  class example_physics : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

    btDefaultCollisionConfiguration config;       /// setup for the world
    btCollisionDispatcher *dispatcher;            /// handler for collisions between objects
    btDbvtBroadphase *broadphase;                 /// handler for broadphase (rough) collision
    btSequentialImpulseConstraintSolver *solver;  /// handler to resolve collisions
    btDiscreteDynamicsWorld *world;             /// physics world, contains rigid bodies

    dynarray<btRigidBody*> rigid_bodies;
    dynarray<scene_node*> nodes;
	int frame_count = 0;

    void add_box(mat4t_in modelToWorld, vec3_in size, material *mat, bool is_dynamic=true) {

      btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld));
      btVector3 pos(get_btVector3(modelToWorld[3].xyz()));

      btCollisionShape *shape = new btBoxShape(get_btVector3(size));

      btTransform transform(matrix, pos);

      btDefaultMotionState *motionState = new btDefaultMotionState(transform);
      btScalar mass = is_dynamic ? 1.0f : 0.0f;
      btVector3 inertiaTensor;
   
      shape->calculateLocalInertia(mass, inertiaTensor);
    
      btRigidBody * rigid_body = new btRigidBody(mass, motionState, shape, inertiaTensor);
      world->addRigidBody(rigid_body);
      rigid_bodies.push_back(rigid_body);

      mesh_box *box = new mesh_box(size);
      scene_node *node = new scene_node(modelToWorld, atom_);
      nodes.push_back(node);

      app_scene->add_child(node);
      app_scene->add_mesh_instance(new mesh_instance(node, box, mat));
    }
  public:
    /// this is called when we construct the class before everything is initialised.
    example_physics(int argc, char **argv) : app(argc, argv) {
      dispatcher = new btCollisionDispatcher(&config);
      broadphase = new btDbvtBroadphase();
      solver = new btSequentialImpulseConstraintSolver();
      world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, &config);
    }

    ~example_physics() {
      delete world;
      delete solver;
      delete broadphase;
      delete dispatcher;
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 5, 0));
	  
      mat4t modelToWorld;
	  mat4t *pm2w = new mat4t();
      material *floor_mat = new material(vec4(0, 1, 0, 1));
	  material *Boxie_mat = new material(vec4(1, 0, 0, 1));

      // add the ground (as a static object)
      add_box(modelToWorld, vec3(100.0f, 0.5f, 200.0f), floor_mat, false); ////// change to size of a lane?////

	  // adds a box//
	  modelToWorld.loadIdentity();
	  modelToWorld.translate(3.0f, 4.0f, 5.0f);
	  pm2w->loadIdentity();
	  pm2w->translate(-3.0f, 4.0f, 5.0f);
	  Box Lane, Boxie;
	  Lane.initBox(modelToWorld, vec3(2.0f), floor_mat, true);
	  Boxie.initBox((*pm2w), vec3(3.0f), Boxie_mat, true);
	  Lane.addToScene((*world), rigid_bodies, nodes, app_scene);
	  Boxie.addToScene((*world), rigid_bodies, nodes, app_scene);



      // add the boxes (as dynamic objects)
      modelToWorld.translate(-5.5f, 10.0f, 0);
      material *mat = new material(vec4(0, 1, 1, 1));
	  for (int i = 0; i != 4; ++i) {
		  modelToWorld.translate(3, 0, 0);
		  modelToWorld.rotateZ(20 / 20);
	  }
        add_box(modelToWorld, vec3(0.5f), mat);

	  modelToWorld.translate(-5.5f, 15.0f, 0);

	  for (int i = 0; i != 3; ++i) {
		  modelToWorld.translate(3, 0, 0);
		  modelToWorld.rotateZ(20 / 20);
		  add_box(modelToWorld, vec3(0.5f), mat);
	  }
      // comedy box
      modelToWorld.loadIdentity();
      modelToWorld.translate(0, 200, 0);
      add_box(modelToWorld, vec3(5.0f), floor_mat);
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      world->stepSimulation(1.0f/30);
      for (unsigned i = 0; i != rigid_bodies.size(); ++i) {
        btRigidBody *rigid_body = rigid_bodies[i];
        btQuaternion btq = rigid_body->getOrientation();
        btVector3 pos = rigid_body->getCenterOfMassPosition();
        quat q(btq[0], btq[1], btq[2], btq[3]);
        mat4t modelToWorld = q;
        modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);
		nodes[i]->access_nodeToParent() = modelToWorld;
	  }
	  // update matrices. assume 30 fps.
	  app_scene->update(1.0f / 30);
	  // draw the scene
	  app_scene->render((float)vx / vy);
	  // this is the same as writing frame_count = frame_count + 1
	  frame_count++;

	  if (frame_count % 30 == 10) {
		  // initialse box instantiate values
		  mat4t modelToWorld;
		  // random num generator
		  random *random_gen = new random();

		  float pos_x, pos_y, pos_z;
		  // assign random floats
		  pos_x = random_gen->get(-5.0f, 5.0f);
		  pos_y = random_gen->get(20.0f, 20.0f);		// sets the spawn height of blocks
		  pos_z = random_gen->get(-5.0f, 0.0f);		// in front of camera
		  modelToWorld.translate(pos_x, pos_y, pos_z);
		  float Red, Blue, Green, Alpha;
		  Red = random_gen->get(0.5f, 1.0f);
		  Blue = random_gen->get(0.5f, 1.0f);
		  Green = random_gen->get(0.5f, 1.0f);

		  printf("Red %f, Green %f, Blue %f \n", Red, Green, Blue);
		  material *mat = new material(vec4(Red, Green, Blue, 1));

		  float ext_x, ext_y, ext_z;
		  ext_x = random_gen->get(1.0f, 3.0f);
		  ext_y = 1.0f; // so height of box is constant
		  ext_z = random_gen->get(1.0f, 3.0f);

		  add_box(modelToWorld, vec3(ext_x, ext_y, ext_z), mat);
	  }
	}
  };
}