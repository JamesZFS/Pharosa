//
// Created by James on 2019/4/9.
//

template<typename Render_Algorithm, typename Camera_Type>
Renderer<Render_Algorithm, Camera_Type>::Renderer() :
		_stage(nullptr), _camera(nullptr), _illuminator(nullptr), prev_epoch(0)
{
}

template<typename Render_Algorithm, typename Camera_Type>
Renderer<Render_Algorithm, Camera_Type>::~Renderer()
{
	delete _stage;
	delete _camera;
	delete _illuminator;
}

template<typename Render_Algorithm, typename Camera_Type>
Stage &Renderer<Render_Algorithm, Camera_Type>::stage()
{
	if (_stage == nullptr) {
		warn("Error: stage is not setup yet.\n");
		exit(1);
	}
	return *_stage;
}

template<typename Render_Algorithm, typename Camera_Type>
Camera &Renderer<Render_Algorithm, Camera_Type>::camera()
{
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	return *_camera;
}

template<typename Render_Algorithm, typename Camera_Type>
void Renderer<Render_Algorithm, Camera_Type>::
setupStage(const String &config_path)
{
	if (_stage != nullptr) delete _stage;
	_stage = new Stage;            // new an empty stage
	if (config_path.length() > 0) {
		_stage->fromJsonFile(config_path);
	}
}

template<typename Render_Algorithm, typename Camera_Type>
void Renderer<Render_Algorithm, Camera_Type>::
setupCamera(const Pos &pos_, const ElAg &euler_angles_, size_t width_, size_t height_,
			const String &prev_path_, size_t prev_epoch_)
{
	prev_epoch = prev_epoch_;
	if (_camera != nullptr) delete _camera;
	_camera = new Camera_Type(pos_, euler_angles_, width_, height_);    // new a camera

	// load from prev
	if (prev_path_.length() > 0) {
		_camera->readPPM(prev_path_, prev_epoch_);
	}
}

template<typename Render_Algorithm, typename Camera_Type>
void Renderer<Render_Algorithm, Camera_Type>::
setupAlgorithm()
{
	if (_stage == nullptr) {
		warn("Warning: stage is not setup yet, the result shall be empty.\n");
		_stage = new Stage;
	}
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	// setup algorithm
	if (_illuminator == nullptr) {
		_illuminator = new Render_Algorithm(*_stage, *_camera);
	}
	printf("loaded %ld singletons, %ld bounding boxes in total.\n",
		   _stage->getSingletonCount(), _stage->getBoundingBoxCount());
	printf("camera viewpoint at %s  orienting towards %s\n",
		  camera().viewpoint().toString().data(), camera().orientation().toString().data());
	printf("ready to render.\n");
}

template<typename Render_Algorithm, typename Camera_Type>
void Renderer<Render_Algorithm, Camera_Type>::renderFrame(size_t n_epoch, size_t verbose_step,
														  const String &checkpoint_dir)
{
	if (verbose_step == 0)
		_illuminator->render(n_epoch, prev_epoch, checkpoint_dir);
	else
		_illuminator->renderVerbose(n_epoch, prev_epoch, verbose_step, checkpoint_dir);
}

template<typename Render_Algorithm, typename Camera_Type>
void Renderer<Render_Algorithm, Camera_Type>::start(size_t n_epoch,
													size_t verbose_step, const String &checkpoint_dir)
{
	setupAlgorithm();
	printf("===== rendering start =====\n");
	if (checkpoint_dir.length() > 0) {
		printf("writing to checkpoint \"%s\"", checkpoint_dir.data());
	}

	double since = omp_get_wtime();
	renderFrame(n_epoch, verbose_step, checkpoint_dir);
	auto elapse = lround(omp_get_wtime() - since);

	printf("\n===== rendering finished in %ld min %ld sec =====\n", elapse / 60, elapse % 60);
	prev_epoch += n_epoch;
}

template<typename Render_Algorithm, typename Camera_Type>
void Renderer<Render_Algorithm, Camera_Type>::
startKinetic(size_t n_frame, void (*motion)(), size_t n_epoch,
			 size_t verbose_step, const String &checkpoint_dir)
{
	if (prev_epoch > 0) {
		warn("Error: before rendering kinetic images, you should not pre-read from previous image.\n");
		exit(1);
	}
	setupAlgorithm();
	bool checkpoint = (checkpoint_dir.length() > 0);
	if (checkpoint) {
		printf("writing to checkpoint \"%s\"", checkpoint_dir.data());
	}
	printf("======= kinetic rendering start =======\n");

	double since = omp_get_wtime();
	for (size_t frame = 0; frame < n_frame; ++frame) {
		printf("\n===== frame %ld / %ld =====\n", frame + 1, n_frame);
		renderFrame(n_epoch, verbose_step, "");
		motion();    // deal with motion
		if (checkpoint) {    // save checkpoint for each frame
			Buffer out_path;
			sprintf(out_path, "%s/frame - %ld (samps = %ld).ppm", checkpoint_dir.data(), frame + 1, n_epoch);
			_camera->writePPM(out_path);
		}
	}
	auto elapse = lround(omp_get_wtime() - since);

	printf("\n======= kinetic rendering finished in %ld min %ld sec =======\n", elapse / 60, elapse % 60);
}

template<typename Render_Algorithm, typename Camera_Type>
void Renderer<Render_Algorithm, Camera_Type>::save(const String &out_path)
{
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	_camera->writePPM(out_path);
	printf("image written to \"%s\" \n", out_path.data());
}
