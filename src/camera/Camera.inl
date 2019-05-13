
void Camera::render(size_t i, size_t j, const Color &color)
{
	img.at(i, j) += color;
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

size_t Camera::renderCount() const
{
	return render_cnt;
}