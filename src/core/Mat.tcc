
template<typename T>
Mat<T>::Mat(const T (&a)[3][3])
{
	el = new double[3][3];
	memcpy(el, a, sizeof(a));
}

template<typename T>
Mat<T>::~Mat()
{
	delete[] el;
}

template<typename T>
Mat<T>::Mat(T k)    // init diag
{
	el = new double[3][3]{};
	el[0][0] = el[1][1] = el[2][2] = k;
}

template<typename T>
Mat<T> Mat<T>::operator*(const Mat<T> &B) const
{
	Mat C;
	C.el = new T[3][3];
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
Mat<T>::Mat(const Mat &b)
{
	el = new double[3][3];
	memcpy(el, b.el, 9 * sizeof(T));
}

template<typename T>
Mat<T> &Mat<T>::operator=(const Mat &b)
{
	delete[] el;
	el = new double[3][3];
	memcpy(el, b.el, 9 * sizeof(T));
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
	C.el = new T[3][3];
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
	b.el = nullptr;
}

template<typename T>
Mat<T> &Mat<T>::operator=(Mat &&b) noexcept
{
	el = std::move(b.el);
	b.el = nullptr;
	return *this;
}
