template<typename T>
Mat<T>::Mat() : el(List2D<T>(3, List<T>(3, 0)))
{
}

template<typename T>
Mat<T>::Mat(List2D<T> &&a) noexcept : el(std::move(a))
{
	assert(el.size() == 3 && el[0].size() == 3 && el[1].size() == 3 && el[2].size() == 3); // noexcept
}

template<typename T>
Mat<T>::Mat(T k) : el(List2D<T>(3, List<T>(3, 0)))   // init diag
{
	el[0][0] = el[1][1] = el[2][2] = k;
}

template<typename T>
Mat<T> Mat<T>::operator*(const Mat<T> &B) const
{
	Mat C;
	C.el[0][0] = el[0][0] * B.el[0][0] + el[0][1] * B.el[1][0] + el[0][2] * B.el[2][0];
	C.el[0][1] = el[0][0] * B.el[0][1] + el[0][1] * B.el[1][1] + el[0][2] * B.el[2][1];
	C.el[0][2] = el[0][0] * B.el[0][2] + el[0][1] * B.el[1][2] + el[0][2] * B.el[2][2];
	C.el[1][0] = el[1][0] * B.el[0][0] + el[1][1] * B.el[1][0] + el[1][2] * B.el[2][0];
	C.el[1][1] = el[1][0] * B.el[0][1] + el[1][1] * B.el[1][1] + el[1][2] * B.el[2][1];
	C.el[1][2] = el[1][0] * B.el[0][2] + el[1][1] * B.el[1][2] + el[1][2] * B.el[2][2];
	C.el[2][0] = el[2][0] * B.el[0][0] + el[2][1] * B.el[1][0] + el[2][2] * B.el[2][0];
	C.el[2][1] = el[2][0] * B.el[0][1] + el[2][1] * B.el[1][1] + el[2][2] * B.el[2][1];
	C.el[2][2] = el[2][0] * B.el[0][2] + el[2][1] * B.el[1][2] + el[2][2] * B.el[2][2];
	return std::move(C);
}

template<typename T>
Mat<T> &Mat<T>::operator*=(const Mat<T> &B)
{
	*this = *this * B;
	return *this;
}

template<typename T>
Mat<T> Mat<T>::operator*(T k) const
{
	el[0][0] *= k;
	el[0][1] *= k;
	el[0][2] *= k;
	el[1][0] *= k;
	el[1][1] *= k;
	el[1][2] *= k;
	el[2][0] *= k;
	el[2][1] *= k;
	el[2][2] *= k;
	return *this;
}

template<typename T>
Vec<T> Mat<T>::operator*(const Vec<T> &b) const
{
	return {el[0][0] * b.x + el[0][1] * b.y + el[0][2] * b.z,
			el[1][0] * b.x + el[1][1] * b.y + el[1][2] * b.z,
			el[2][0] * b.x + el[2][1] * b.y + el[2][2] * b.z};
}

template<typename T>
void Mat<T>::report() const
{
	for (int i = 0; i < 3; ++i) {
		debug("[ ");
		for (int j = 0; j < 3; ++j) {
			debug("%5.2f  ", el[i][j]);
		}
		debug("]\n");
	}
	debug("\n");
}

template<typename T>
Mat<T> Mat<T>::operator+(const Mat<T> &B) const
{
	Mat C;
	C.el[0][0] = el[0][0] + B.el[0][0];
	C.el[0][1] = el[0][1] + B.el[0][1];
	C.el[0][2] = el[0][2] + B.el[0][2];
	C.el[1][0] = el[1][0] + B.el[1][0];
	C.el[1][1] = el[1][1] + B.el[1][1];
	C.el[1][2] = el[1][2] + B.el[1][2];
	C.el[2][0] = el[2][0] + B.el[2][0];
	C.el[2][1] = el[2][1] + B.el[2][1];
	C.el[2][2] = el[2][2] + B.el[2][2];
	return std::move(C);
}

template<typename T>
Mat<T> Mat<T>::operator+=(const Mat<T> &B)
{
	el[0][0] += B.el[0][0];
	el[0][1] += B.el[0][1];
	el[0][2] += B.el[0][2];
	el[1][0] += B.el[1][0];
	el[1][1] += B.el[1][1];
	el[1][2] += B.el[1][2];
	el[2][0] += B.el[2][0];
	el[2][1] += B.el[2][1];
	el[2][2] += B.el[2][2];
	return *this;
}

template<typename T>
Mat<T>::Mat(Mat &&b) noexcept : el(std::move(b.el))
{
}

template<typename T>
Mat<T> &Mat<T>::operator=(Mat &&b) noexcept
{
	el = std::move(b.el);
	return *this;
}

template<typename T>
void Mat<T>::reset()
{
	el = List2D<T>(3, List<T>(3, 0));
}
