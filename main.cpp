/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */

#include "src/Renderer.h"
#include "src/utils/funcs.hpp"
#include "src/test.h"
using namespace std;

#ifdef __DEV_STAGE__
size_t __counter__ = 0;
real __debug__ = 0;
size_t __print_cnt__ = 0;
size_t __max_depth__ = 0;

#endif

int main(int argc, char *argv[])
{
//	Test::main();
	// **************** parse cmd args ****************
	if (argc < 2) TERMINATE("usage: Pharosa <json_config_path>");
	const String config_path = argv[1];

	// **************** init render engine ****************
	Renderer renderer(config_path);

	// **************** start rendering ****************
	renderer.start();

	// **************** save results ****************
	renderer.save();

#ifdef __DEV_STAGE__
	debug("\n -- debug info -- \n");
	debug("  print_cnt = %ld\n", __print_cnt__);
	debug("  max_depth = %ld\n", __max_depth__);
	debug("  counter   = %ld\n", __counter__);
	debug("  debug     = %f\n", __debug__);
#endif

	return 0;
}
