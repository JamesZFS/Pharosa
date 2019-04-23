/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */

#include "src/Renderer.h"
#include "src/test.h"
using namespace std;

#ifdef __DEV_STAGE__
size_t __counter__ = 0;
size_t __print_cnt__ = 0;

#endif

int main(int argc, char *argv[])
{
	// init random engine
//	if (argc < 2) TERMINATE("usage: Pharosa <json_config_path>");
//	Renderer renderer(argv[1]);
	Test::render(argc, argv);
	return 0;
}
