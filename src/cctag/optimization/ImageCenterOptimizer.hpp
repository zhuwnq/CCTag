#ifdef USE_IMAGE_CENTER_OPT // undefined. Depreciated

#ifndef VISION_IMAGECENTEROPTIMIZER_HPP_
#define	VISION_IMAGECENTEROPTIMIZER_HPP_

#include <cctag/Global.hpp>
#include <cctag/geometry/Point.hpp>
#include <cctag/ImageCut.hpp>
#include <cctag/algebra/matrix/Matrix.hpp>
#include <cctag/geometry/Ellipse.hpp>

#include <OPT++_config.h>
#include <OptLBFGS.h>
#include <NLF.h>
#include <OptQNewton.h>

#include <boost/numeric/ublas/vector.hpp>
#include <opencv2/core/core.hpp>

#include <cstddef>
#include <vector>


namespace cctag {
namespace identification {

/**
 * @brief Optimizer to find the subpixel position of an edge point from a 1D signal.
 */
class ImageCenterOptimizer : public OPTPP::FDNLF1
{
public:
	typedef ImageCenterOptimizer This;
	typedef OPTPP::FDNLF1 Parent;
	typedef std::vector< cctag::Point2dN<double> > VecExtPoints;
	typedef std::vector< cctag::ImageCut > VecSignals;

public:
	ImageCenterOptimizer( const VecExtPoints & vecExtPoints );

	/**
	 * @brief Do optimization.
	 *
	 * All pararameters are used to initialize the optimization.
	 * @param[in] pToRefine initial point to refine
	 * @return refined 2D point
	 */
	Point2dN<double> operator()(
                const cctag::Point2dN<double> & pToRefine,
                const std::size_t lengthSig,
                const cv::Mat & src,
                const cctag::numerical::geometry::Ellipse & outerEllipse,
                const cctag::numerical::BoundedMatrix3x3d & mT );

	inline void initFcn()
	{
		if ( init_flag == false )
		{
			initOpt( dim, mem_xc );
			init_flag = true;
		}
		else
		{
			CCTAG_COUT_ERROR( "FDNLF1:initFcn: Warning - initialization called twice\n" );
			initOpt( dim, mem_xc );
		}
	}

private:
	/// @brief Optimization initialization function.
	void initOpt( int ndim, NEWMAT::ColumnVector& x );
	/// @brief Optimization cost function.
	static void optimizePointFun( int n, const NEWMAT::ColumnVector& x, double& fx, int& result, void* );

private:
	const VecExtPoints & _vecExtPoints;
	cctag::Point2dN<double> _pToRefine;
	std::size_t _lengthSig;
	cv::Mat _src;
	cctag::numerical::geometry::Ellipse _ellipse;
	std::size_t _numIter;
	cctag::numerical::BoundedMatrix3x3d _mT;
	cctag::numerical::BoundedMatrix3x3d _mInvT;
};

} // namespace identification
} // namespace cctag

#endif


#endif // USE_IMAGE_CENTER_OPT // undefined. Depreciated