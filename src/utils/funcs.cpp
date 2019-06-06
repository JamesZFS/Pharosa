//
// Created by James on 2019/4/9.
//

#include "funcs.hpp"

namespace Funcs
{
	std::default_random_engine generator((unsigned int) clock());
	std::uniform_real_distribution<real> uniform_distribution(0.0, 1.0);
	std::normal_distribution<real> normal_distribution(0.0, 1.0);

	real factorial[11] = {
			1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800
	};
}
