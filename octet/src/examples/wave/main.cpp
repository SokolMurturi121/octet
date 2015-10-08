#include "c:\UniProjects\octet\octet\src\octet.h"
#define OCTET_BULLET 1
#include "wave.h"


//int width;
//int howBig;
//std::vector<octet::vec3> verts;
//std::vector<int> indis;
//
//void grid(int W){
//	howBig = W*W;
//		for (int y= 0; y < W; y++){
//			
//			for (int x = 0; x < W; x++){
//
//				vec3 vertex(x, y, 0);
//				verts.push_back(vertex);
//				printf("My X: %fMy Y: %f\n", vertex.x(), vertex.y());
//			}
//		}
//		for (int i = 0; i < howBig; i++){
//			if (i % W == W - 1){
//				continue;
//			}
//			if (i == (W*W) - W){
//				break;
//			}
//
//			indis.push_back(i);
//			indis.push_back(i + W);
//			indis.push_back(i + 1);
//			indis.push_back(i + W);
//			indis.push_back(i + W + 1);
//			indis.push_back(i + 1);
//			
//		}
//	}

/// Create a box with octet
int main(int argc, char **argv) {
	
	// set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::wave app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();
}
