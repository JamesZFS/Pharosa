
#define rankOf(i, j) ((j) * width + (i))
#define checkCoordinate(i, j) assert(0 <= (i) && (i) < width && 0 <= (j) && (j) < height)

void Camera::render(size_t rank, const Color &color)
{
	img[rank] += color;
}

void Camera::renderAt(size_t i, size_t j, const Color &color)
{
	auto rank = rankOf(i, j);
	img[rank] += color;
}

Ray Camera::shootRay(size_t rank) const
{
	return shootRayAt(rank % width, rank / width, 0.5);
}

const Color &Camera::pixelAt(size_t i, size_t j) const
{
	return img[rankOf(i, j)];
}

const Pos &Camera::viewpoint() const
{
	return pos;
}

const Dir &Camera::orientation() const
{
	return ez;
}

Ray Camera::shootRayAt(double i, double j, double sigma) const
{
	return shootRayAt(i + Funcs::randfNormal(0, sigma), j + Funcs::randfNormal(0, sigma));
}

void Camera::rotate(const ElAg &euler_angles)
{
	(ex = Dir::X_AXIS).rotate(euler_angles).rotate(ea);
	(ey = Dir::Y_AXIS).rotate(euler_angles).rotate(ea);
	(ez = Dir::Z_AXIS).rotate(euler_angles).rotate(ea);
	ea = ez.getEulerAngles();
}

void Camera::translate(const Pos &delta)
{
	pos += delta;
}

void Camera::step()
{
	++render_cnt;
}


#undef rankOf
#undef checkCoordinate
