
void Camera::render(const Color &color)
{
	img[cur_rank] += color;
	++render_cnt[cur_rank];    // counts up rendering time of current pixel
}

void Camera::render(unsigned int rank, const Color &color)
{
	img[rank] += color;
	++render_cnt[rank];
}

void Camera::renderAt(unsigned int i, unsigned int j, const Color &color)
{
	auto rank = rankOf(i, j);
	img[rank] += color;
	++render_cnt[rank];
}

#undef rankOf
#undef checkCoordinate

bool Camera::finishedVerbose(unsigned int n_step) const
{
	assert(n_step > 0);
	if (cur_rank % n_step == 0) {
		debug("\r  progress:  %.1f %%", cur_rank * 100.0 / size);
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
