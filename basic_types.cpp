#include "basic_types.hpp"

bool doc_info_cat_id_asc(const doc_info_t & a , const doc_info_t & b)
{
    if (a.cat == b.cat)
        return a.id < b.id;
    return a.cat < b.cat;
}

bool doc_info_cat_asc(const doc_info_t & a , const doc_info_t & b)
{
    return a.cat <= b.cat;
}