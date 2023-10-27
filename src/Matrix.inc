#include <cassert>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <utility>
#include <iomanip>

/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( T value)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at( row).at( column) = value;
		}
	}
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( const std::initializer_list< T >& aList)
{
	// Check the arguments
	assert( aList.size() == M * N);

	auto row_iter = aList.begin();
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column, ++row_iter)
		{
			matrix.at( row).at( column) = *row_iter;
		}
	}
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( const std::initializer_list< std::initializer_list< T > >& aList)
{
	// Check the arguments, the static assert assures that there is at least 1 M and 1 N!
	assert( aList.size() == M && (*aList.begin()).size() == N);

	auto row_iter = aList.begin();
	for (std::size_t row = 0; row < aList.size(); ++row, ++row_iter)
	{
		auto column_iter = (*row_iter).begin();
		for (std::size_t column = 0; column < (*row_iter).size(); ++column, ++column_iter)
		{
			matrix.at( row).at( column) = *column_iter;
		}
	}
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( const Matrix< T, M, N >& aMatrix) :
				matrix( aMatrix.matrix)
{
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
std::array< T, N >& Matrix< T, M, N >::at( std::size_t aRowIndex)
{
	return matrix.at( aRowIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
const std::array< T, N >& Matrix< T, M, N >::at( std::size_t aRowIndex) const
{
	return matrix.at( aRowIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
T& Matrix< T, M, N >::at( 	std::size_t aRowIndex,
							std::size_t aColumnIndex)
{
	return matrix.at( aRowIndex).at( aColumnIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
const T& Matrix< T, M, N >::at( std::size_t aRowIndex,
								std::size_t aColumnIndex) const
{
	return matrix.at( aRowIndex).at( aColumnIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
std::array< T, N >& Matrix< T, M, N >::operator[]( std::size_t aRowIndex)
{
	return matrix[aRowIndex];
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
const std::array< T, N >& Matrix< T, M, N >::operator[]( std::size_t aRowIndex) const
{
	return matrix[aRowIndex];
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >& Matrix< T, M, N >::operator=( const Matrix< T, M, N >& rhs)
{
	if (this != &rhs)
	{
		matrix = rhs.matrix;
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
bool Matrix< T, M, N >::operator==( const Matrix< T, M, N >& rhs) const
{
	return matrix == rhs.matrix;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N >& Matrix< T, M, N >::operator*=( const T2& scalar)
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at( row).at( column) *= scalar;
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N > Matrix< T, M, N >::operator*( const T2& scalar) const
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	Matrix< T, M, N > result( *this);
	return result *= scalar;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N >& Matrix< T, M, N >::operator/=( const T2& aScalar)
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at( row).at( column) /= aScalar;
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N > Matrix< T, M, N >::operator/( const T2& aScalar) const
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	Matrix< T, M, N > result( *this);
	return result /= aScalar;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >& Matrix< T, M, N >::operator+=( const Matrix< T, M, N >& rhs)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix[row][column] += rhs.at( row, column);
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::operator+( const Matrix< T, M, N >& rhs) const
{
	Matrix< T, M, N > result( *this);
	return result += rhs;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >& Matrix< T, M, N >::operator-=( const Matrix< T, M, N >& rhs)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix[row][column] -= rhs.at( row, column);
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::operator-( const Matrix< T, M, N >& rhs) const
{
	Matrix< T, M, N > result( *this);
	return result -= rhs;
}
/**
 * (M, N) * (N, P) -> (M, P)
 */
template< typename T, std::size_t M, std::size_t N >
template< std::size_t columns >
Matrix< T, M, columns > Matrix< T, M, N >::operator*( const Matrix< T, N, columns >& rhs) const
{
	// TODO Implement this function

	Matrix< T, M, columns > result;
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < columns; ++column)
		{
			for(std::size_t factor = 0; factor < N; ++factor)
			{
				result.at( row).at( column) += this->at( row).at( factor) * rhs.at( factor).at( column);
			}
		}
	}
	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, N, M > Matrix< T, M, N >::transpose() const
{
	Matrix< T, N, M > result;
	for (std::size_t row = 0; row < N; ++row)
	{
		for (std::size_t column = 0; column < M; ++column)
		{
			result.at( row).at( column) = this->at( column).at( row);
		}
	}
	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::identity() const
{
	Matrix< T, N, M > result;
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			if(row == column)
			{
				result.at( row).at( column) = 1;
			}
			else
			{
				result.at( row).at( column) = 0;
			}
		}
	}
	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::gauss() const
{

	Matrix< T, M, N > result(*this);


	for(std::size_t gaus_row = 0; gaus_row < M; ++gaus_row)
	{
		std::size_t max_val_row = gaus_row;
		T max_val = result.at(max_val_row).at(max_val_row);
		for(std::size_t get_max_row = gaus_row; get_max_row < M; ++get_max_row)
		{
			if(std::abs(result.at(get_max_row).at(gaus_row) > max_val))
			{
				max_val_row = get_max_row;
				max_val = std::abs(result.at(get_max_row).at(gaus_row));
			}
		}

		std::array< T, N > temp = result.at(max_val_row);
		result.at(max_val_row) = result.at(gaus_row);
		result.at(gaus_row) = temp;



		T pivot = result.at( gaus_row).at( gaus_row);
		for(std::size_t gaus_column = 0; gaus_column < N; ++gaus_column)
		{
			result.at(gaus_row).at(gaus_column) = result.at(gaus_row).at(gaus_column) / pivot;
		}

		if(gaus_row != M - 1){
			for(std::size_t to_zero_row = gaus_row + 1; to_zero_row < M; ++to_zero_row)
			{

				T to_zero_cell_val = result.at( to_zero_row).at( gaus_row);
				for(std::size_t column = 0; column < N; ++column)
				{
					result.at(to_zero_row).at(column) = result.at(to_zero_row).at(column) - (to_zero_cell_val * result.at(gaus_row).at(column));
				}
			}
		}
	}

	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::gaussJordan() const
{
	Matrix < T, M, N > result;

	result = this->gauss();

	for(std::int32_t pivot_row = M - 1; pivot_row > 0; --pivot_row)
	{
		for(std::int32_t to_reduce_row = pivot_row - 1; to_reduce_row >= 0; --to_reduce_row)
		{
			T to_zero_cell_val = result.at( to_reduce_row).at( pivot_row);
			for(std::size_t column = 0; column < N; ++column)
			{
				result.at(to_reduce_row).at(column) = result.at(to_reduce_row).at(column) - (to_zero_cell_val * result.at(pivot_row).at(column));
			}
		}
	}
	return result;
}
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, 1 > Matrix< T, M, N >::solve() const
{
	Matrix < T, M, 1 > result;
	Matrix < T, M, N> temp = this->gaussJordan();
	for(std::size_t row = 0; row < M; ++row)
	{
		result.at(row).at(0) = temp.at(row).at(N - 1);
	}

	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::inverse() const
{
	Matrix< T, N, M > result;
	Matrix< T, M, 2*N> temp;
	Matrix< T, M, N > I = this->identity();
	for(std::size_t row = 0; row < M; ++row)
	{
		for(std::size_t column = 0; column < N; ++column)
		{
			temp.at(row).at(column) = this->at(row).at(column);
			temp.at(row).at(N + column) = I.at(row).at(column);
		}
	}
	temp = temp.gaussJordan();
	for(std::size_t row = 0; row < M; ++row)
	{
		for(std::size_t column = 0; column < N; ++column)
		{
			result.at(row).at(column) = temp.at(row).at(N + column);
		}
	}
	return result;
}
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::diagonal() const
{
	Matrix< T, M, N > result(*this);
	for(size_t row = 0; row < M; ++row)
	{
		for(size_t column = 0; column < M; ++column)
		{
			if(row != column)
			{
				result.at(row).at(column) = 0;
			}
		}
	}
	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
std::string Matrix< T, M, N >::to_string() const
{
	std::string result = "Matrix<" + std::to_string( N) + "," + std::to_string( M) + ">\n{\n";
	for (std::size_t i = 0; i < M; ++i)
	{
		for (std::size_t j = 0; j < N; ++j)
		{
			result += std::to_string( matrix[i][j]) + ",";
		}
		result += "\n";
	}
	result += "}";
	return result;
}

namespace MatrixUtils
{

	/**
	 *
	 */
	template< typename T, const std::size_t N >
	bool equals(	const Matrix< T, 1, N >& lhs,
					const Matrix< T, 1, N >& rhs,
					const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
					const unsigned long aFactor /*= 1*/)
	{
		for(std::size_t column = 0; column < N; ++column)
		{
			T diff = std::abs(lhs.at(0).at(column) - rhs.at(0).at(column));
			if(diff > (aPrecision * aFactor))
			{
				return false;
			}
		}
		return true;
	}
	/**
	 *
	 */
	template< typename T, const std::size_t M >
	bool equals(	const Matrix< T, M, 1 >& lhs,
					const Matrix< T, M, 1 >& rhs,
					const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
					const unsigned long aFactor /*= 1*/)
	{
		for(std::size_t row = 0; row < M; ++row)
		{
			T diff = std::abs(lhs.at(row).at(0) - rhs.at(row).at(0));
			if(diff > (aPrecision * aFactor))
			{
				return false;
			}
		}
		return true;
	}
	/**
	 *
	 */
	template< typename T, const std::size_t M, const std::size_t N >
	bool equals(	const Matrix< T, M, N >& lhs,
					const Matrix< T, M, N >& rhs,
					const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
					const unsigned long aFactor /*= 1*/)
	{
		for(std::size_t row = 0; row < M; ++row)
		{
			for(std::size_t column = 0; column < N; ++column){
				T diff = std::abs(lhs.at(row).at(column) - rhs.at(row).at(column));
				if(diff > (aPrecision * aFactor))
				{
					return false;
				}
			}
		}
		return true;
	}

	template< typename T, std::size_t M, std::size_t N >
	Matrix< T, 1, N > transition(const Matrix< T, M, N >& transitionMatrix, const Matrix< T, 1, M >& stateVector, const uint16_t steps)
	{
		Matrix< T, 1, N > result;
		Matrix< T, M, N > temp;
		temp = temp.identity();
		for(std::size_t step_nr = 0; step_nr < steps; ++step_nr)
		{
			temp = temp * transitionMatrix;
		}
		result = stateVector * temp;
		return result;
	}

	template< typename T, std::size_t M, std::size_t N >
	Matrix< T, M, 1 > transition(const Matrix< T, M, N >& transitionMatrix, const Matrix< T, N, 1 >& stateVector, const uint16_t steps)
	{
		Matrix< T, M, 1 > result;
		Matrix< T, M, N > temp;
		temp = temp.identity();
		for(std::size_t step_nr = 0; step_nr < steps; ++step_nr)
		{
			temp = temp * transitionMatrix.transpose();
		}
		result = temp * stateVector;
		return result;
	}

	template< typename T, std::size_t M_tm, std::size_t N_tm, std::size_t M_sv, std::size_t N_sv >
	Matrix< T, M_sv, N_sv > steadyState(const Matrix< T, M_tm, N_tm >& transitionMatrix, const Matrix< T, M_sv, N_sv >& stateVector, const unsigned long aFactor)
	{
		uint16_t steps = 0;
		bool steadyStateFound = false;
		Matrix< T, M_sv, N_sv > temp(stateVector);
		while(!steadyStateFound)
		{
			std::cout << "----------------------------------------" << std::endl;
			std::cout << transition(transitionMatrix, temp, steps) << std::endl;
			std::cout << transition(transitionMatrix, temp, steps + 1) << std::endl;
			if(equals(transition(transitionMatrix, temp, steps), transition(transitionMatrix, temp, steps + 1), std::numeric_limits<T>::epsilon(), aFactor))
			{
				steadyStateFound = true;
				std::cout << steps << std::endl;
			}
			else
			{
				++steps;
			}
		}
		return transition(transitionMatrix, temp, steps);
	}

	template< typename T, std::size_t M, std::size_t N, std::size_t M_u, std::size_t N_u >
	void kalmanFilter(Matrix< T, M, N >& stateVector,
							Matrix< T, M, M >& covarianceMatrix,
							const Matrix< T, M, M >& transitionMatrixA,
							const Matrix< T, M_u, N_u >& updateVector,
							const Matrix< T, M, M_u >& transitionMatrixB,
							const Matrix< T, M, M >& transitionMatrixC,
							const Matrix< T, M, M >& Q,
							const Matrix< T, M, N >& measurement)
	{
		Matrix< T, M, N > predictedStateVector;
		Matrix< T, M, M > predictedCovarianceMatrix;
		Matrix< T, M, M > kalmanGain;
		Matrix< T, M, N > measurementVector;

		predictedStateVector = getPredictedStateVector(stateVector, transitionMatrixA, updateVector, transitionMatrixB);
		predictedCovarianceMatrix = getPredictedCovarianceMatrix(covarianceMatrix, transitionMatrixA);
		kalmanGain = getKalmanGain(predictedCovarianceMatrix, transitionMatrixC, Q);
		measurementVector = getMeasurementVector(measurement, transitionMatrixC);
		stateVector = getAdjustedStateVector(predictedStateVector, transitionMatrixC, kalmanGain, measurementVector);
		covarianceMatrix = getAdjustedCovarianceMatrix(predictedCovarianceMatrix, transitionMatrixC, kalmanGain);
		covarianceMatrix = covarianceMatrix.diagonal();
	}

	template< typename T, std::size_t M, std::size_t N, std::size_t M_u, std::size_t N_u >
	void kalmanFilter(Matrix< T, M, N >& stateVector,
							Matrix< T, M, M >& covarianceMatrix,
							const Matrix< T, M, M >& transitionMatrixA,
							const Matrix< T, M_u, N_u >& updateVector,
							const Matrix< T, M, M_u >& transitionMatrixB,
							const Matrix< T, M, M >& transitionMatrixC,
							const Matrix< T, M, M >& Q,
							const Matrix< T, M, N >& measurement,
							const Matrix< T, M, N >& stateVectorError,
							const Matrix< T, M, M >& covarianceError,
							const Matrix< T, M, N >& measurementError)
	{
		Matrix< T, M, N > predictedStateVector;
		Matrix< T, M, M > predictedCovarianceMatrix;
		Matrix< T, M, M > kalmanGain;
		Matrix< T, M, N > measurementVector;

		predictedStateVector = getPredictedStateVector(stateVector, transitionMatrixA, updateVector, transitionMatrixB, stateVectorError);
		predictedCovarianceMatrix = getPredictedCovarianceMatrix(covarianceMatrix, transitionMatrixA, covarianceError);
		kalmanGain = getKalmanGain(predictedCovarianceMatrix, transitionMatrixC, Q);
		measurementVector = getMeasurementVector(measurement, transitionMatrixC, measurementError);
		stateVector = getAdjustedStateVector(predictedStateVector, transitionMatrixC, kalmanGain, measurementVector);
		covarianceMatrix = getAdjustedCovarianceMatrix(predictedCovarianceMatrix, transitionMatrixC, kalmanGain);
		covarianceMatrix = covarianceMatrix.diagonal();
	}

	template< typename T, std::size_t M, std::size_t N, std::size_t M_u, std::size_t N_u >
	Matrix< T, M, N > getPredictedStateVector(const Matrix< T, M, N >& stateVector,
											const Matrix< T, M, M >& transitionMatrixA,
											const Matrix< T, M_u, N_u >& updateVector,
											const Matrix< T, M, M_u >& transitionMatrixB)
	{
		Matrix< T, M, N > result;
		result = (transitionMatrixA * stateVector) + (transitionMatrixB * updateVector);
		return result;
	}

	template< typename T, std::size_t M, std::size_t N, std::size_t M_u, std::size_t N_u >
	Matrix< T, M, N > getPredictedStateVector(const Matrix< T, M, N >& stateVector,
											const Matrix< T, M, M >& transitionMatrixA,
											const Matrix< T, M_u, N_u >& updateVector,
											const Matrix< T, M, M_u >& transitionMatrixB,
											const Matrix< T, M, N >& stateVectorError)
	{
		Matrix< T, M, N > result;
		result = (transitionMatrixA * stateVector) + (transitionMatrixB * updateVector) + stateVectorError;
		return result;
	}

	template< typename T, std::size_t M >
	Matrix< T, M, M > getPredictedCovarianceMatrix(const Matrix< T, M, M >& covarianceMatrix,
													const Matrix< T, M, M >& transitionMatrixA)
	{
		Matrix< T, M, M > result;
		result = (transitionMatrixA * covarianceMatrix) * transitionMatrixA.transpose();
		return result.diagonal();
	}

	template< typename T, std::size_t M >
	Matrix< T, M, M > getPredictedCovarianceMatrix(const Matrix< T, M, M >& covarianceMatrix,
													const Matrix< T, M, M >& transitionMatrixA,
													const Matrix< T, M, M >& covarianceError)
	{
		Matrix< T, M, M > result;
		result = ((transitionMatrixA * covarianceMatrix) * transitionMatrixA.transpose()) + covarianceError;
		return result.diagonal();
	}

	template< typename T, std::size_t M >
	Matrix< T, M, M > getKalmanGain(const Matrix< T, M, M >& predictedCovarianceMatrix,
									const Matrix< T, M, M >& transitionMatrixC,
									const Matrix< T, M, M >& Q)
	{
		Matrix< T, M, M > result;
		Matrix< T, M, M > divident;
		Matrix< T, M, M > divisor;
		result = result.identity();
		divident = (predictedCovarianceMatrix * transitionMatrixC.transpose());
		divident = divident.diagonal();

		divisor = ((transitionMatrixC * predictedCovarianceMatrix) * transitionMatrixC.transpose()) + Q;
		divisor = divisor.diagonal();

		for(size_t pivot = 0; pivot < M; ++pivot)
		{
			result.at(pivot).at(pivot) = divident.at(pivot).at(pivot) / divisor.at(pivot).at(pivot);
		}
		return result;
	}

	template< typename T, std::size_t M, std::size_t N >
	Matrix< T, M, N > getMeasurementVector(const Matrix< T, M, N >& measurement,
											const Matrix< T, M, M >& transitionMatrixC)
	{
		Matrix< T, M, N > result;
		result = transitionMatrixC * measurement;
		return result;
	}

	template< typename T, std::size_t M, std::size_t N >
	Matrix< T, M, N > getMeasurementVector(const Matrix< T, M, N >& measurement,
											const Matrix< T, M, M >& transitionMatrixC,
											const Matrix< T, M, N >& measurementError)
	{
		Matrix< T, M, N > result;
		result = (transitionMatrixC * measurement) + measurementError;
		return result;
	}

	template< typename T, std::size_t M, std::size_t N >
	Matrix< T, M, N > getAdjustedStateVector(const Matrix< T, M, N >& predictedStateVector,
											const Matrix< T, M, M >& transitionMatrixC,
											const Matrix< T, M, M >& kalmanGain,
											const Matrix< T, M, N >& measurementVector)
	{
		Matrix< T, M, N > result;
		result = predictedStateVector + (kalmanGain * (measurementVector - (transitionMatrixC * predictedStateVector)));
		return result;
	}

	template< typename T, std::size_t M >
	Matrix< T, M, M > getAdjustedCovarianceMatrix(const Matrix< T, M, M >& predictedCovarianceMatrix,
												const Matrix< T, M, M >& transitionMatrixC,
												const Matrix< T, M, M >& kalmanGain)
	{
		Matrix< T, M, M > result;
		result = (result.identity() - (kalmanGain * transitionMatrixC)) * predictedCovarianceMatrix;
		return result.diagonal();
	}
} // namespace MatrixUtils
