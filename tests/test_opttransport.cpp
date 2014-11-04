#include <MA/optimal_transport.hpp>
#include <boost/timer/timer.hpp>
#include <lbfgs.hpp>
#include <cstdlib>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Vector_2<K> Vector;
typedef CGAL::Line_2<K> Line;
typedef CGAL::Polygon_2<K> Polygon;

typedef Eigen::SparseMatrix<FT> SparseMatrix;
typedef Eigen::SparseVector<FT> SparseVector;
typedef Eigen::VectorXd VectorXd;
typedef Eigen::MatrixXd MatrixXd;

typedef CGAL::Delaunay_triangulation_2<K> T;

double rr() 
{ 
  return 2*double(rand() / (RAND_MAX + 1.0))-1;
}


int main(int argc, const char **argv)
{
  if (argc < 2)
    return -1;
  size_t N = 100;
  if (argc >= 3)
    N = atoi(argv[2]);

  std::map<T::Face_handle,
	   MA::Linear_function<K>> functions;
  T t;
  cimg_library::CImg<double> image(argv[1]);
  double total_mass = MA::image_to_pl_function(image, t, functions);
  boost::timer::auto_cpu_timer tm(std::cerr);

  // generate points
  MatrixXd X(N,2);
  VectorXd masses(N);
  for (size_t i = 0; i < N; ++i)
    {
      X(i,0) = rr();
      X(i,1) = rr();
      masses(i) = total_mass/N;
    }

  VectorXd res;
  MA::ot_solve(t, functions, X, masses, res);
  return 0;
}

