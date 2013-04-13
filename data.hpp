#ifndef MNB_LIBSVM_DATA_HPP
#define MNB_LIBSVM_DATA_HPP

#include "basic_types.hpp"
#include <string>

doc_info_t load_doc(std::string &src);

doc_list_t load_data_file(std::string &fname);

#endif // MNB_LIBSVM_DATA_HPP
