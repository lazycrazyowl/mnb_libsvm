#ifndef MNB_LIBSVM_NAIVE_BAYES_HPP
#define MNB_LIBSVM_NAIVE_BAYES_HPP

#include <google/sparse_hash_map>
#include <google/sparse_hash_set>

#include "basic_types.hpp"

class MultinomialNaiveBayes
{
public:
    MultinomialNaiveBayes();

    void add_doc(const doc_info_t &doc);
    void finalize();
    cat_id_t classify(const doc_t &doc) const;

private:
    typedef google::sparse_hash_map<
          feature_id_t
        , feature_value_t >                             feat_prob_map_t;

    struct cat_info_t
    {
        cat_info_t()
            : amount_of_docs(0)
        {
            features.set_deleted_key(UNDEFINED_FEATURE_ID);
        }

        feat_prob_map_t                                 features;
        uint32_t                                        amount_of_docs;
    };

    typedef google::sparse_hash_map<
          cat_id_t
        , cat_info_t >                                  cats_map_t;

    cats_map_t                                          m_cats;
    google::sparse_hash_set<feature_id_t>               m_vocab;
};

#endif // MNB_LIBSVM_NAIVE_BAYES_HPP
