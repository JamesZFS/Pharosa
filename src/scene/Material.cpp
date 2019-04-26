//
// Created by James on 2019/4/26.
//

#include "Material.h"

Material::Material() : color(Color::WHITE), emi(Emission::NONE), reft(Object::DIFF)
{

}

void Material::BRDF(const Ray &in, const Dir &normal, Ray &out)
{
	// todo
	warn("Material::BRDF() is not finished!");
	out.dir = in.dir - normal * (in.dir % normal * 2);
}
