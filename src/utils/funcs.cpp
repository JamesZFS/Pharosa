//
// Created by James on 2019/4/9.
//

#include "funcs.hpp"

namespace Funcs
{
	std::default_random_engine generator((unsigned int) clock());
	std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);
	std::normal_distribution<double> normal_distribution(0.0, 1.0);

	double factorial[11] = {
			1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800
	};
}
