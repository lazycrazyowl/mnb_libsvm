#ifndef MNB_LIBSVM_BASIC_TYPES_HPP
#define MNB_LIBSVM_BASIC_TYPES_HPP

#include <cstdint>
#include <vector>

typedef uint32_t                                        feature_id_t;
typedef double                                          feature_value_t;
typedef int                                             cat_id_t;

const feature_id_t UNDEFINED_FEATURE_ID = -1;
const cat_id_t UNDEFINED_CAT_ID = 0;

typedef std::pair<feature_id_t, feature_value_t>        feature_t;

typedef std::vector<feature_t>                          doc_t;

struct doc_info_t
{
    doc_info_t(cat_id_t c = UNDEFINED_CAT_ID, const doc_t &d = doc_t())
        : cat(c), doc(d)
    { }
        
    cat_id_t    cat;
    doc_t       doc;
};

typedef std::vector<doc_info_t>                         doc_list_t;

#endif // MNB_LIBSVM_BASIC_TYPES_HPP
