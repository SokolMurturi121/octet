#include <string>

namespace octet{
	class sand{
		collada_builder loader; //loader for the dae file

	public:
		material *seabed;
		mesh *seaMesh;
		scene_node *seaNode;

		int height = 0;

		sand()
		{

		}

		void render()
		{
			//load the dae file for the rocks
			resource_dict dict;
			if (!loader.load_xml("src/examples/water/testseabed.dae"))
			{
				return;
			}
			loader.get_resources(dict);

			//add mesh to array
			dynarray<resource*> meshes;
			dict.find_all(meshes, atom_mesh);

			if (meshes.size())
			{
				seabed = new material(vec4(0.70, 0.76,0.50,1)); //set sand colour
				seaMesh = meshes[0]->get_mesh();				//get the mesh from the array
				seaNode = new scene_node();						
				seaNode->translate(vec3(32, -10, 32));
				seaNode->scale(0.5);
			}
		}

		//adjust seabed floor
		void increment()
		{
			if (height < 15)
			{
				seaNode->translate(vec3(0, 1, 0));
				height++;
			}	
		}

		//lower seabed floor
		void decrement()
		{
			if (height > 0)
			{
				seaNode->translate(vec3(0, -1, 0));
				height--;
			}
		}
	};
}