////////////////////////////////////////////////////////////////////////////////
//
// (C) Sokol Murturi 2012-2014
//
// First Assignment
//
namespace octet {
	/// Scene using bullet for physics effects.

	/// Box class, this class uses box mesh from physics example///

	class Box {

	protected:
		mat4t modelToWorld;
		vec3 size;
		material *mat;
		btRigidBody* rigbody;
		mesh_box* mesh;
		scene_node *node;
		btDefaultMotionState *motionState;
		btTransform boxTransform;
		

	public:

		Box(){

		}

		~Box(){

		}

		void initBox(mat4t_in model2World, vec3_in size_box, material *mat_box, bool is_dynamic = true){

			modelToWorld = model2World;
			mat = mat_box;
			size = size_box;

			btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld)); 
			btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
			btCollisionShape *shape = new btBoxShape(get_btVector3(size));
			btTransform transform(matrix, pos);
			motionState = new btDefaultMotionState(transform);
			btScalar mass = is_dynamic ? 1.0f : 0.0f; 
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

	class sphereBase{

	protected:
		
		material *mat;
		mat4t modelToWorld;
		scene_node *node;
		btDefaultMotionState *motionState;
		btRigidBody *rigbody;
		float sphereSize;
		mesh_sphere *spheremesh;
		vec3 myVector;
		btTransform transform;
		float maxSpeed;

	public:

			sphereBase(){

		}
			~sphereBase(){

			}
			btRigidBody *getRigidBody(){
				return rigbody;
			}
			void initShpere(mat4t_in model2World, float_in size_sphere, material *mat_box, bool is_dynamic = true){

				modelToWorld = model2World;
				mat = mat_box;
				sphereSize = size_sphere;

				btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld)); ///Translation vector from M2W///
				btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
				const vec3 position(pos.getX(), pos.getY(), pos.getZ());
				btCollisionShape *shape = new btSphereShape(float(sphereSize));
				btTransform transform(matrix, pos);
				motionState = new btDefaultMotionState(transform);
				btScalar mass = is_dynamic ? 1.0f : 0.0f; ///Mass of the object///
				btVector3 inertiaTensor;
				shape->calculateLocalInertia(mass, inertiaTensor);
				rigbody = new btRigidBody(mass, motionState, shape, inertiaTensor); ///Constructs the rigidbody///
				maxSpeed = 90.0f;
				/// init mesh and scene node ///
				spheremesh = new mesh_sphere(static_cast<vec3>(position), float(sphereSize));
				node = new scene_node(modelToWorld, atom_);
			}
			void addToScene(btDiscreteDynamicsWorld&world, dynarray<btRigidBody*> &rigid_bodies, dynarray<scene_node*>&nodes, ref<visual_scene>&app_scene){

				world.addRigidBody(rigbody);
				rigid_bodies.push_back(rigbody);
				nodes.push_back(node);
				app_scene->add_child(node);
				app_scene->add_mesh_instance(new mesh_instance(node, spheremesh, mat));

			}
			void reset(){
				myVector = vec3(14.0F, 6.0f, 42.0f);
				transform = btTransform(get_btMatrix3x3(modelToWorld), get_btVector3(myVector));
				rigbody->setWorldTransform(transform);
				rigbody->setLinearVelocity(get_btVector3(vec3(0, 0, 0)));
				rigbody->setAngularVelocity(get_btVector3(vec3(0, 0, 0)));

			}
			void update(){
				if (maxSpeed >= 0)
				{
					btVector3 velocity = rigbody->getLinearVelocity();
					float speed = velocity.length();
					if (speed > maxSpeed)
						rigbody->setLinearVelocity(velocity * (maxSpeed / speed));
				}

			}
				
	};


	class skittle : public Box
	{
	protected:

		vec3 position;
		btVector3 MyPos;
		vec3 resetPos;

	public:

		skittle(){

		}

		~skittle(){

		}

		void setResetPosition(vec3 resetPosition){
			resetPos = resetPosition;
		}

		vec3 *getResetPosition() {
			return &resetPos;
		}

		btRigidBody *SkittleRigBody1(){
			return rigbody;
		}
		scene_node *skittle1node(){
			return node;
		}
		void reset(){
			position = resetPos;
			boxTransform = btTransform(get_btMatrix3x3(modelToWorld), get_btVector3(position));
			rigbody->setWorldTransform(boxTransform);
			rigbody->setLinearVelocity(get_btVector3(vec3(0, 0, 0)));
			rigbody->setAngularVelocity(get_btVector3(vec3(0, 0, 0)));
		}

		void myPosition(){
			btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
			vec3 position(pos.getX(), pos.getY(), pos.getZ());
			printf("X %f, Y %f, Z %F", pos.getZ(), pos.getY(), pos.getZ());
		}

		/*void StartPosX(){
		float x, y, z;
		vec4 x = node->access_nodeToParent;
		printf("x %f", x);

			modelToWorld = skittle1.skittle1node()->access_nodeToParent();
		vec4 x = modelToWorld.x();
		vec4 y = modelToWorld.y();
		vec4 z = modelToWorld.z();*/
		//}
		/*void MypositionX(){
			modelToWorld = skittle1node()->access_nodeToParent();
			vec4 x = modelToWorld.xyz((x), (y), (z), (w));
			//vec4 y = modelToWorld.y();
			//vec4 z = modelToWorld.z();

			printf("x: %f", x[0]);
			//printf("x: %f y: %f z: %f \n", y[0], y[1], y[2]);
			//printf("x: %f y: %f z: %f \n", z[0], z[1], z[2]);
			return Myx;

	}*/
	};
	struct StartingPos{
		float X;
		float Y;
		float Z;
		StartingPos() : X(0), Y(0), Z(0){
		}
		StartingPos(float startX, float startY, float startZ) : X(startX), Y(startY), Z(startZ){
		}
	};

	class my_leet_project : public app {
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
		sphereBase BowlingBall;
		skittle skittle1, skittle2, skittle3, skittle4, skittle5, skittle6, skittle7, skittle8, skittle9, skittle10;

		random *random_gen = new random();
		mat4t modelToWorld;

		Box

			lane,
			outerGutterOne,
			outerGutterTwo,
			innerGutterOne,
			innerGutterTwo,
			gutterFloorOne,
			gutterFloorTwo;


		void add_box(mat4t_in modelToWorld, vec3_in size, material *mat, bool is_dynamic = true) {

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
		my_leet_project(int argc, char **argv) : app(argc, argv) {
			dispatcher = new btCollisionDispatcher(&config);
			broadphase = new btDbvtBroadphase();
			solver = new btSequentialImpulseConstraintSolver();
			world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, &config);
		}

		~my_leet_project() {
			delete world;
			delete solver;
			delete broadphase;
			delete dispatcher;
		}

		/// this is called once OpenGL is initialized
		void app_init() {
			app_scene = new visual_scene();
			app_scene->create_default_camera_and_lights();
			app_scene->get_camera_instance(0)->get_node()->translate(vec3(14, 11, 32));////CAMERA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			material *floor_mat = new material(vec4(0.5432f, 0.5432f, 0.5432f, 0.0f));
			// add the ground (as a static object)
			add_box(modelToWorld, vec3(100.0f, 0.5f, 200.0f), floor_mat, false); ////// change to size of a lane?////


			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		 

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			////// This is where the Bowling Lane is Created, Including: Colour(material), Position(ModelToWorld), Size(vec3) and whether or not the Lane is a static or Dynamic Object(the Bool.) It uses 7 parts to create the lane//////

			material *laneMat = new material(vec4(0.787f, 0.551f, 0.1587f, 0.0f));/// the colour of the Lane ///
			/////Why? this is used in the Initialize box, which requires a specific material. This vec4 gives the programmer the choice of how much Red Blue Green and Alpha to use. I choose to use a Float rather than an Int to give me more options with Colour. /////


			//Lane//

			mat4t laneModelToWorld; /// creates mat4t specificaly for lane (a 4x4 Matrix)///
			laneModelToWorld.loadIdentity(); /// Creates an Identity Matrix ///
			laneModelToWorld.translate(14, 4, 5); /// Gives a position for which object is to be placed ///
			///// Why? /////

			lane.initBox(laneModelToWorld, vec3(10.0f, 0.25f, 40.0f), laneMat, false); /// Lane.initBox(modelToWorld(where the box is placed within the world), vec3(width,Height,depth size of box), floor_mat(the colour of the box), true/false whether or not object is dynamic or static) ///
			lane.addToScene((*world), rigid_bodies, nodes, app_scene);

			//Left Side Outer Wall//

			mat4t oGOModelToWorld; /// oGO = outerGutterOne, used to give a 4x4 matrix to outer gutter one ///
			oGOModelToWorld.loadIdentity(); /// Creates an Identity Matrix ///
			oGOModelToWorld.translate(2.0f, 3.75f, 5.0f); /// Gives a position for object is to be placed ///
			///// Why? /////

			outerGutterOne.initBox(oGOModelToWorld, vec3(0.25f, 1.0f, 40.0f), laneMat, false);
			outerGutterOne.addToScene((*world), rigid_bodies, nodes, app_scene);

			//Right Side Outer Wall//

			mat4t oGTModelToWorld; /// oGTModelToWorld = outerGutterTwo, used to give a 4x4 matrix to outer gutter two ///
			oGTModelToWorld.loadIdentity(); /// Creates an identity matrix ///
			oGTModelToWorld.translate(26.0f, 3.75f, 5.0f); /// Gives a postiion for object to be placed ///
			///// Why? /////

			outerGutterTwo.initBox(oGTModelToWorld, vec3(0.25f, 1.0f, 40.0f), laneMat, false);
			outerGutterTwo.addToScene((*world), rigid_bodies, nodes, app_scene);

			//Left Side Gutter Floor//

			mat4t gFOModelToWorld; /// gFOModelToWorld = gutter floor one, used to give a 4x4 matrix to gutter floor one ///
			gFOModelToWorld.loadIdentity();/// Creates an identity matrix
			gFOModelToWorld.translate(2.9f, 2.8f, 5.0f);
			///// Why? /////

			gutterFloorOne.initBox(gFOModelToWorld, vec3(1.0f, 0.1f, 40.0f), laneMat, false);
			gutterFloorOne.addToScene((*world), rigid_bodies, nodes, app_scene);

			//Right Side Gutter Floor//

			mat4t gFTModelToWorld; /// gFOModelToWorld = gutter floor one, used to give a 4x4 matrix to gutter floor one ///
			gFTModelToWorld.loadIdentity();/// Creates an identity matrix
			gFTModelToWorld.translate(24.9f, 2.8f, 5.0f);
			///// Why? /////

			gutterFloorTwo.initBox(gFTModelToWorld, vec3(1.0f, 0.1f, 40.0f), laneMat, false);
			gutterFloorTwo.addToScene((*world), rigid_bodies, nodes, app_scene);

			//Left Side Inner Wall//

			mat4t iGOModelToWorld; /// iGOModelToWorld = innerGutterOne, used to give a 4x4 matrix to inner gutter one ///
			iGOModelToWorld.loadIdentity();/// Creates an identity matrix
			iGOModelToWorld.translate(4.0f, 3.5f, 5.0f);
			///// Why? /////

			innerGutterOne.initBox(iGOModelToWorld, vec3(0.1f, 0.75f, 40.0f), laneMat, false);
			innerGutterOne.addToScene((*world), rigid_bodies, nodes, app_scene);

			//Right Side Inner Wall//

			mat4t iGTModelToWorld; ///
			iGTModelToWorld.loadIdentity();
			iGTModelToWorld.translate(23.9f, 3.5f, 5.0f);
			///// Why? /////

			innerGutterTwo.initBox(iGTModelToWorld, vec3(0.1f, 0.75f, 40.0f), laneMat, false);
			innerGutterTwo.addToScene((*world), rigid_bodies, nodes, app_scene);

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			////// This is where skittles are Created, Including: Colour(material), Position(ModelToWorld), Size(vec3) and whether or not the Lane is a static or Dynamic Object(the Bool.) //////

			material *skittleMat = new material(vec4(1.0f, 1.0f, 1.0f, 0.0f));

			//Skittle 1//
			
			mat4t s1ModelToWorld;
	
			/*for (int i = 0; i != 4; ++i) {*/
			s1ModelToWorld.loadIdentity();
			s1ModelToWorld.translate(6.5f, 6.0f, 0.0f);

			skittle1.initBox(s1ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle1.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle1.setResetPosition(vec3 (6.5f, 6.0f, 0.0f));

			//Skittle 2//
			mat4t s2ModelToWorld;
			s2ModelToWorld.loadIdentity();
			s2ModelToWorld.translate(11.5f, 6.0f, 0.0f);

			skittle2.initBox(s2ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle2.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle2.setResetPosition(vec3(11.5f, 6.0f, 0.0f));

			//Skittle 3//
			mat4t s3ModelToWorld;
			s3ModelToWorld.loadIdentity();
			s3ModelToWorld.translate(16.5f, 6.0f, 0.0f);

			skittle3.initBox(s3ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle3.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle3.setResetPosition(vec3(16.5f, 6.0f, 0.0f));

			//Skittle 4//
			mat4t s4ModelToWorld;
			s4ModelToWorld.loadIdentity();
			s4ModelToWorld.translate(21.5f, 6.0f, 0.0f);

			skittle4.initBox(s4ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle4.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle4.setResetPosition(vec3(21.5f, 6.0f, 0.0f));

			//Skittle 5//
			mat4t s5ModelToWorld;
			s5ModelToWorld.loadIdentity();
			s5ModelToWorld.translate(9.0f, 6.0f, 5.0f);

			skittle5.initBox(s5ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle5.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle5.setResetPosition(vec3(9.0f, 6.0f, 5.0f));

			//Skittle 6//
			mat4t s6ModelToWorld;
			s6ModelToWorld.loadIdentity();
			s6ModelToWorld.translate(14.0f, 6.0f, 5.0f);

			skittle6.initBox(s6ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle6.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle6.setResetPosition(vec3(14.0f, 6.0f, 5.0f));

			//Skittle 7//
			mat4t s7ModelToWorld;
			s7ModelToWorld.loadIdentity();
			s7ModelToWorld.translate(19.0f, 6.0f, 5.0f);

			skittle7.initBox(s7ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle7.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle7.setResetPosition(vec3(19.0f, 6.0f, 5.0f));

			//Skittle 8//
			mat4t s8ModelToWorld;
			s8ModelToWorld.loadIdentity();
			s8ModelToWorld.translate(11.5f, 6.0f, 10.0f);

			skittle8.initBox(s8ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle8.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle8.setResetPosition(vec3(11.5f, 6.0f, 10.0f));

			//Skittle 9//
			mat4t s9ModelToWorld;
			s9ModelToWorld.loadIdentity();
			s9ModelToWorld.translate(16.5f, 6.0f, 10.0f);

			skittle9.initBox(s9ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle9.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle9.setResetPosition(vec3(16.5f, 6.0f, 10.0f));

			//Skittle 10//
			mat4t s10ModelToWorld;
			s10ModelToWorld.loadIdentity();
			s10ModelToWorld.translate(14.0f, 6.0f, 15.0f);

			skittle10.initBox(s10ModelToWorld, vec3(0.5f, 3.0f, 0.5f), skittleMat, true);
			skittle10.addToScene((*world), rigid_bodies, nodes, app_scene);
			skittle10.setResetPosition(vec3(14.0f, 6.0f, 15.0f));
			

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



			material *BBMat = new material(vec4(0.0f, 0.0f, 0.0f, 0.0f));
			mat4t BBModelToWorld;

			BBModelToWorld.loadIdentity();
			BBModelToWorld.translate(14.0F, 6.0f, 42.0f);
			///14, 11, 32
			BowlingBall.initShpere(BBModelToWorld, float(1.0f), BBMat, true);
			BowlingBall.addToScene((*world), rigid_bodies, nodes, app_scene);


			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// add the boxes (as dynamic objects)
			modelToWorld.translate(-4.5f, 10.0f, 0);
			material *mat = new material(vec4(0, 1, 1, 1));
			for (int i = 0; i != 20; ++i) {
			//modelToWorld.translate(3, 0, 0);
			//modelToWorld.rotateZ(360 / 20);
			//add_box(modelToWorld, vec3(0.5f), mat);
				mat4t modelToWorld;

			}


			// comedy box
			//modelToWorld.loadIdentity();
			//modelToWorld.translate(0, 200, 0);
			//add_box(modelToWorld, vec3(5.0f), floor_mat);
		}

		/// this is called to draw the world
		void draw_world(int x, int y, int w, int h) {
			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			app_scene->begin_render(vx, vy);

			world->stepSimulation(1.0f / 30);
			for (unsigned i = 0; i != rigid_bodies.size(); ++i) {
				btRigidBody *rigid_body = rigid_bodies[i];
				btQuaternion btq = rigid_body->getOrientation();
				btVector3 pos = rigid_body->getCenterOfMassPosition();
				quat q(btq[0], btq[1], btq[2], btq[3]);
				mat4t modelToWorld = q;
				modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);
				nodes[i]->access_nodeToParent() = modelToWorld;
			}
			if (is_key_down('W')){
				//printf("wIsDown");check for key
				BowlingBall.getRigidBody()->applyImpulse(btVector3(0.0F, 0.0F, -10.0F), btVector3(0.0f, 0.0f, 0.0f));
			}
			if (is_key_down('D')){
				//printf("dIsDown");check for key
				BowlingBall.getRigidBody()->applyImpulse(btVector3(5.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
			}

			if (is_key_down('A')){
				//printf("aIsDown");check for key
				BowlingBall.getRigidBody()->applyImpulse(btVector3(-5.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
			}
			if (is_key_down('Q')){
				//printf("qIsDown");check for key
				BowlingBall.getRigidBody()->translate(btVector3(-0.5F, 0.0f, 0.0f));
			}
			if (is_key_down('E')){
				//printf("eIsDown");check for key
				BowlingBall.getRigidBody()->translate(btVector3(0.5F, 0.0f, 0.0f));
			}
			if (is_key_down('R')){
				//printf("rIsDown");check for key
				BowlingBall.reset();
				skittle1.reset();
				skittle2.reset();
				skittle3.reset();
				skittle4.reset();
				skittle5.reset();
				skittle6.reset();
				skittle7.reset();
				skittle8.reset();
				skittle9.reset();
				skittle10.reset();
			}
			BowlingBall.update();
			if (frame_count % 30 == 1){
				modelToWorld = skittle1.skittle1node()->access_nodeToParent();
				vec4 x = modelToWorld.x();
				//vec4 y = modelToWorld.y();
				//vec4 z = modelToWorld.z();
				
				printf("x: %f \n", x[0]);
				//printf("x: %f y: %f z: %f \n", x[0], x[1], x[2]);
				//printf("x: %f y: %f z: %f \n", y[0], y[1], y[2]);
				//printf("x: %f y: %f z: %f \n", z[0], z[1], z[2]);
			}
			// update matrices. assume 30 fps.
			app_scene->update(1.0f / 30);
			// draw the scene
			app_scene->render((float)vx / vy);
			// this is the same as writing frame_count = frame_count + 1
			frame_count++;
			float maxSpawn;
			maxSpawn = random_gen->get(1, 3);
			if (frame_count % 30 == 10) {
				for (int i = 0; i != maxSpawn; ++i){
					// initialse box instantiate values
					mat4t modelToWorld;
					// random num generator

					float pos_x, pos_y, pos_z;
					//assign random floats
					pos_x = random_gen->get(-15.0f, 0.0f);
					pos_y = random_gen->get(40.0f, 40.0f);		// sets the spawn height of blocks
					pos_z = random_gen->get(-5.0f, 42.0f);		// in front of camera
					modelToWorld.translate(pos_x, pos_y, pos_z);
					//printf("x %f, y %f, z %f \n", pos_x, pos_y, pos_z);
					float Red, Blue, Green;
					Red = random_gen->get(0.5f, 1.0f);
					Blue = random_gen->get(0.5f, 1.0f);
					Green = random_gen->get(0.5f, 1.0f);

					//printf("Red %f, Green %f, Blue %f \n", Red, Green, Blue);
					material *mat = new material(vec4(Red, Green, Blue, 1));

					float ext_x, ext_y, ext_z;
					ext_x = random_gen->get(1.0f, 3.0f);
					ext_y = 1.0f; // so height of box is constant
					ext_z = random_gen->get(1.0f, 3.0f);

					add_box(modelToWorld, vec3(ext_x, ext_y, ext_z), mat);

				}

			}
		}
	};
}