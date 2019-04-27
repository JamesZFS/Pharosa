
#define rankOf(i, j) ((j) * width + (i))
#define checkCoordinate(i, j) assert(0 <= (i) && (i) < width && 0 <= (j) && (j) < height)

void Camera::render(const Color &color)
{
	img[cur_rank] += color;
	++render_cnt[cur_rank];    // counts up rendering time of current pixel
}

void Camera::render(size_t rank, const Color &color)
{
	img[rank] += color;
	++render_cnt[rank];
}

void Camera::renderAt(size_t i, size_t j, const Color &color)
{
	auto rank = rankOf(i, j);
	img[rank] += color;
	++render_cnt[rank];
}

bool Camera::finishedVerbose(size_t n_step) const
{
	assert(n_step > 0);
	if (cur_rank % n_step == 0) {
		printf("\r  progress:  %.1f %%", cur_rank * 100.0 / size);
		fflush(stdout);
	}
	return (cur_rank >= size);
}

void Camera::updateProgress()
{
	++cur_rank;
	if (++cur_i == width) {        // first ++i then ++j
		cur_i = 0;
		if (++cur_j > height) {
			warn("Warning: pixel index overflows.\n");
			resetProgress();
		}
	}
}

bool Camera::finished() const
{
	return (cur_rank >= size);
}

void Camera::resetProgress()
{
	cur_i = cur_j = cur_rank = 0;
}

void Camera::translate(const Pos &delta)
{
	pos += delta;
}

Ray Camera::shootRay() const
{
	return shootRayAt(cur_i, cur_j, 0.5);
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

#undef rankOf
#undef checkCoordinate
