#ifndef FS_HPP_HPBGAO0G
#define FS_HPP_HPBGAO0G

#ifdef BOOST_FS
    #include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#else
    #include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif /* ifndef  */


#endif /* end of include guard: FS_HPP_HPBGAO0G */
