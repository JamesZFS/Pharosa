
//
// Created by James on 2019/4/5.
//

#include "InfPlane.h"
#include "../scene/Object.h"

InfPlane::InfPlane() : InfPlane(Pos::ORIGIN, ElAg::NONROT)
{
}

InfPlane::InfPlane(const Pos &p_, const ElAg &euler_angles_) :
		Geometry(p_, euler_angles_)
{
	applyTransform();
}

InfPlane::InfPlane(const Dir &n_, const Pos &p_) :
		Geometry(p_, n_.getEulerAngles()), p(p_), n(n_)
{
/*#ifdef __DEV_STAGE__
	printf("n_: ");
	n_.report(true);

	auto ea = n_.getEulerAngles();
	printf("ea of n_: (%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);

	printf("n: ");
	n.report(true);
	printf("\n");
#endif*/
}

void InfPlane::applyTransform()
{
	p = mat.tra;
	n = mat.rot * Dir::Z_AXIS;
}
