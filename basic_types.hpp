#ifndef MNB_LIBSVM_BASIC_TYPES_HPP
#define MNB_LIBSVM_BASIC_TYPES_HPP

#include <cstdint>
#include <vector>
#include <deque>
#include <iostream>

//#define LOG(msg) std::cout << msg << std::endl
#define LOG(msg)

typedef uint32_t                                        feature_id_t;
typedef double                                          feature_value_t;
typedef int                                             cat_id_t;
typedef int                                             doc_id_t;

const feature_id_t UNDEFINED_FEATURE_ID = -1;
const cat_id_t UNDEFINED_CAT_ID = 0;

typedef std::pair<feature_id_t, feature_value_t>        feature_t;

typedef std::deque<feature_t>                          doc_t;

struct doc_info_t
{
    doc_info_t(doc_id_t did = 0, cat_id_t c = UNDEFINED_CAT_ID, const doc_t &d = doc_t())
        : id(did), cat(c), doc(d)
    { }
    
    doc_id_t    id;
    cat_id_t    cat;
    doc_t       doc;
};

bool doc_info_cat_id_asc(const doc_info_t & a , const doc_info_t & b);
bool doc_info_cat_asc(const doc_info_t & a , const doc_info_t & b);

typedef std::deque<doc_info_t>                         doc_list_t;

#endif // MNB_LIBSVM_BASIC_TYPES_HPP
